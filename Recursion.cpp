//Recursion
#include <iostream>
using namespace std;



int sumIntegers(int value){
    if (value == 0)
    return 0;
    

    value--;
    std::cout << "Value = " << value << std::endl;
    return value += sumIntegers(value);
}

 int main()
 {
    
   /*for(int i =0;i <= 10; i++){

       finish += start;
   }*/
   
   std::cout << "Result = " << sumIntegers(10) << std::endl;

    system("pause>0");
}

