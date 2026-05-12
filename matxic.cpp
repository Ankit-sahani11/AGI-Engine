
#include<cstdlib>
#include <cstdio>

struct Matxics{
int rose = 3;
int **Mat_1;
int **Mat_2;
int **Reasult;
int colume = 3; 
    Matxics(){
        Memoryallocate();
    }
    void Memoryallocate(){
       Mat_1 = (int**)malloc(rose * sizeof(int*));// Inilaizing a Void pointer Array of Pointer
       Mat_2 = (int**)malloc(rose*sizeof(int*));
       Reasult = (int**)malloc(rose*sizeof(int*));
       for(int i = 0;i<rose;i++){
          Mat_1[i] = (int*)malloc(colume* sizeof(int));
          Mat_2[i] = (int*)malloc(colume*sizeof(int));
          Reasult[i] =(int*)malloc(colume*sizeof(int));
          for(int j = 0; j < colume; j++) {
                Reasult[i][j] = 0;
    }
    }
    }void multiplication(){
         for(int i =0;i<rose;i++){
         for(int j = 0;j<colume;j++){
         for(int k = 0;k<colume;k++){
             *(*(Reasult + i)+j) += *(*(Mat_1 + i)+k) *  *(*(Mat_2 + k)+j);
         }
         }
         }
    } void display(){
         for(int i =0;i<rose;i++){
              printf("%s","[");                 
         for(int j = 0;j<colume;j++){
              printf("%d ", Reasult[i][j]);
         }
            
         } 
              printf("%s" ,"]");
    }~Matxics(){
         for(int i =0;i<rose;i++){
              free(Mat_1[i]);
              free(Mat_2[i]);
              free(Reasult[i]);
        }
              free(Mat_1);
              free(Mat_2);
              free(Reasult);
     }
};

int main(){
    Matxics m;
    m.multiplication();
    m.display();
    return 0;
}
