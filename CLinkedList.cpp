#include <iostream>
#include <string.h>
#include <stdlib.h>
using namespace std;

class CNode
{
    public:
	long int m_id;
    char* m_cpName;
    string m_strName = "";
    CNode* m_pPrevious;
	CNode* m_pNext;

// This function initializes the node's data members.
// Postconditions: Each of the node's data members are initialized.
void CNode::Initialize()
{
	this->m_id= 0;
	//this->m_inheritanceFlag=0;

	//~bvs~20020305:  Note that m_name must be initialized
	// to NULL (not "") in order for subsequent calls to
	// SetName() to work properly.
	this->m_strName = ""; 

	// Initialize linked-list pointers.
	this->m_pNext = NULL;
	this->m_pPrevious = NULL;
	//this->m_pLeft = NULL;
	//this->m_pRight = NULL;

	//sthis->m_pList = NULL;

	//m_eType = None;

	//~maa~20011101
}


void szSetName(string name)
{
		// Allocate new memory for the new name and
		// store the string in this new memory.
		this->m_strName = name;
	
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


// This function returns the id of a node.
// Postconditions: The id of the node is returned to the caller.
long int GetId() const
{
	return this->m_id;

	//~maa~20011101
}

void SetId(long id)
{
	this->m_id = id;
	//~maa~20011101
}

string szGetName(){
    return this->m_strName.c_str();
}


const char* GetName(){
    return this->m_cpName;
}

};


void static printSingleLinkList(CNode* ptr){
    while(ptr !=NULL){
         std::cout << ptr->GetName() << std::endl;
         ptr = ptr->m_pNext;
    }

}

void printListReverse(CNode* ptr){
    if(ptr == NULL){
        return;
    }
 printListReverse(ptr->m_pNext);
 std::cout << ptr->GetName() << std::endl;
    //return ptr;
}

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

int main()
{
     CNode* Primary = new CNode();
     Primary->SetId(000);
     Primary->SetName("Qotmagh");
     
     CNode* Secondary = new CNode();
     Secondary->SetId(001);
     Secondary->SetName("mI'lInnID");

     CNode* Tertiary = new CNode();
     Tertiary->SetId(010);
     Tertiary->SetName("'a'Setbur");

     /*CNode* Quaternary = new CNode();
     Quaternary->SetId(010);
     Quaternary->SetName("Quvar valer");

     CNode* Quinary = new CNode();
     Quinary->SetId(010);
     Quinary->SetName("jol rojpuqloD");

     Primary->m_pNext = Secondary;
     Secondary->m_pNext = Tertiary;
     Tertiary->m_pNext = Quaternary;
     Quaternary->m_pNext = Quinary;
     Quinary->m_pNext = NULL;*/

     //printSingleLinkList(Primary);
     //printListReverse(Primary);

     std::cout << Primary->GetName()<< std::endl;
     std::cout << Secondary->GetName()<< std::endl;
     std::cout << Tertiary->GetName() << std::endl;
     

    system("pause>0");
}