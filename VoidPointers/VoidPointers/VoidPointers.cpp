//Inside a void pointer you can assign the address of any data type.

#include <iostream>
using namespace std;

void printNumber(int* numberPtr) {
    cout << *numberPtr << endl;
};

void printLetter(char* charPtr) {
    cout << *charPtr << endl;
};

void printLetter(float* floatPtr) {
    cout << *floatPtr << endl;
};


void printValue(void* ptr, char type)
{
    switch (type)
    {
    case 'i':
        cout << "Integer value: " << *(int*)ptr << endl;
        break;
    case 'f':
        cout << "Float value: " << *(float*)ptr << endl;
        break;
    case 'c':
        cout << "Character value: " << *(char*)ptr << endl;
        break;
    default:
        cout << "Unknown type!" << endl;
    }
}
int main()
{
	int number = 42;
	char letter = 'A';
	float pi = 3.14f;

    printValue(&number, 'i');
    printValue(&pi, 'f');
    printValue(&letter, 'c');
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
