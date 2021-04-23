#pragma once
#ifndef LINKEDGIT_H
#define LINKEDGIT_H

#include <string>

using namespace std;

struct singlyNode {
    string fileName;
    string fileVersion = "0";
    singlyNode* next;
};

struct doublyNode {
    int commitNumber;
    singlyNode* head;
    doublyNode* previous;
    doublyNode* next;
};

class Git {
    private:
        singlyNode* head;
        doublyNode* comHead;
    public:
        Git();
        ~Git();
        void init();
        void add();
        void remove();
        int commit();
        void checkout();
};

#endif