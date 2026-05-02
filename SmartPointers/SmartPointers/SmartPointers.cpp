// SmartPointers.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <memory>

using namespace std;

class Beamship {

    public:
    Beamship() {
        cout << "Beamship created" << endl;
    }
    ~Beamship() {
        cout << "Beamship destroyed" << endl;
	}

};

int main()
{
	// Unique pointers cannot be shared between multiple owners. 
	// They are designed to have sole ownership of the object they point to.
	// When the unique pointer goes out of scope, it automatically deletes the object it owns. 
	// This ensures that there are no memory leaks and that the object is properly cleaned up 
	// when it is no longer needed.

	/*unique_ptr<int> ptr1 = make_unique<int>(78557);// Sierpinski numbers
	cout << "Memory address: " << ptr1.get() << endl;
	cout << "Value: " << *ptr1 << endl;

	unique_ptr<int> ptr2 = move(ptr1); // Transfer ownership from ptr1 to ptr2
	cout << "After move:" << endl;
	cout << "ptr1: " << (ptr1 ? "Exists" : "Null") << endl;
	cout << "ptr2: " << (ptr2 ? "Exists" : "Null") << endl;*/



	//Unique pointer will automatically delete the object it owns when it goes out of scope.
	//there is no need to manually delete the object, which helps prevent memory leaks and ensures proper resource management.

	/*unique_ptr<Beamship> beamship1 = make_unique<Beamship>();
	unique_ptr<Beamship> beamship2 = move(beamship1);
	cout << "Beamship 1: " << (beamship1 ? "Exists" : "Null") << endl;
    cout << "Beamship 2: " << (beamship2 ? "Exists" : "Null") << endl;*/

	/*shared_ptr<Beamship> beamship3 = make_shared<Beamship>();
	cout << "Beamship 3 Number of pilots...: " << beamship3.use_count() << endl; // Output: 1

	shared_ptr<Beamship> beamship4 = beamship3; // Shared ownership
	cout << "Beamship 4 Number of pilots...: " << beamship4.use_count() << endl; // Output: 2*/


	{
		shared_ptr<Beamship> beamship3 = make_shared<Beamship>();
		cout << "Beamship 3 Number of pilots...: " << beamship3.use_count() << endl; // Output: 1
		{
			//Shared pointer is destroyed when it goes out of scope.
			shared_ptr<Beamship> beamship4 = beamship3; // Shared ownership
			cout << "Beamship 4 Number of pilots...: " << beamship4.use_count() << endl; // Output: 2
		}
		// After beamship4 goes out of scope, the reference count decreases by one.
		cout << "Beamship 3 Number of pilots...: " << beamship3.use_count() << endl; // Output: 1
	}

	// After beamship3 goes out of scope, the reference count becomes zero, 
	// and the Beamship object is automatically destroyed.
	return 0;


}

