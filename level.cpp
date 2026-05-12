#include<iostream>
#include<random>
using namespace std;

int main(){
    int Score;
    bool play = true;
    cout<<"Welcome Gusses the number game"<<endl;
          random_device rd;  // Obtain a seed from hardware    
          mt19937 gen(rd()); // Initialize generator (Mersenne Twister)    
          uniform_int_distribution<> distrib(1, 100); // Define range [1, 100]     
          int Computer_choice =  distrib(gen);
         while(play){
          cout<<"Gusses Computeer Choice num ";
          int player;
          cin >> player;
          if(Computer_choice == player ){
              cout<<"You Win"<<endl;
              cout<<"replay (Y/N) ";
              string replay;
              cin>>replay;
              if(replay == "y" || replay == "Y"){
                  cout<<"Restarting "<<endl;
                  random_device rd;  // Obtain a seed from hardware
                  mt19937 gen(rd()); // Initialize generator (Mersenne Twister)
                  uniform_int_distribution<> distrib(1, 100); // Define range [1, 100]
                  Computer_choice =  distrib(gen);
                  continue;
              }else if( replay == "N" || replay == "n"){
                  break;
              }else{
                  cout<<" Only (Y/N) "<<endl;
                  cout<<" Good bay";
                  play = false;
                }
          }else if(player > Computer_choice){
              cout<<"A One little number"<<endl;
              continue;
          }else{
              cout<<"A One big number"<<endl;
              continue;
          }
    }
    return 0;
}
