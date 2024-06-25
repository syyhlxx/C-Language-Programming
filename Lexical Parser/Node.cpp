// Node.cpp: implementation of the CNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Node.h"
#include "RList.h"
#include ".\node.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// The constructor establishes the name of the node as well as the id
// then calls the Initialize function which initializes all of the
// node's data members.
CNode::CNode(const char* name, long int id)
{
	this->Initialize();
	this->SetName(name);
	this->m_id = id;

	//~maa~2001201
}

CNode::~CNode()
{
	this->SetName(NULL);

	//~bvs~20040808:  If there is a list associated with this node,
	// tell the list to unlink this node.
	if (this->m_pList)
	{
		this->m_pList->UnLink(this);
	}
	else
	{
		this->Unlink();
	}
}
// This overidden constructor allows the creation of a node
// by passing it only an id.
CNode::CNode(long int id)
{
	this->Initialize();

	this->m_id = id;

	//~maa~20011101
}

// This function initializes the node's data members.
// Postconditions: Each of the node's data members are initialized.
void CNode::Initialize()
{
	this->m_id= 0;
	this->m_inheritanceFlag=0;

	//~bvs~20020305:  Note that m_name must be initialized
	// to NULL (not "") in order for subsequent calls to
	// SetName() to work properly.
	this->m_name = NULL; 

	// Initialize linked-list pointers.
	this->m_pNext = NULL;
	this->m_pPrevious = NULL;
	this->m_pLeft = NULL;
	this->m_pRight = NULL;

	this->m_pList = NULL;

	m_eType = None;

	//~maa~20011101
}

// This function allows the caller to establish the name of the node.
// Preconditions:
// Postcondtions: The node's name reassigned the name passed in by
// the caller.
void CNode::SetName(const char* name)
{

	//~bvs~20020305:  Writing this code out more explicitly now.
	// Check to see if a name has already been established
	if (this->m_name != NULL)
	{
		// De-allocate the memory that was allocated for
		// the pre-existing name.
		delete this->m_name;
	}

	// Check to see if a name was specified.
	if (name)
	{
		// Allocate new memory for the new name and
		// store the string in this new memory.
		this->m_name = strdup(name);
	}
	else
	{
		// There will be no name for this node, so
		// make sure the member points to NULL.
		this->m_name = NULL;
	}

	//~maa~20011101
	//~bvs~20020305
}

// This function allows the caller to set the id of a node.
// Postconditions: The node's id is set to that provided by the
// caller.
void CNode::SetId(long id)
{
	this->m_id = id;

	//~maa~20011101
}

// This function returns the name of the node to the caller.
// Postconditions: The name of the node is returned to the caller.
const char* CNode::GetName() const
{
	return this->m_name;

	//~maa~20011101
}


// This function returns the id of a node.
// Postconditions: The id of the node is returned to the caller.
long int CNode::GetId() const
{
	return this->m_id;

	//~maa~20011101
}

// This class method is responsible for unlinking a 
// CNode object from its sibling previous and next objects.
// Pre-condtions: The node to be unlinked must have a previous node 
// and a next node.
// Postconditions: The node is removed from the list and its previous
// node is made to point to the unlinked node's next node. As well as its
// next node's previous is made to point to the unlinked node's previous.
// The boolean unlinked is set to true and returned to the caller.
bool CNode::Unlink()
{
	// Declare and initialize local variables.
	bool unlinked=false;

	// If there is a node before the node to be unlinked, make the
	// previous node point to the unlinked node's next.
	if (this->m_pPrevious != NULL)
	{
		this->m_pPrevious->m_pNext = this->m_pNext;
		unlinked=true;
	}
	
	// If there is a node after the node to be unlinked, make that next
	// node's previous point to the node to be unlinked's previous.
	if (this->m_pNext != NULL)
	{
		this->m_pNext->m_pPrevious = this->m_pPrevious;
		unlinked=true;
	}
	
	// Set the unlinked node's next and previous pointers to NULL.
	this->m_pPrevious = this->m_pNext = NULL;

	//~bvs~20040808
	this->m_pList = NULL;
	
	return unlinked;

	//~maa~20011101

}

// This function allows the caller to establish a node as the
// previous node of the current node.
// Preconditions: The current node must not be at the end of the list. 
// Postconditions: The current node's previous is pushed back and the
// new node is put into its place. The boolean was set as previous is
// set to true and returned to the caller.
bool CNode::SetAsPrevious(CNode *pNode)
{
	// Declare and initialize local variables.
	bool wasSetAsPrevious = false;

	if (pNode)
	{
		// First, make sure the given link is unlinked.
		pNode->Unlink();

		// Then, tie the given link to the previous link.
		// Note that if there is no previous node, the following
		// code will still work correctly.
		if (this->m_pPrevious)
			this->m_pPrevious->m_pNext = pNode;
		pNode->m_pPrevious = this->m_pPrevious;

		// Finally, tie the given link to this link, set
		// wasSetAsPrevious to true and return it to the
		// caller.
		pNode->m_pNext = this;
		this->m_pPrevious = pNode;

		wasSetAsPrevious = true;
	}
	return wasSetAsPrevious;

	//~maa~20011101
}

// This function allows the caller to set a node as the current
// node's next.
// Preconditions: The current node must not be at the end
// of the list.
// Postconditions: The current node's next is pushed back in the list,
// and the new node becomes the "next" node. The boolean wasSetToNext
// is set to true and returned to the caller.
bool CNode::SetAsNext(CNode *pNode)
{
	//Declare and initialize local variables.
	bool wasSetAsNext = false;

	// If the input parameter is not NULL...
	if (pNode)
	{
		// First, make sure the given link is unlinked.
		pNode->Unlink();

		// Then, tie the given link to the next link.
		if (this->m_pNext)
			this->m_pNext->m_pPrevious = pNode;
		pNode->m_pNext = this->m_pNext;

		// Finally, tie the given link to this link.
		// Set wasSetAsNext to true and return it to the
		// caller.
		pNode->m_pPrevious = this;
		this->m_pNext = pNode;

		wasSetAsNext = true;
	}
	return wasSetAsNext;

	//~maa~20011101
}

// This class method returns a pointer to a node that is either
// "upstream" or "downstream" from this CNode object, depending
// on the type of node specified (via the "eTypeOfNode" parameter).
// Preconditions: The caller must pass this function a valid enumerated
// type to specify what type of pointer the function is to return.
// Postconditions: A pointer to the type of node specified is 
// returned to the caller.
CNode* CNode::GetPointerTo(CNode::EnumNodeType eTypeOfNode)
{
	//Declare and initialize local variables.
	CNode* pNode=NULL;
	
	switch(eTypeOfNode)
	{
	case PREVIOUS:
		
		pNode=m_pPrevious;
		break;
		
	case NEXT:
		pNode = m_pNext;
		break;
		
	case FIRST:
		for(pNode=this;pNode->m_pPrevious!=NULL;pNode=pNode->m_pPrevious)
		{}
		break;
		
	case LAST:
		for(pNode=this;pNode->m_pNext!=NULL;pNode=pNode->m_pNext)
		{}
		break;
		
	default:
		pNode = NULL;
		break;
		
	}
	return pNode;

	//~maa~20011114
	//~dv~20020803
}



// This function returns a pointer to the current node's
// next.
CNode* CNode::GetNext() const
{
	return this->m_pNext;

	//~maa~20011114
	//~dv~20020725
}

// This function allows the caller to set the id of and value contained
// by a node.
// Pre-condtions: The id passed in by the caller must not be identical
// to the current id.
// Postconditions: The id and value passed in by the caller become
// the id and value of the current node. The boolean done is set to
// true and returned to the caller.
bool CNode::SetDataMember(const char *sNameOfDataMember, double value)
{
	bool done=false;
	if (sNameOfDataMember != NULL)
	{
		if (done=!stricmp(sNameOfDataMember,"ID"))
			this->m_id=(int)value;
	}
	return done;

	//~maa~20011114
}

int CNode::GetListIndex() const
{
	int index=0;
	CNode *pNode=this->m_pPrevious;
	while (pNode)
	{
		index++;
		pNode=pNode->m_pPrevious;
	}
	return index;
}

bool CNode::operator<(const CNode& node) const
{
	return IsLessThan(&node);
}

bool CNode::IsLessThan(const CNode *pNode) const
{
	bool bIsLessThan=false;
	if (pNode)
	{
		if (this->GetName() && pNode->GetName())
		{
			if (stricmp(this->GetName(), pNode->GetName())<0)
				bIsLessThan=true;
		}
		else if (this->GetId()<pNode->GetId())
			bIsLessThan=true;
	}
	return bIsLessThan;
}

CNode* CNode::GetPrevious() const
{
	return this->m_pPrevious;
	//~dv~20020615
}

void CNode::SetId(int iId)
{
	this->m_id=iId;
	//~dv~20020726
}

bool CNode::Matches(int id) const
{
	return (this->m_id==id) ? true : false;
	//~bvs~20021029
}

bool CNode::Matches(const char *name) const
{
	if (name && this->m_name)
		return stricmp(name,this->m_name) ? false : true;
	return false;
	//~bvs~20021029
}

CNode::CNode(const CNode &node)
{
	this->Initialize();
	this->SetName(node.GetName());
	this->SetId(node.GetId());
	//this->SetInheritanceFlag(node.m_inheritanceFlag);
	//this->m_flags=node.m_flags;
	//~bvs~20030322
}

bool CNode::SetTokenType(EnumTokenType TypeOfToken)
{
	bool bWasChanged = false;
	if(m_eType != TypeOfToken)
		bWasChanged = true;
	m_eType = TypeOfToken;
	return false;
}

// Call this function to return the node's current type.
CNode::EnumTokenType CNode::GetTokenType(void)
{
	return  m_eType;
}

// Call this function to set the next pointer of a Node object.
void CNode::SetLeft(CNode* pLeftNode)
{
	this->m_pLeft = pLeftNode;
}

// Call this function to set the previous pointer of a Node object.
void CNode::SetRight(CNode* pRightNode)
{
	this->m_pRight = pRightNode;
}

// Call this function to get the left node of a Node object
CNode* CNode::GetLeft(void)
{
	return this->m_pLeft;
}

// Call this function to get the left Node of a Node object.
CNode* CNode::GetRight(void)
{
	return this->m_pRight;
}
