#pragma once
#ifndef LINKEDGIT_H
#define LINKEDGIT_H

#include <string>

using namespace std;


// file linked list
struct singlyNode {
    string fileName;
    string fileVersion;
    singlyNode* next;
};

// commit Linked List
struct doublyNode {
    int commitNumber;
    singlyNode* head;
    doublyNode* previous;
    doublyNode* next;
};

class Git {
    private:
         doublyNode* comHead = NULL;
    public:
        Git();
        ~Git();
        bool init();
        void add();
        void remove();
        void commit();
        void checkout();
};

#endif