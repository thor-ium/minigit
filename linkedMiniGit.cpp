#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include <filesystem>
namespace fs = std::filesystem;

#include "linkedGit.hpp"
#include <vector>

Git::Git() {
    fs::remove_all(".minigit");
    fs::create_directory(".minigit");
}
Git::~Git() {

}

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

void debug_printLL(singlyNode* A) {
    while (A != NULL) {
        cout << A->fileName << " -> ";
        A = A->next;
    }
    cout << endl;
}

int hashAddress(string filename, int table_size) {
    int hash = 0;
    ifstream in;
    string line;
    string curr_direc = fs::current_path();
    in.open(filename);
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
    
    fs::current_path(curr_direc);
    return hash % table_size;


}

bool isFileInSLL(singlyNode* head, string filename) {
    while(head != NULL) {
        if (head->fileName == filename) {
            return true;
        }
        head = head->next;
    }
    return false;
}

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
doublyNode* getCurrCommit(doublyNode* comHead) {
    doublyNode* curr = comHead;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    return curr;
}

doublyNode* insertLast(doublyNode* head, doublyNode* prev, int comitnumber) {
    doublyNode* temp = new doublyNode();
    temp->commitNumber = comitnumber + 1;
    temp->next = NULL;
    temp->previous = prev;

    if (head == NULL) {
        cout << "OOOOOH i forgot to set head of DLL to first commit" << endl;
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



// copies function into .minigit directory then returning to original directory
void copy(string file, string newFile) {
    string line;
    ifstream in;
    ofstream out;
    int i = 0;
    in.open(file);
    string curr_direct = fs::current_path();
    fs::current_path(".minigit");
    out.open(newFile);
    while(getline(in, line)) {
        out << line << endl;
    }
    fs::current_path(curr_direct);
}
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
            cout << "oh?" << endl;
            split(currSLLhead->fileVersion, '.', splitFilename, 3);
            return to_string(stoi(splitFilename[2]) + 1);

        }
        currSLLhead = currSLLhead->next;
    }
    return "0";
}
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
            if (hashAddress(filename, 100) == hashAddress(head_->fileName, 100)) {
                cout << "File: " << filename << " already exists." << endl;
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
    debug_printLL(tempD->head);
}
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


bool existsNowhere(doublyNode* comhead, singlyNode* curr) {
    if (comhead->previous == NULL ) {
        cout << "im so " << endl;
        return true;
    }
    cout << "Y" << endl;
    singlyNode* prev = comhead->previous->head;
    cout << "X" << endl;
    while (comhead != NULL) {
        cout << "42 mil" << endl;
        prev = comhead->previous->head;
        cout << "went to ibiza" << endl;
        while (prev != NULL) {
            cout << "hmmm" << endl;
            cout << "prev:" << prev->fileName << endl;
            cout << "curr: " << curr->fileName << endl;
            if (prev->fileName == curr->fileName) {
                return false;
            }
            prev = prev->next;
        }
        comhead = comhead->next;
        
    }
    return true;
}

singlyNode* copyLL(singlyNode* head_O) {
    if (head_O == NULL) {
        return NULL;
    }
    singlyNode* clone = new singlyNode();
    clone->fileName = head_O->fileName;
    clone->fileVersion = head_O->fileVersion;
    clone->next = copyLL(head_O->next);
    return clone;
}

void Git::commit() {
    
    doublyNode* currentCommit = getCurrCommit(comHead);
    doublyNode* tempComHead = comHead;
    cout << currentCommit->head->fileName << endl;
    singlyNode* curr_SLL = currentCommit->head;
    singlyNode* prev_SLL = NULL;
    string currFile[3]; 
    cout << "currcom num: " << currentCommit->commitNumber << endl;
    if (currentCommit->commitNumber == 0) {
        while (curr_SLL != NULL) {
            cout << "entered" << endl;
            split(curr_SLL->fileVersion, '.' , currFile, 3);
            cout << curr_SLL->fileVersion << endl;
            copy(curr_SLL->fileName, curr_SLL->fileVersion);
            curr_SLL = curr_SLL->next;
        }
    } else {
        prev_SLL = currentCommit->previous->head;
        debug_printLL(prev_SLL);
        while (prev_SLL != NULL) {
            cout << "prev_SLL:" << prev_SLL->fileVersion << endl;
            cout << "curr_SLL:" << curr_SLL->fileVersion << endl;
            if (prev_SLL->fileName == curr_SLL->fileName) {
                if (hashAddress(prev_SLL->fileVersion, 100) != hashAddress(curr_SLL->fileName, 100)) {
                    string temp[3];
                    split(prev_SLL->fileVersion, '.', temp, 3);
                    curr_SLL->fileVersion = temp[0] + "." + to_string(stoi(temp[1]) + 1) + "." + temp[2];
                    cout << "to_string(stoi(temp[1]) + 1) : " << to_string(stoi(temp[1]) + 1) << endl;
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
        curr_SLL = currentCommit->head;
        tempComHead = comHead;
        if (existsNowhere(currentCommit, curr_SLL)) {
            copy(curr_SLL->fileName, curr_SLL->fileVersion);
        }
        


    }
    doublyNode* nextCom = insertLast(comHead, currentCommit, currentCommit->commitNumber);
}
void Git::checkout() {

}
