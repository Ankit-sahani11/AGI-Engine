#include<string>
#include<iostream>
using namespace std;

class MyVector{
    int capcity;
    int size;
    void** list;
    char* typeArray;
    private:
        void shift(int index){
            for(int p = index; p < size-1; p++){
                list[p] = list[p + 1];
                typeArray[p] = typeArray[p +1];
       }
        size--;
    }
    public:
       MyVector(){
          size = 0;
          capcity = 5;
          list = new void*[capcity];
          typeArray = new char[capcity];          
      }~MyVector(){
          for(int i = 0; i< size; i++){
              freeMomory(list[i],typeArray[i]);
              }
              delete [] list;
              delete [] typeArray;
      }void add(int values){
           if(capcity == size){resize();}
               int *p = new int(values); 
              *(typeArray + size) = 'i';
               *(list + size) = p;
                size++;
       }void add(float values){
          if(capcity == size){resize();}
           float *temp = new float(values);
           *(list + size) = temp;
           *(typeArray + size) = 'f';
           size++;
       }void add(char values){
           if(capcity == size){resize();}
               char*p = new char(values);
              *(typeArray + size) = 'c';
               *(list + size) = p;
                size++;
       }void add(string values){
           if(capcity == size){resize();}
           string *temp = new string(values);
           *(list + size) = temp;
           *(typeArray + size) = 's';
           size++;
      }void add(const char* values){
           add(string(values)); 
       }void add(bool values){
           if(capcity == size){resize();}
           bool *temp = new bool(values);
           *(list + size) = temp;
           *(typeArray + size) = 'b';
           size++;
        }void add(long double values){
           if(capcity == size){resize();}
           long double *temp = new long double(values);
           *(list + size) = temp;
           *(typeArray + size) = 'b';
           size++;
        }void add(double values){
           if(capcity == size){resize();}
           double *temp = new double(values);
           *(list + size) = temp;
           *(typeArray + size) = 'b';
           size++;
}
       void resize(){
           int NewCapcity = 2 * capcity;
           void** newList = new void*[NewCapcity];
           char*newtpye = new char[NewCapcity];
           for(int i = 0; i <size; i++){
               newList[i] = list[i]; 
               newtpye[i] = typeArray[i];
           } delete [] list;
             delete [] typeArray;
             list = newList;
             typeArray = newtpye;  
             capcity = NewCapcity;

         }void remove(int target){
             for(int i = 0; i<size;i++){
                    if (typeArray[i] == 'i' && *((int*)list[i]) == target) {
                      shift(i); 
                      return ;  
                }
             }
}void remove(float target) {
    int idx = getIndex(target);
    if (idx != -1) {
        delete (float*)list[idx];
        shift(idx);
    }
}

void remove(double target) {
    int idx = getIndex(target);
    if (idx != -1) {
        delete (double*)list[idx];
        shift(idx);
    }
}

void remove(long double target) {
    int idx = getIndex(target);
    if (idx != -1) {
        delete (long double*)list[idx];
        shift(idx);
    }
}

void remove(string target) {
    int idx = getIndex(target);
    if (idx != -1) {
        delete (string*)list[idx];
        shift(idx);
    }
}
         } void print(void * ptr, char type){
               switch(type){
               case 'i': cout << *((int*)ptr); break; 
               case 'f': cout << *((float*)ptr); break;
               case 'c': cout << *((char*)ptr); break;
               case 'd': cout << *((double*)ptr); break;
               case 'L': cout << *((long double*)ptr); break;                                                                        
               case's':cout<<*((string*)ptr);break;
               case 'b': cout << (*((bool*)ptr) ? "true" : "false"); break;
               default : cout << "N/A";
               }
          }void freeMomory(void *ptr,char type){
              switch(type){
                  case'i':delete((int*)ptr);break;
                  case'f':delete((float*)ptr);break;
                  case's':delete((string*)ptr);break;
                  case'b':delete((bool*)ptr);break;
                  case'c':delete((char*)ptr);break;
                  case'L':delete((long double*)ptr);break;
                  case'd':delete((double*)ptr);break;                  
                  }
          }int getIndex(int thing){
              int accIndex = -1;
              for(int i =0; i < size;i++){
                  if(typeArray[i] == 'i' && *((int*)list[i]) == thing){
                      accIndex = i; 
          }
       }
             cout<<"not exits ! ";
             return -1;

}         
int getIndex(float thing) {
    for (int i = 0; i < size; i++) {
        if (typeArray[i] == 'f' && *((float*)list[i]) == thing) return i;
    }cout<<"not exits ! ";
    return -1;
}

int getIndex(double thing) {
    for (int i = 0; i < size; i++) {
        if (typeArray[i] == 'd' && *((double*)list[i]) == thing) return i;
    }cout<<"not exits ! ";
    return -1;
}

int getIndex(long double thing) {
    for (int i = 0; i < size; i++) {
        if (typeArray[i] == 'L' && *((long double*)list[i]) == thing) return i;
    }cout<<"not exits ! ";
    return -1;
}

int getIndex(string thing) {
    for (int i = 0; i < size; i++) {
        if (typeArray[i] == 's' && *((string*)list[i]) == thing) return i;
    }cout<<"not exits ! ";
    return -1;
}
 
void printAll(){
    for(int i = 0; i < size; i++ ){
       print(list[i],typeArray[i]);
       if(i<size -1){
           cout<<" , ";
       }
     }
            cout<<endl;
   }

};
class User{
     string* name;
     int* pin;
     int mini_bal;
     User(){}
     User(string name ,int pin,int bal){
         
     }
};
struct Test_Vector{
    MyVector Name;
    MyVector Pin;
    MyVector balance;
    Test_Vector(){}
    void create_Acoount(){
        cout<<"Welcoem "<<endl;
        cout<<"Enter name : ";
        string name;
        cin>>name;
        Name.add(name);
        cout<<"Enter pin :";
        int pin;
        cin>>pin;
        Pin.add(pin);
}        
};

int main(){
    return 0;
}






