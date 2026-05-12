#include<iostream>
using namespace std;

class Arraylist{
void** arr;
char* typeArr;
int accIndex;
int capcity;
int size;
void* temptr;
 public:
   ~Arraylist(){
        for(int i = 0;i<size;i++){
           delete arr[i];
        }
           delete typeArr;
           delete arr[capcity];
   } void freeMemory(void* ptr, char type) {
    if (ptr == nullptr) return; // Agar pointer pehle se khali hai toh kuch mat karo

    switch(type) {
        case 'i': 
            delete (int*)ptr;    // Pointer ko int* mein badal kar delete karo
            break;
        case 'f': 
            delete (float*)ptr;  // Pointer ko float* mein badal kar delete karo
            break;
        case 'c': 
            delete (char*)ptr;   
            break;
        case 'b': 
            delete (bool*)ptr;   
            break;
        case 's': 
            delete (string*)ptr; // String ke liye ye bohot zaroori hai
            break;
    }
}

    public:
       void print(void * ptr, char type){
           switch(type){
           case 'i': cout << *((int*)ptr); break; // cout zaroori hai
           case 'f': cout << *((float*)ptr); break;
           case 'c': cout << *((char*)ptr); break;
           case 'b': cout << (*((bool*)ptr) ? "true" : "false"); break;
           default : cout << "N/A";
           }
       }Arraylist(){
          size = 0;
          capcity = 1;
          typeArr = new char[capcity];
          arr = new void *[capcity]; // I Have no logic and idea this type of arr[Something] 
       }void UserChoice(char type) { // Yahan ptr ki zaroorat nahi hai
                 cout << "String: s, int: i, float: f, char: c, bool: b" << endl;
    
    switch(type) {
        case 'i': { // Bracket shuru (Naya scope)
            int *p = new int;
            cin >> *p;
            temptr = p; // Ab ye 'p' sirf is case ke liye hai
            break;
        } // Bracket khatam
        
        case 'f': {
            float *p_2 = new float;
            cin >> *p_2;
            temptr = p_2;
            break;
        }
        case'c':{
             char *p_3 = new char;
             cin >> *p_3;
             temptr = p_3;
             break;
        }
        case 's': {
            string *p_4 = new string;
            cin >> *p_4;
            temptr = p_4;
            break;
        }
        case 'b' :{
            bool* p_5 = new bool;
            cin >> *p_5;
            temptr = p_5;
            }      
       }
    
       }void add(void*inputptr, char type){
          if(size == capcity){resize();}
           typeArr[size] = type;
          *(arr + size) = inputptr;
           size++;
       }void resize(){
    int newcapcity = capcity * 2;
    void** NewArr = new void*[newcapcity];
    char* NewTypeArr = new char[newcapcity]; // Nayi type array
    
    for(int i = 0; i < size; i++){
        NewArr[i] = arr[i];
        NewTypeArr[i] = typeArr[i]; // Type bhi copy karo
    }
       delete[] arr;
       delete[] typeArr; // Purana saaf karo
       arr = NewArr;
       typeArr = NewTypeArr;
       capcity = newcapcity;     
   }void executeShifting(int index) {
    // Ab ye function call kaam karega kyunki upar humne ise define kar diya hai
    freeMemory(arr[index], typeArr[index]); 

    // Baki shifting wala logic same rahega
    for (int i = index; i < size - 1; i++) {
    
    arr[i] = arr[i + 1];
        typeArr[i] = typeArr[i + 1];
    }
    size--;
       }public:
    // Integer ke liye
    void remove(int target) {
        for (int i = 0; i < size; i++) {
            if (typeArr[i] == 'i' && *(int*)arr[i] == target) {
                executeShifting(i);
                return;
            }
        }
        cout << "Bhai, ye Integer array mein nahi hai!" << endl;
    }

    // Float ke liye
    void remove(float target) {
        for (int i = 0; i < size; i++) {
            if (typeArr[i] == 'f' && *(float*)arr[i] == target) {
                executeShifting(i);
                return;
            }
        }
        cout << "Bhai, ye Float array mein nahi hai!" << endl;
    }

    // Char ke liye
    void remove(char target) {
        for (int i = 0; i < size; i++) {
            if (typeArr[i] == 'c' && *(char*)arr[i] == target) {
                executeShifting(i);
                return;
            }
        }
        cout << "Bhai, ye Character array mein nahi hai!" << endl;
    }

    // String ke liye
    void remove(string target) {
        for (int i = 0; i < size; i++) {
            if (typeArr[i] == 's' && *(string*)arr[i] == target) {
                executeShifting(i);
                return;
            }
        }
        cout << "Bhai, ye String array mein nahi hai!" << endl;
    }

    // Bool ke liye
    void remove(bool target) {
        for (int i = 0; i < size; i++) {
            if (typeArr[i] == 'b' && *(bool*)arr[i] == target) {
                executeShifting(i);
                return;
            }
        }
        cout << "Bhai, ye Boolean array mein nahi hai!" << endl;
    }

       void print(){
    for(int i = 0; i < size; i++){
        print(arr[i], typeArr[i]); // Ye upar wale switch wale print ko call karega
        cout << ", ";
    }
    cout << endl;
}

       int getIndex(void* num, char type) { // Type bhi pass karo
    for(int i = 0; i < size; i++) {
        if(typeArr[i] == type) { // Pehle type match karo
            if(type == 'i') {
                if(*(int*)arr[i] == *(int*)num) return i; // Value compare hui!
            }
            if(type == 'f') {
                if(*(float*)arr[i] == *(float*)num) return i;
            }
            // ... baaki types ke liye bhi
        }
    }
    return -1;
}


};

int main(){
    Arraylist* a = new Arraylist();
    int choice;

    while(true) {
        cout << "\n--- Atraylist  Menu ---" << endl;
        cout << "1. Initialize Arraylist (Create Arraylist)" << endl;
        cout << "2. Add Buffer (Enter Data)" << endl;
        cout << "3. Resize (Dynamaic)" << endl;
        cout << "4. print All (print)" << endl;
        cout << "6. Exit" << endl;
        cout << "Choose Option: ";
        cin >> choice;

        if (choice == 6) break;

        switch(choice) {
            case 1:  break;
            case 2: { // Bracket lagao
                char t;
                cout << "Enter type (i/f/c/b/s): ";
                cin >> t;
                a->UserChoice(t); // Ye temptr mein value bhar dega
                a->add(a->temptr,i); // Phir address aur type dono add honge
                break;
              }
            case 3: (*a).resize(); break;
            case 4: (*a).print(); break;
            default: cout << "Invalid choice, Bhai!" << endl;
              
    }
    }     
         delete a;
    return 0;
}
