#include<iostream>
using namespace std;

int main(){
    int rose = 3;
    int colume = 2;
    int matxics[rose][colume] =  { {1, 2, 3}, {4, 5, 6} }; 
    int rose_2 = 2;
    int col_2 = 3;
    int matxics_1[rose_2][col_2] = { { 1,2,3}, {4,5,6}};
    int matxics_2[rose][colume] = { { 0,0,0},{0,0,0}};
     for(int i = 0;i<rose;i++){
     for(int j = 0;j<colume;j++){
     for(int k = 0; k<rose;k++){
         matxics_2[i][j] += matxics[k][j] * matxics_1[k][j]; 
    }
    cout<<","<<endl;
  }
    cout<<","<<endl;
}
    cout<<"[ " << matxics_2 << "]"<<endl;

    return 0;
}
