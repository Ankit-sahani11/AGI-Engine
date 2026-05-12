#include<iostream>
using namespace std;

int main() {
    int a = 7;
    int* b = &a;    // b stores address of a
    int** c = &b;   // c stores address of b
    int*** d = &c;  // d stores address of c

    cout << "Value of a: " << a << endl;
    cout << "Address of a (Value of b): " << b << endl;
    cout << "Value at address b (*b): " << *b << endl; // Yeh 7 print karega

    cout << "Address of b (Value of c): " << c << endl;
    cout << "Value at address c (*c): " << *c << endl; // Yeh a ka address print karega
    cout << "Double pointer value (**c): " << **c << endl; // Yeh bhi 7 print karega

    cout << "Address of c (Value of d): " << d << endl;
    cout << "Triple pointer value (***d): " << ***d << endl; // Yeh bhi 7 print karega

    return 0;
}
