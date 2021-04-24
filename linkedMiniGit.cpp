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

string copy(string file, string newFile) {
    string line;
    ifstream in;
    ofstream out;
    in.open(file);
    while(getline(in, line)) {
            

        }
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
        in.open(filename);
        currCom->head = addFile;
        addFile->fileName = addFile->fileVersion + "_" + filename;

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
        if (tempS->fileName == checkS->fileName) {
            
        }
    }
    // doublyNode* newCom = new doublyNode();
    // newCom->previous = currCom;
    // newCom->next = NULL;
    

}
void Git::checkout() {

}
