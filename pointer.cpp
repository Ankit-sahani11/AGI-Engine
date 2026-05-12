#include<iostream>
using namespace std;

int main(){
    int a = 7;
    int* b = &a;
    cout<<"Address of a = adress of b Address of a"<<&a<<endl<<"Address of b = "<<b;
    cout<<"Values of b = " <<*b<<endl;
    int ** c = &b;
    cout<<"pointer of pointer " <<c<<endl;
    return 0;
}
