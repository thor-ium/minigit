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

bool isFileInSLL(singlyNode* head, string filename) {
    while(head != NULL) {
        if (head->fileName == filename) {
            return true;
        }
        head = head->next;
    }
    return false;
}

void Git::init() {
    fs::create_directory(".miniGit");
    doublyNode* curr = new doublyNode();
    curr->commitNumber = 0;
    comHead = curr;
    curr->next = NULL;
    curr->previous = NULL;
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
    while(comHead->next != NULL) {
        comHead = comHead->next;
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
    // traverses commit LL
    while (tempD != NULL) {
            tempS = tempD->head;
            // traverses file LL
            while (tempS != NULL) {
                // checks if file already excists in LL
                if (tempS->fileName == filename && areFilesIdentical(tempS, filename)) {
                    cout << "Filename '" << filename << "' already taken!" << endl;
                    return;
                }
                tempS = tempS->next;
        }
        tempD = tempD->next;
    }
    doublyNode* currCom = getCurrCommit(comHead);
    singlyNode* addFile = new singlyNode();
    addFile->next = NULL;
    tempS = currCom->head;
    singlyNode* tempPrev = NULL;
    // if first file added in linkedList
    if (currCom->head == NULL) {
        currCom->head = addFile;
        addFile->fileName = filename;

    // all other cases
    } else {
        cout << "s" << endl;
    // finds last node in LL and its previous node
        while (tempS !=  NULL) {
            tempPrev = tempS;
            tempS = tempS->next; 
        }
        addFile->fileName = filename;
        tempPrev->next = addFile;
    }
}
void Git::remove() {
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
            fs::current_path(".minigit");
            fs::remove(filename);
            tempPrev->next == NULL;
            delete tempS;
            tempS = nullptr;
        } else if (comHead->head == tempS){
            fs::current_path(".minigit");
            fs::remove(filename);
            comHead->head == tempS->next;
            delete tempS;
            tempS = nullptr;
        } else {
            fs::current_path(".minigit");
            fs::remove(filename);
            tempPrev->next = tempS->next;
            delete tempS;
            tempS = nullptr;
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

int Git::commit() {
    
    string tempName;
    doublyNode* currCom = getCurrCommit(comHead);
    singlyNode* tempS = currCom->head;
    doublyNode* check = comHead;
    singlyNode* checkS = NULL;
    debug_printLL(tempS);
    while (tempS != NULL) {
        checkS = check->head;
        if (tempS->fileName == checkS->fileName) {
            cout << "A" << endl;
            if (areFilesIdentical(tempS, checkS->fileName)) {
                cout << "C" << endl;
                copy(tempS->fileName, tempS->fileName + "_" + to_string(tempS->fileVersion));
            } else {
                cout << "B" << endl;
                copy(tempS->fileName, tempS->fileName + "_" + to_string(tempS->fileVersion++));
            }
         } else {
            copy(tempS->fileName, tempS->fileName + "_" + to_string(tempS->fileVersion++));

         }
        //     if (!areFilesIdentical(tempS, checkS->fileName)) {
        //         cout << "B" << endl;
                
        //         tempS->fileVersion++;
        //         tempName = tempS->fileName + "_" + to_string(tempS->fileVersion);
        //         copy(tempS->fileName, tempName);
        //     }
        // }
        tempS = tempS->next;
        checkS = checkS->next;

    }
    // doublyNode* newCom = new doublyNode();
    // newCom->previous = currCom;
    // newCom->next = NULL;
    

}
void Git::checkout() {

}
