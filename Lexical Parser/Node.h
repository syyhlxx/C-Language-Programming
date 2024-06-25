// Node.h: interface for the CNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NODE_H__B7715F38_D548_46D3_AFB1_BD63A3E2175F__INCLUDED_)
#define AFX_NODE_H__B7715F38_D548_46D3_AFB1_BD63A3E2175F__INCLUDED_

//#include "RBitFlags.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//~bvs~20031024:  Defining what "zero" means.
#define IS_ZERO(x) ((x) >  (-0.0001) && (x) <  (0.0001))   // ~dv~20031023
#define IS_UNDEFINED(x) ((x) == VALUE_UNDEFINED)   // ~bvs~20040601

class RList;

class CNode
{
	//Friends of this class:
	friend class RList;
public:

	//--- Bit Flags ---
/*	enum BitFlags {MODIFIED};
	bool IsModified() const { return m_flags.IsBitSet(MODIFIED); }
	bool SetIsModified(bool bSet) { return m_flags.SetBit(MODIFIED, bSet); }
	//--- Bit Flags ---*/

	//Class enumerations:
	enum EnumNodeType {
		PREVIOUS,
		NEXT,
		FIRST,
		LAST,
	};

	enum EnumTokenType{
		None,
		Begin,
		Integer,
		GreaterThan,
		LessThan,
		Equals,
		Plus,
		Minus,
		Error,
		Identifier,
		Semicolon,
		While,
		If,
		Set,
		Root,
		Read,
		Write,
		To,
		End,
	};

	//Methods for accessing data members:
	bool operator < (const CNode& node) const;
	int GetListIndex() const;
	long int GetId() const;
	virtual const char* GetName() const;
	// Call this function to set the token type of the node.
	bool SetTokenType(EnumTokenType TypeOfToken);
	// Call this function to return the node's current type.
	EnumTokenType GetTokenType();
	// Call this function to set the next pointer of a Node object.
	void SetLeft(CNode* pNextNode);
	// Call this function to set the previous pointer of a Node object.
	void SetRight(CNode* pPreviousNode);
	CNode* GetNext() const;
	CNode* GetPrevious(void) const;
	CNode* GetPointerTo(CNode::EnumNodeType eTypeOfNode);
	bool Matches(const char *name) const;
	bool Matches(int id) const;
	bool SetAsNext(CNode* pNode);
	bool SetAsPrevious(CNode* pNode);
	virtual bool SetDataMember(const char* sNameOfDataMember, double value);
	void SetId(long int id);
	void SetId(int iId);
	virtual void SetName(const char * name);
	//Methods for performing actions:
	bool Unlink();
	//Construction:
	CNode(const CNode &node);
	CNode(const char* name, long int id);
	CNode(long int id);
	//Destruction:
	virtual ~CNode();

protected:
	long int m_id;
	char* m_name;
	EnumTokenType m_eType;
	CNode* m_pPrevious;
	CNode* m_pNext;
	CNode* m_pLeft;
	CNode* m_pRight;


	virtual bool IsLessThan(const CNode *pNode) const; // Note that derived classes must use the
													   // exact same prototype.
	//Inheritance can only be established by a derived class.
	/*void SetInheritanceFlag(
		const unsigned int inheritanceFlag) 
	{ m_inheritanceFlag|= inheritanceFlag; }*/

	void Initialize();

	//Data members:
	unsigned long int m_inheritanceFlag;
	//RBitFlags m_flags;//~bvs~20030326

private:
	RList *m_pList; //~bvs~20040806

public:
	// Call this function to get the left node of a Node object
	CNode* GetLeft(void);
	// Call this function to get the left Node of a Node object.
	CNode* GetRight(void);
};

#endif // !defined(AFX_NODE_H__B7715F38_D548_46D3_AFB1_BD63A3E2175F__INCLUDED_)
