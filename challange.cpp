#include<iostream>

using namespace std;

class Buffer{
    int input;
    int NewSize;
    int* buffer;
    int* newbuffer;
    int feel;
    int add = 0;
    int buffer_feel;
public:
    Buffer(){}
     ~Buffer(){
         cleanUp();
      }
    public:  
    void cleanUp() {
        if (buffer != nullptr) {
            delete[] buffer;
            buffer = nullptr;
        }
   }
      void initbuffer(){
          cout<<"Enter Size of buffer: "; 
          cin>>input;
          buffer = new int[input];
          int* ptr = buffer;  
      }
    public:
      void initfill(){
         cout<<"Enter feeling num "; 
         cin>>feel;
         for(int i = 0; i<input;i++){
             cin>> *(buffer + i);
             cout<< "Printed: "<<*(buffer + i )<<endl;
         }
     }
     public:
       void calculate_sum(){
         cout<<"Enter Num abb ";
         for(int i = 0;i<input;i++){
              add = *(buffer + i) + add;
              cout<< "added: "<<add<<endl;
         } 
     }
      public:
        void add_num(){
           int num;
           cout<<"Add Number";
           cin>>num;
           for(int i = 0;i<input;i++){ 
               *(buffer + i) += num;          
                cout<<"added: "<<*(buffer + i) <<endl;
           }
      }
      public:
        void resize(){
           cout<<"Enter New Size ";
           cin>>NewSize;
           newbuffer = new int[NewSize];
           for(int i =0;i<input;i++){
              *(newbuffer + i) = *(buffer + i); 
           } 
           delete[] buffer;
           buffer = newbuffer;
           input = NewSize;         
        }

};
int main() {
    Buffer b;
    int choice;

    while(true) {
        cout << "\n--- 4GB RAM Optimizer Menu ---" << endl;
        cout << "1. Initialize Buffer (Create Memory)" << endl;
        cout << "2. Fill Buffer (Enter Data)" << endl;
        cout << "3. Calculate Sum (Logic Test)" << endl;
        cout << "4. Add Number to All (Pointer Math)" << endl;
        cout << "5. Resize Buffer (Memory Management)" << endl;
        cout << "6. Exit" << endl;
        cout << "Choose Option: ";
        cin >> choice;

        if (choice == 6) break;

        switch(choice) {
            case 1: b.initbuffer(); break;
            case 2: b.initfill(); break;
            case 3: b.calculate_sum(); break;
            case 4: b.add_num(); break;
            case 5: b.resize(); break;
            default: cout << "Invalid choice, Bhai!" << endl;
        }
    }
    return 0;
}

