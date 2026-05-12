#include<iostream>
using namespace std;

int main(){
    string name = ["Ankit","Ramu","Jeff_dean"];
    int [] pin = [1111,2222,3333];
    int [] bal = [1000,2000,3000];
    cout<<"Welcome atm "<<endl;
    cout<<"Enter name";
    string name;
    int accIndex = -1;
    int length = size(name);
    for(int i = 0;i<length;i++){
       if(name[i] == name){
           accIndex = i;
           cout<<"Enter pin ";
           int pin;
           cin>>pin;
           if(pin[accIndex] == pin){
               cout<<"Login Sucessfull !"<<endl;
           }else{
               cout<<"Wrong pin"<<endl;
       }else{
        cout<<"Account Not fo&nd ! "<<endl;
       }
    }
    return 0;
}
