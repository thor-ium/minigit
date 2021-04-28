Thor Breece
CSCI2270 - Section 104
Professor: Ashutosh Trivedi
TA: Himanshu Gupta
4/27/21

Minigit Final Project

    This project mimicks the website github but does so locally and without using the local git commands. It saves fileversions
    in the minigit repository. If a file in the main directory is edited then committed via this program, it will edit the name of
    the file version and save it as a new file in the minigit repository.

    Format of fileversions: "file.filversion.filetype"
    Ex: "test.0.txt"
    
    header file: linkedGit.hpp
        structs:
                singlyNode {
                    string fileName;
                    string fileVersion;
                    singlyNode* next;
                };
                doublyNode {
                    int commitNumber;
                    singlyNode* head;
                    doublyNode* previous;
                    doublyNode* next;
                }
        Git class member functions:
            Git();
            ~Git();
            bool init();
            void add();
            void remove();
            void commit();
            void checkout();
    implimentation file: linkedMiniGit.cpp
        member functions:
            Git:
                constructor function. resets .minigit directory and recreates it
            ~Git:
                deconstructor function. deletes .minigit directory
            init: 
                init function creates ".minigit" repository 
                creates first commit node
                returns true if ran
            add:
                adds file to current file linked list (SLL) that will then be set
                as data member of DLL in commit() Function
                Ex: 
                    before: test.0.txt -> NULL
                    after: test.0.txt->test2.0.txt -> NULL
            remove:
                removes a file from the current SLL before commit function is executed
                Ex:
                    before: test.0.txt -> test2.0.txt -> NULL
                    after: test.2.0.txt -> NULL
            commit:
                the commit function takes the current SLL and checks if
                it has been committed in it's current version. If it hasn't
                it will copy the file with the name of the fileVersion to the ".minigit" repository
                Once file is stored in the .minigit repository, a new DLL node is created and data is copied
                for use the next time the add function and commit functions are called
                Ex:
                    user/main_directory:
                        test.txt
                    *commit function executed*
                    user/.minigit:
                        test.0.txt
            checkout: 
                the checkout function takes a specified commit number and reverts the main_directory
                versions of the files to the versions stored in the .minigit repository. 
                Ex. 
                    test.txt contents: "Hello World!"
                    test.0.txt contents: "Hello CSCI2270!"
                    *checkout function executed*
                    test.txt contents: "Hello CSCI2270!"
                    test.0.txt contents: "Hello CSCI2270!"
        helper functions:
            split:
                function used to split filenames and fileversions
            debug_printLL:
                function used to check if SLL is properly changed / created
            hashAddress:
                function adds all ASCII values for each character in each line of a given file
                returns value % table_size
                function used to check if files have been edited
            getCurrCommit:
                function to find current commit 
                used to traverse current commit in commit function
            insertLast:
                function that inserts next commit node at the end of the DLL
            copy:
                copies file into .minigit directory
            getFileVersion:
                function to properly assign file version. Used in add() function
            existsNowhere:
                function used in commit function to check if a fiel has ever been inserted before
            copyLL:
                recursively copies current SLL to the next commit while checking if the files have been changed
            deleteRedundancies:
                checks new linekd list in next commit for doubly added nodes
            copyCheck:
                reverse of copy function. Copies a fileVersion from .minigit directory 
                to the corresponding file in the main directory