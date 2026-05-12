#include <iostream>
using namespace std;

int main() {
    int choice;
    cout << "Enter 1 claculate mark 2 - time convert ( 1 / 2 )  ";
    cin >> choice;

    if (choice == 1) {
        cout << "Welcome  User!" << endl;
        float oneSubject;
        cout << " Enter your one stubect mark " ;
        cin >> oneSubject;
        cout << " Enter Mark 1: ";
        float mark_1 ;
            cin >> mark_1;
        float mark_2;
            cout << " Enter Mark 2: " ;
            cin >> mark_2;
        float mark_3;
            cout << " Enter mark 3: ";
            cin >> mark_3;
        float mark_4;
            cout << " Enter Mark 4: " ;
            cin >> mark_4;
        float mark_5;
            cout << " Enter mark 5: " ;
            cin >> mark_5;
        float total = mark_1 + mark_2 + mark_3 + mark_4 + mark_5;
            cout << " Your Mark is " << total << " Percantage " << (total / (oneSubject * 5)) * 100 << "%";
    } else if (choice == 2) {
        int totalSeconds;
        char repeat = 'y';
        while( repeat == 'y' || repeat == 'Y' ){
        cout << "\nEnter total seconds: ";
        if( cin >> totalSeconds){
        int hours = totalSeconds / 3600;
        int minutes = (totalSeconds % 3600) / 60;
        int seconds = totalSeconds % 60;

        cout << "Converted Time -> " << hours << "h : " << minutes << "m : " << seconds << "s" << endl;
        cout << "\n Repeat This Thing? (y/n): ";
        cin >> repeat;
    
        cout << "Returning to main menu..." << endl;
       } else{
            cout << " Only Number ";
            break;
       }
       }
     } else {
        cout << "Invalid choice." << endl;
      }

        return 0;
}

