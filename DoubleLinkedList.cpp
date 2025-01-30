#include <iostream>
using namespace std;

class CNode
{
    public:
	long int m_id;
    char* m_cpName;
    CNode* m_pPrevious;
	CNode* m_pNext;


long int GetId() const
{
	return this->m_id;

	//~maa~20011101
}

const char* GetName()
{
    return this->m_cpName;
}

void SetId(long id)
{
	this->m_id = id;
	//~maa~20011101
}

void szSetName(string name)
{
		// Allocate new memory for the new name and
		// store the string in this new memory.
        char* charPtr = new char[name.length() + 1];
        std::strcpy(charPtr, name.c_str());
    
        //std::cout << charPtr << std::endl;
        this->m_cpName= charPtr;
        // Don't forget to free the allocated memory
        delete[] charPtr;
		
}

void SetName(const char* name)
{
	// Check to see if a name was specified.
	if (name)
	{
		// Allocate new memory for the new name and
		// store the string in this new memory.
		this->m_cpName = strdup(name);
	}
	
	//~maa~20011101
	//~bvs~20020305
}

};


char* strdup (const char* s)
{
  size_t slen = strlen(s);
  char* result = (char*) malloc(slen + 1);
  if(result == NULL)
  {
    return NULL;
  }

  memcpy(result, s, slen+1);
  return result;
}

void printForward(CNode* head){
CNode* visitor = head;
    while(visitor !=nullptr){
        cout << visitor->GetId() << endl;
        visitor = visitor->m_pNext;
    }
}

void printInReverse(CNode* tail){
CNode* visitor = tail;
    while(visitor !=nullptr){
        cout << visitor->GetId() << " " 
        << visitor->GetName() << endl;
        visitor = visitor->m_pPrevious;
    }

}

int main()
{
    CNode* Head;
    CNode* Tail;

    CNode* Primary = new CNode();
     Primary->SetId(000);
     Primary->m_pNext = nullptr;
     Primary->m_pPrevious = nullptr;
     Primary->SetName("Qotmagh");

     Head = Primary;
     Tail = Primary;
     
     // Code repeats to add any new nodes to the list.
     CNode* Secondary = new CNode();
     Secondary->SetId(001);
     Secondary->SetName("mI'lInnID");
     Secondary->m_pNext = nullptr;
     Secondary->m_pPrevious = Tail;
     Tail->m_pNext = Secondary;
     Tail = Secondary;

     CNode* Tertiary = new CNode();
     Tertiary->SetId(010);
     Tertiary->SetName("'a'Setbur");
     Tertiary->m_pNext = nullptr;
     Tertiary->m_pPrevious = Tail;
     Tail->m_pNext = Tertiary;
     Tail = Tertiary;
    
    CNode* Quaternary = new CNode();
     Quaternary->SetId(011);
     Quaternary->SetName("Quvar valer");
     Quaternary->m_pNext = nullptr;
     Quaternary->m_pPrevious = Tail;
     Tail->m_pNext = Quaternary;
     Tail = Quaternary;

     CNode* Quinary = new CNode();
     Quinary->SetId(100);
     Quinary->SetName("jol rojpuqloD");

    printInReverse(Tail);

     system("pause>0");
}