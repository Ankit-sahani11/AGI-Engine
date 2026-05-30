#include<cstdlib> 
#include <cstdio>
const int My_Int = 0;
const int My_Bool = 1;
const int My_Char = 2;
const int My_Float = 3;
const int My_Double = 4;
const int My_Long_Double = 5;

struct ANY{// badal kar dod base banana hai 
    int Capicaty;
    int *type;
    int size;
    void** list;
    static int index;
    int* Int_type;
    int tempSize;
     int tempCap;
    ~ANY(){
        freeMemory(list,type);
        free(list);
        free(type);
    }
    ANY(){
        size = 0;
        Capicaty = 5;
        list = (void**)malloc(Capicaty * sizeof(void*));
        type = (int*)malloc(Capicaty* sizeof(int));
        tempSize = 0;
        tempCap = 100;
    }
    void resize(){
        int NewCapicaty = Capicaty*2;
        void** NewList =(void**)malloc(NewCapicaty* sizeof(void*));
        int * NewType = (int*)malloc(NewCapicaty* sizeof(int));
        for(int i = 0; i < size; i++){
            NewList[i] = list[i];
            NewType[i] = type[i];
        }
        free(list);
        free(type);
        list = NewList;
        type = NewType;
        Capicaty = NewCapicaty;
    }// badal kar deep copy karnahai 
    int add(int num){
       if (size == Capicaty) { resize(); }
       int* temp = new int(num);
       *(list + size) = temp;
       *(type + size) = My_Int;
        size++;
       return num;
    }
    float add(float num){
       if (size == Capicaty) { resize(); }
       float* temp = new float(num);
       *(list + size) = temp;
       *(type + size) = My_Float;
        size++;
        return num;
    }
    long double add(long double  num){
       if (size == Capicaty) { resize(); }
       long double* temp = new long double(num);
       *(list + size) = temp;
       *(type + size) = My_Long_Double;
        size++;
        return num;
    }
    double add(double num){
       if (size == Capicaty) { resize(); }
       double* temp = new double(num);
       *(list + size) = temp;
       *(type + size) = My_Double;
        size++;
        return num;
    }
    bool add(bool num){
       if (size == Capicaty) { resize(); }
       bool* temp= new bool(num);
       *(list + size) = temp;
       *(type + size) = My_Bool;
        size++;
       return num;
    }
    char add(char num){
       if (size == Capicaty) { resize(); }
       char* temp = new char(num);
       *(list + size) = temp;
       *(type + size) = My_Char;
        size++;
        return num;
    }
    void freeMemory(void ** list,int * type){
       for(int i = 0; i < size;i++){
         if (type[i] == My_Char)  delete ((char*)list[i]);
         if (type[i] == My_Int)  delete ((int*)list[i]);
         if (type[i] == My_Bool)  delete ((bool*)list[i]); 
         if (type[i] == My_Float)  delete ((float*)list[i]);
         if (type[i] == My_Double)  delete ((double*)list[i]);
         if (type[i] == My_Long_Double)  delete ((long double*)list[i]);
       }
    }
    void print() {
        for (int i = 0; i < size; i++) {
            switch (type[i]) {
                case My_Int:
                    printf("%d\n", *((int*)list[i]));
                    break;
                case My_Float:
                    printf("%f\n", *((float*)list[i]));
                    break;
                case My_Bool:
                    printf("%d\n", *(bool*)list[i]);
                    break;
                case My_Long_Double:
                    printf("%Lf\n",*(long double*)list[i]);
                    break;
                case My_Double:
                    printf("%lf\n",*(double*)list[i]);
                    break;
                case My_Char: 
                    printf("%c\n",*(char*)list[i]);
            }
        }
    }
    int getIndex(int num){
        int tempCap = 100;
        Int_type = new int[tempCap];
        for(int i = 0; i< size;i++){
           if(*((int*)list[i]) == My_Int){
               int Int_valus = *((int*)list[i]);
               Int_type[tempSize] = Int_valus;
               tempSize++; 
           }
        }
        sort(Int_type);
        int index = binarySearch(Int_type,num);
        return index;
    }
    void remove(int target){
       int tempIndex = getIndex(target);
       shift(tempIndex);
    }
    private :
       void shift(int index){
          for(int p = index; p < size-1; p++){
                list[p] = list[p + 1];
                type[p] = type[p +1];
           }
           size--;
        }
    private :
    void sort(int arr[]){
        for(int i = 0; i < tempSize - 1; i++ ) {
        for(int j = 0;j < tempSize - i - 1;j++){
           if(arr[j] > arr[j + 1]){
               int temp = arr[j];     // 1. Pehle element ko safe jagah rakh diya
               arr[j] = arr[j+1];     // 2. Dusre element ko pehle ki jagah daal diya
               arr[j+1] = temp;       // 3. Safe rakha hua element dusre ki jagah daal diya
           }
        }
        }
    }


/*
Inner Loop Condition (j < size - i - 1) Kitni baar chalega?
i = 0 (1st Pass) j < 5 - 0 - 1 \implies j < 4 j chalega 0, 1, 2, 3 (Sabse bada element aakhiri mein pahunch jayega)
i = 1 (2nd Pass) j < 5 - 1 - 1 \implies j < 3 j chalega 0, 1, 2 (Aakhiri element fix ho chuka hai, use chhod kar baaki check honge)
i = 2 (3rd Pass) j < 5 - 2 - 1 \implies j < 2 j chalega 0, 1
i = 3 (4th Pass) j < 5 - 3 - 1 \implies j < 1 j chalega 0
*/

    private :
      int binarySearch(int arr[],int element){
          int low = 0;
          int high = tempSize - 1;
          int mid;
          while(low <= high){
              mid = (high + low) / 2;
              if(arr[mid] == element){
                 return mid;
              }else if(arr[mid] > element){
                 low = mid + 1;
              }else{
                 high = mid - 1;
              }
          }
          return -1;
      }
};
int main(){
    ANY myVector;
    int a = myVector.add(6);
    int b = myVector.add('k');
    printf("%c",myVector.print());
    int aa = myVector.getIndex(6);
    printf("%d" , aa);    
    return 0;
}
