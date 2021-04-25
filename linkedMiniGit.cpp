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
    in.open(filename);
    while(getline(in, line)) {
        for (int i = 0; i < line.length(); i++) {
            hash = hash + (int) line[i];
        }
    }
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
    comHead = curr;
    curr->next = NULL;
    curr->previous = NULL;
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
    int i = 0;
    if (comHead->next ==  NULL) {
        return comHead;
    }
    while(comHead != NULL) {
        comHead = comHead->next;
        i++;
        cout <<i << endl;
    }
    return comHead;
}

bool areFilesIdentical(singlyNode* existingFile, string newFile) {
    ifstream inSLL;
    ifstream inNew;
    string lineE;
    string lineN;
    inSLL.open(existingFile->fileName);
    inNew.open(newFile);
    inSLL.open(existingFile->fileName);
    inNew.open(newFile);
    int E = 0;
    int N = 0;
    string line;
    while(getline(inSLL, line)) {
        E++;
    }
    while(getline(inNew, line)) {
        N++;
    }
    if (N != E) {
        return false;
    }
    while(getline(inSLL, lineE)) {
        getline(inNew, lineN);
        if (lineE != lineN) {
            return false;
        }
    }
    return true;
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

void Git::add() {
    // asks for filename until valid file in directory
    string filename = getFileName();
    singlyNode* tempS = NULL;
    ifstream in;
    ofstream out;
    string line;
    doublyNode* tempD = comHead;
    int newFileVers = 0;
    // traverses file LL
    while (tempS != NULL) {
        // checks if file already exists in LL

        if (tempS->fileName == filename + "_" + to_string(tempS->fileVersion)) {
            cout << "Filename '" << filename << "' already taken!" << endl;
            return;
        }
        tempS = tempS->next;
    }
    
    doublyNode* currCom = getCurrCommit(comHead);
    singlyNode* addFile = new singlyNode();
    addFile->next = NULL;
    tempS = currCom->head;
    singlyNode* tempPrev = NULL;
    // if first file added in linkedList
    if (currCom->head == NULL) {
        currCom->head = addFile;
        addFile->fileName = filename + "_" + to_string(addFile->fileVersion);

    // all other cases
    } else {
    // finds last node in LL and its previous node
        while (tempS !=  NULL) {
            tempPrev = tempS;
            tempS = tempS->next; 
        }
        addFile->fileName = filename + "_" + to_string(addFile->fileVersion);
        tempPrev->next = addFile;
    }
}
void Git::remove() {
    string curr_direct;
    string filename;
    cout << "Enter the name of the file to be deleted:" << endl;
    cin >> filename;
    doublyNode* currCom = getCurrCommit(comHead);
    singlyNode* tempS = currCom->head;
    singlyNode* tempPrev = NULL;
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
        if (tempS->next == NULL) {
            curr_direct = fs::current_path();
            fs::current_path(".minigit");
            fs::remove(filename);
            tempPrev->next = NULL;
            delete tempS;
            tempS = nullptr;
            fs::current_path(curr_direct);
        } else if (comHead->head == tempS){
            curr_direct = fs::current_path();
            fs::current_path(".minigit");
            fs::remove(filename);
            comHead->head = tempS->next;
            delete tempS;
            tempS = nullptr;
            fs::current_path(curr_direct);
        } else {
            curr_direct = fs::current_path();
            fs::current_path(".minigit");
            fs::remove(filename);
            tempPrev->next = tempS->next;
            delete tempS;
            tempS = nullptr;
            fs::current_path(curr_direct);

        }
    }



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

int Git::commit() {
    
    
    // get current and previous heads of SLLs
    string currComfile[2];
    string prevComfile[2];
    doublyNode* currentCommit = getCurrCommit(comHead);
    singlyNode* SLL_currCom = currentCommit->head;
    if (currentCommit != comHead) {
        cout << "next commit" << endl;
        doublyNode* prevCommit = currentCommit->previous;
        singlyNode* SLL_prevCom = prevCommit->head;
        int curr = 0, prev = 0;
        while (SLL_currCom != NULL) {
            curr++;
            SLL_currCom = SLL_currCom->next;
        }
        while (SLL_prevCom != NULL) {
            prev++;
            SLL_prevCom = SLL_prevCom->next;
        }
        SLL_currCom = currentCommit->head;
        SLL_prevCom = prevCommit->head;
        if (curr == prev) {
            while (SLL_currCom != NULL) {
                split(SLL_currCom->fileName, '_', currComfile, 2);
                split(SLL_prevCom->fileName, '_', prevComfile, 2);
                if (currComfile[0] == prevComfile[0]) {
                    if (hashAddress(currComfile[0], 100) == hashAddress(prevComfile[0], 100)) {
                        cout << "Current commit is up to date" << endl;
                    } else {
                        SLL_currCom->fileVersion = SLL_prevCom->fileVersion + 1;
                        copy(currComfile[0], currComfile[0] + "_" + to_string(SLL_currCom->fileVersion));
                    }
                    SLL_currCom = SLL_currCom->next;
                    SLL_prevCom = SLL_prevCom->next;
                } else {
                    copy(currComfile[0], currComfile[0] + "_" + to_string(SLL_currCom->fileVersion));
                }
            }
        }
    } else {
        // add files to .minigit
        cout << "First commit" << endl;
        while (SLL_currCom != NULL) {
            split(SLL_currCom->fileName, '_', currComfile, 2);
            copy(currComfile[0], currComfile[0] + "_" + to_string(SLL_currCom->fileVersion));
            SLL_currCom = SLL_currCom->next;
        }
    }
    SLL_currCom = currentCommit->head;
    doublyNode* nextCom = new doublyNode();
    nextCom->previous = currentCommit;
    nextCom->next = NULL;
    nextCom->commitNumber = currentCommit->commitNumber + 1;
    cout << "Current commit: ";
    debug_printLL(SLL_currCom);
    SLL_currCom = currentCommit->head;
    nextCom->head = copyLL(SLL_currCom);
    cout << "Next commit: ";
    debug_printLL(nextCom->head);






}
void Git::checkout() {

}
