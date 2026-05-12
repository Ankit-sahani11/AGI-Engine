#include<iostream>
#include<string>
#include<unordered_map>
using namespace std;

class User{
    string* name;
    int* pin;	
    int* mini_bal;
    public :
    User(){
    name = nullptr; 
    pin = nullptr;
    mini_bal = nullptr;
}
    public:
    User(string n ,int p, int b ){
     name = new string(n);
     pin =new int(p);
     mini_bal = new int(b);
    }
    int getpin(){
    return *pin;
    }
    ~User(){
       delete name;
       delete pin;
       delete mini_bal;
     }
    };
class myAccount{
    User CurrentUser;
    unordered_map < string , User* > Database;
    myAccount(){}
    public:
    void create_account(){
    cout<<"creste account"<<endl;
    string name;
    cout << "Set name: ";
    cin>>name;
    cin.ignore(); 
    if(Database.count(name)){
        cout<<name<<"Are Presant"<<endl;
        return;
    }else{
        cout<<"Set pin: ";
        int pin;
        cin>>pin;
        cin.ignore();
        if( pin >= 1000 && pin <= 9999){
        int fees;
        cout<<"Pay 500rs fees: ";
        cin>>fees;
        cin.ignore();
        Database [name] = new User(name,pin,fees);
        cout <<"Account Created Sucessfull! "<<endl; 
       }else{
         cout<<" Only 4 digit pin "<<endl;
       }
    }
}
    public:
    void login(){
     do{
     string* name = new string; 
        cout << "Enter name: ";
        if(getline(cin, *name )){
        cin>> *name;
            cin.ignore();
            if(Database.count(*name)){
               int* pin = new int;
               cout<<"Enter pin: ";
               cin >> *pin;
               cin.ignore();
               User u;
                if(u != nullptr && u.getpin() == *pin){
                     cout<< "Login sucessfull "<<endl;
                     CurrentUser = u;
                     delete pin;
                     break;
               }else{
                    cout<<"Wrong pin ";
                    int i = 0;
                    while(i <= 3){
                    // Three wrong pin logic
                  }
               }
               }else{
                    cout<<"Acco&nt not found "<<endl;
                    continue;
               }
        }else{
            cout<<"Only String "<<endl;
            continue;
            }
      delete name ;
     }while(true);
 }
     public:
     void DeepCopy(string name,int pin ,int bal){
    }
}; 
int main(){

    return 0;
}
