#include <iostream>
#include <fstream>

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
    fs::create_directiory(".miniGit");
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
    inSLL.open(existingFile->fileName);
    inNew.open(newFile);
    string lineSLL;
    string lineNew;
    while (!inSLL.end) {
        getline(inSLL, lineSLL);
        getline(inNew, lineNew);
        if (lineSLL != lineNew) {
            return false;
        }
    }
    return true;
}

void Git::add() {
    // asks for filename until valid file in directory
    string filename = getFileName();
    singlyNode* tempS = NULL;

    doublyNode* tempD = comHead;
    int newFileVers = 0;
    // traverses commit LL
    while (tempD != NULL) {
            tempS = tempD->head;
            // traverses file LL
            while (tempS != NULL) {
                // checks if file already excists in LL
                if (tempS->fileName == filename) {
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
        addFile->fileName = filename + "_" + addFile->fileVersion;
    // all other cases
    } else {
    // finds last node in LL and its previous node
        while (tempS !=  NULL) {
            tempPrev = tempS;
            tempS = tempS->next; 
        }
        addFile->fileName = filename + "_" + addFile->fileVersion;
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
            tempPrev->next == NULL;
            delete tempS;
            tempS = nullptr;
        } else if (comHead->head == tempS){
            comHead->head == tempS->next;
            delete tempS;
            tempS = nullptr;
        } else {
            tempPrev->next = tempS->next;
            delete tempS;
            tempS = nullptr;
        }
    }



}

// void copy_to_repo(string filename) {
//     ifstream in;
//     ofstream out;
//     string line;
//     in.open(filename);
//     out.open(filename);
//     while(getline(in, line)) {
//         out << line;
//     }
// }

int Git::commit() {
    doublyNode* currCom = getCurrCommit(comHead);
    singlyNode* tempS = currCom->head;
    doublyNode* check = comHead;
    singlyNode* checkS = NULL;
    while (check->next != NULL) {
        checkS = check->head;
        if (tempS->fileName != checkS->fileName) {

        }
    }
    // doublyNode* newCom = new doublyNode();
    // newCom->previous = currCom;
    // newCom->next = NULL;
    

}
void Git::checkout() {

}
