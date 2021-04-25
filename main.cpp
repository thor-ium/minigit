#include <iostream>
#include "linkedGit.hpp"

using namespace std;


#include <filesystem>
namespace fs = std::filesystem;
void menu() {
    cout << "+=========Main Menu=========+" << endl;
    cout << " 1. init" << endl;
    cout << " 2. add" << endl;
    cout << " 3. remove" << endl;
    cout << " 4. commit" << endl;
    cout << " 5. checkout" << endl;
    cout << " 6. exit" << endl;
    cout <<"+===========================+" << endl;
}

int main() {
    Git G;
    int choice = 0;
    while (choice != 6) {
        menu();
        cin >> choice;
        while( ((choice > 6) || (choice < 1)) || cin.fail() )    
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout<< "Invalid option, input must be a number between 1 and 3, please enter a valid option"<< endl;
            cin>> choice;
        }
        switch (choice)
        {
        case 1:
            G.init();
            break;
        case 2:
            if (!fs::exists(".minigit")) {
                cout << "minigit has not been initialized." << endl;
                break;
            }
            G.add();
            break;
        case 3:
            if (!fs::exists(".minigit")) {
                cout << "minigit has not been initialized." << endl;
                break;
            }
            G.remove();
            break;
        case 4:
            if (!fs::exists(".minigit")) {
                cout << "minigit has not been initialized." << endl;
                break;
            }
            G.commit(); 
            break;
        case 5:
            if (!fs::exists(".minigit")) {
                cout << "minigit has not been initialized." << endl;
                break;
            }
            G.checkout();
            break;

        }
    }

    cout << "Goodbye!" << endl;
}