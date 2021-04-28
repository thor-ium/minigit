#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "linkedGit.hpp"
#include <vector>
// constructor
Git::Git() {
    fs::remove_all(".minigit");
    fs::create_directory(".minigit");
}
// deconstructor
Git::~Git() {
    fs::remove_all(".minigt");
    comHead = NULL;
    delete comHead;
    comHead = nullptr;

}
// function used to split filenames at "." 
int split(string stringSplit, char splitUpPoint, string array[], int size)
{
    int initial = 0;
    int splitPointASCII = int(splitUpPoint);
    int i = 0 ;
    int j = 0;
    int currentPosition = 0;
    int length = 0;
    int currIndex = 0;
    int splitCount;
    stringSplit = stringSplit;
    string word = "";
    // checks that string has a length
    if (stringSplit.length() == 0)
    {
        return 0;
    } else 
    {
        //counts number of splits
        for (j = 0; j < stringSplit.length(); j++)
        {
            if (int(stringSplit[j]) != splitPointASCII)
            {

            } else
            {
                splitCount++;
            }
            
        }
        // if there is at least one split...
        if (splitCount >= 1)
        {
            // fixes errors with no split on last word in string
            stringSplit = stringSplit + splitUpPoint;
            for ( i = 0; i < stringSplit.length(); i++)
            {
                // when the current index of the string doesn't equal a split point
                if (int(stringSplit[i]) != int(splitUpPoint))
                {
                    // adds the characters to a variable string ( before a split point)
                    word = word + stringSplit[i];
                    
                    
                }
                else 
                {
                    /* if split detected, takes word and assigns it to an array index,
                     * changes the index and resets the word to an empty string
                    */
                    array[currIndex] = word;
                    currIndex++;
                    word = "";
                }
            }
            // if the final number of indices is larger than the size, return -1
            if (size < currIndex)
            {
                return -1;
            } else 
            {
                return currIndex;
            }
        // if no splits are detected output the original string in the 0 index of the array
        } else if (splitCount == 0)
        {
            array[0] = stringSplit;
            return 1;
        } else
        {
            return 1;
        }
    }
}

// function to make sure SLLs formed correctly
void debug_printLL(singlyNode* A) {
    while (A != NULL) {
        cout << A->fileVersion << " -> ";
        A = A->next;
    }
    cout << endl;
}

// function used to form hash address for given file and table size
int hashAddress(string filename, int table_size) {
    int hash = 0;
    ifstream in;
    string line;
    string curr_direc = fs::current_path();
    in.open(filename);
    // if file is in .minigit directory, switch directories
    if (in.fail()) {
        fs::current_path(".minigit");
        in.open(filename);
        while(getline(in, line)) {
            for (int i = 0; i < line.length(); i++) {
                hash = hash + (int) line[i];
            }
        }
    } else {
        while(getline(in, line)) {
            for (int i = 0; i < line.length(); i++) {
                hash = hash + (int) line[i];
            }
        }
    }
    // go back to main directory
    fs::current_path(curr_direc);
    return hash % table_size;


}

// initialize .minigit repository and create first DLL node
// bool function in order to check if initialized
bool Git::init() {
    fs::create_directory(".minigit");
    doublyNode* curr = new doublyNode();
    curr->commitNumber = 0;
    curr->next = NULL;
    curr->previous = NULL;
    curr->head = NULL;
    comHead = curr;
    return true;
}
// functio to check if filename does not exist in main directory
string getFileName() {
    string filename;
    cout << "Enter a filename: " << endl;
    cin >> filename;
    while(!fs::exists(filename)) {
        cout << "File not found in directory." << endl;
        cout << "------------------------------" << endl;
        cout << "Renter a filename" << endl;
        cin >> filename;
    }
    return filename;

}

// function to find current commit for use in traversing
doublyNode* getCurrCommit(doublyNode* comHead) {
    doublyNode* curr = comHead;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    return curr;
}


// function that inserts next commit node at the end of the DLL
doublyNode* insertLast(doublyNode* head, doublyNode* prev, int comitnumber) {
    doublyNode* temp = new doublyNode();
    temp->commitNumber = comitnumber + 1;
    temp->next = NULL;
    temp->previous = prev;

    if (head == NULL) {
        head = temp;
    } else {
        doublyNode* last = head;

        while(last->next != NULL) {
            last = last->next;
        }
        last->next = temp;
        temp->previous = last;
    }
    return temp;
}



// copies file into .minigit directory then returning to original directory
void copy(string file, string newFile) {
    string line;
    ifstream in;
    ofstream out;
    in.open(file);
    string curr_direct = fs::current_path();
    fs::current_path(".minigit");
    out.open(newFile);
    while(getline(in, line)) {
        out << line << endl;
    }
    fs::current_path(curr_direct);
}

// function to properly assign file version. used only in add() function
string getFileVersion(doublyNode* curr, string filename) {
    singlyNode* currSLLhead;
    string splitFilename[3];
    while (curr->next != NULL) {
        curr = curr->next;
    }
    currSLLhead = curr->head;
    if (currSLLhead == NULL) {
        return "0";
    }
    while(currSLLhead != NULL) {
        if (filename == currSLLhead->fileName) {
            split(currSLLhead->fileVersion, '.', splitFilename, 3);
            return to_string(stoi(splitFilename[1]) + 1);

        }
        currSLLhead = currSLLhead->next;
    }
    return "0";
}

// function to add file to current SLL that will then be set 
// as data memeber of DLL in commit() function
void Git::add() {
    // asks for filename until valid file in directory
    string filename = getFileName();
    doublyNode* tempD = getCurrCommit(comHead);
    singlyNode* tempS = new singlyNode();
    singlyNode* head_ = tempD->head;
    singlyNode* head_prev = NULL;
    string splittedFilename[2];
    if (tempD->head == NULL) {
        tempS->fileName = filename;
        split(filename, '.', splittedFilename, 2);
        tempS->fileVersion = splittedFilename[0] + "." + getFileVersion(comHead, filename) + "." + splittedFilename[1];
        tempS->next = NULL;
        tempD->head = tempS;
    } else {
        while (head_ != NULL) {
            if (hashAddress(filename, 100) == hashAddress(head_->fileVersion, 100)) {
                    cout << "File: " << filename << " already exists or is unedited." << endl;
                    return;
                }
            head_prev = head_;
            head_ = head_->next;
        }
        tempS->next = NULL;
        tempS->fileName = filename;
        split(filename, '.', splittedFilename, 2);
        tempS->fileVersion = splittedFilename[0] + "." + getFileVersion(comHead, filename) + "." + splittedFilename[1];
        head_prev->next = tempS;
    }
}

// function to remove a file from current SLL before being committed
void Git::remove() {
    string curr_direct;
    string filename;
    cout << "Enter the name of the file to be deleted:" << endl;
    cin >> filename;
    doublyNode* currCom = getCurrCommit(comHead);
    singlyNode* tempS = currCom->head;
    singlyNode* tempPrev = NULL;
    tempS = currCom->head;
    while (tempS != NULL) {
        if (tempS->fileName == filename) {
            break;
        }
        tempPrev = tempS;
        tempS = tempS->next;
    }
    if (tempS == NULL) {
        cout << "File not found." << endl;
    } else {
        if (currCom->head == tempS){
            comHead->head = tempS->next;
            delete tempS;
            tempS = nullptr;
        } else {
            tempPrev->next = tempS->next;
            delete tempS;
            tempS = nullptr;
        }
    }
}

// function used in commit() to check if file has ever been committed
bool existsNowhere(doublyNode* comhead, doublyNode* curr) {
    bool tf = true;
    int p = 0;
    int c = 0;
    singlyNode* curr_SLL = curr->head;
    if (comhead->previous == NULL ) {
        return true;
    }
    singlyNode* prev = comhead->previous->head;
    while (curr_SLL != NULL) {
        c++;
        curr_SLL = curr_SLL->next;
    }
    while (comhead != NULL) {
        prev = comhead->previous->head;
        
        while (prev != NULL) {
            
            prev = prev->next;
            p++;
        }
        if (p == c) {
            prev = comhead->previous->head;
            curr_SLL = curr->head;
            while (prev != NULL) {
                if (prev->fileVersion == curr_SLL->fileVersion) {
                    return false;
                }
                prev = prev->next;
            }
        } else {
            tf = true;
        }
        comhead = comhead->next;
        
    }
    return tf;
}

// recursive function to copyLL to next commit
singlyNode* copyLL(singlyNode* head_O) {
    if (head_O == NULL) {
        return head_O;
    }
    singlyNode* clone = new singlyNode();
    if (hashAddress(head_O->fileVersion, 100) == hashAddress(head_O->fileName, 100)) {
            clone->fileName = head_O->fileName;
            clone->fileVersion = head_O->fileVersion;
            clone->next = copyLL(head_O->next);
            
     }
    return clone;
    
}

// checks new linked list in next commit for doubly added nodes
void deleteRedundancies(singlyNode* head) {
    singlyNode* a;
    singlyNode* b;
    singlyNode* reduc;
    a = head;

    while (a != NULL && a->next != NULL) {
        b = a; 

        while (b->next != NULL) {
            if (a->fileVersion == b->next->fileVersion) {
                reduc = b->next;
                b->next = b->next->next;
                delete reduc;
            } else {
                b = b->next;
            }
        }
        a = a->next;
    }
}


/* 
  the commit function takes the current SLL and checks if
  it has been committed in it's current version. If it hasn't
  it will copy the file with the name of the file 
  to the ".minigit" repository in the following format:
        filename: test.txt
        (if first time committed or file unchanged) fileVersion: test.0.txt
        (if test.txt is altered) fileVersion: test.1.txt
    Finally, a new DLL node to represent the next commit is created for the
    next time the add() function is executed
*/
void Git::commit() {
    doublyNode* currentCommit = getCurrCommit(comHead);
    if(currentCommit->head == NULL) {
        cout << "Nothing to commit." << endl;
        return;
    }
    doublyNode* tempComHead = comHead;
    singlyNode* curr_SLL = currentCommit->head;
    singlyNode* prev_SLL = NULL;
    string currFile[3]; 
    if (currentCommit->commitNumber == 0) {
        while (curr_SLL != NULL) {
            split(curr_SLL->fileVersion, '.' , currFile, 3);
            copy(curr_SLL->fileName, curr_SLL->fileVersion);
            curr_SLL = curr_SLL->next;
        }
    } else {
        prev_SLL = currentCommit->previous->head;
        while (tempComHead != NULL) {
            while (prev_SLL != NULL) {
                if (prev_SLL->fileName == curr_SLL->fileName) {
                    if (hashAddress(prev_SLL->fileVersion, 100) != hashAddress(curr_SLL->fileName, 100)) {
                        string temp[3];
                        split(prev_SLL->fileVersion, '.', temp, 3);
                        curr_SLL->fileVersion = temp[0] + "." + to_string(stoi(temp[1]) + 1) + "." + temp[2];
                        copy(curr_SLL->fileName, curr_SLL->fileVersion);

                        
                    }
                }
                prev_SLL = prev_SLL->next;
                if (curr_SLL->next != NULL) {
                    curr_SLL = curr_SLL->next;
                } else {
                    break;
                }
            }
            tempComHead = tempComHead->next;
        }
        curr_SLL = currentCommit->head;
        tempComHead = comHead;
        if (existsNowhere(currentCommit, currentCommit)) {
            while (curr_SLL != NULL) {
                copy(curr_SLL->fileName, curr_SLL->fileVersion);
                curr_SLL = curr_SLL->next;
            }
        }
        


    }
    doublyNode* nextCom = insertLast(comHead, currentCommit, currentCommit->commitNumber);
    curr_SLL = currentCommit->head;
    if (currentCommit->previous != NULL) {
        curr_SLL = currentCommit->head;
        prev_SLL = currentCommit->previous->head;
        nextCom->head = copyLL(currentCommit->head); 
        
        curr_SLL = currentCommit->head;
    } else {
        nextCom->head = copyLL(curr_SLL);
    }
    deleteRedundancies(nextCom->head);
    


}

// copyCheck is the inverse of the copy() function.
// instead of copying a file from the main directory to .minigit,
// it copies a file from .minigit to the main directory
// Function used in checkout(). 
void copyCheck(string fileVersion, string file) {
    string line;
    ifstream in;
    ofstream out;
    string curr_direct = fs::current_path();
    fs::current_path(".minigit");
    in.open(fileVersion);
    fs::current_path(curr_direct);
    out.open(file);
    while(getline(in, line)) {
        out << line << endl;
    }

}

/* function searches for a specified commit and restores that version of the file
 * to the main directory. Utilizes copyCheck() function in order to copy back to 
   main directory. 
*/
void Git::checkout() {
    bool tf = false;
    int comCheck = 0;
    cout << "Enter commit number: " << endl;
    cin >> comCheck;
    singlyNode* checkS;
    doublyNode* traverse_com = comHead;
    doublyNode* currentCommit = getCurrCommit(comHead);
    if (currentCommit->commitNumber == 0) {
        cout << "first commit currently in progress, nothing to do." << endl;
        return;
    }
    if (currentCommit->commitNumber == comCheck) {
        cout << "Commit currently in progress, nothing to do." << endl;
        return;
    }
    if (currentCommit->commitNumber < comCheck) {
        cout << "Commit does not exist!" << endl;
        return;
    }
    while (traverse_com != NULL) {
        if (traverse_com->commitNumber == comCheck) {
            break;
        }
        traverse_com = traverse_com->next;
    }
    if (traverse_com != NULL) {
        checkS = traverse_com->head;
    }    
    while (checkS != NULL) {
        if (hashAddress(checkS->fileVersion, 100) != hashAddress(checkS->fileName, 100)) {
            copyCheck(checkS->fileVersion, checkS->fileName);
        } else {
            tf = true;
        }
        checkS = checkS->next;
    } 
    if (tf == true) {
        cout << "Given commit: #" << traverse_com->commitNumber << " is the current version of files in main directory." << endl;
    }
}
