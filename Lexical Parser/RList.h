// RList.h: interface for the RList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RLIST_H__A7EB8DED_FD55_467A_B118_735C71A1EC9A__INCLUDED_)
#define AFX_RLIST_H__A7EB8DED_FD55_467A_B118_735C71A1EC9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h> 
#include "Node.h"
//#include <afxmt.h> // for use of CCriticalSection

/********************************************
* Note to Programmer:  In order to use this
* header file in your Visual C++ project,
* be sure to specify the project directory
* as an "Included path".  You may do this in
* the Visual C++ 6.0 IDE via the menu:Tools, Options.
* From the Options dialog, select the
* directories tab and enter
* D:\Data\Vss\Sacman\DataStructures
********************************************/
class RList  
{
public:
	// Public Enumerations:
	enum EnumListPointerType { HEAD, TAIL, CURSOR, BUFFER };
	enum EnumQueueType { PRIORITY, FIFO, LIFO };
	enum EnumFlags { IS_LOCKED = 0x001 };

public:
	bool UnLink(CNode *pNode);
	EnumQueueType GetType() { return m_eTypeOfQueue; }
	void SetType (EnumQueueType type) { m_eTypeOfQueue=type; }

	// Critical Section access functions:
	//bool Lock();//~bvs~20020104:  removed boolean argument
	//bool UnLock();

	// Node-related operations:
	bool Delete(CNode *pNode);
	CNode* DeQueueFIFO();
	virtual bool Push(CNode *pNode);//~bvs~20021130:  made virtual
	virtual bool SortIn(CNode *pNode);//~bvs~20021130:  made virtual
	virtual bool Queue(CNode* pNode);//~bvs~20021130:  made virtual
	virtual RList& Add(CNode* pNode) { Queue(pNode); return *this; } //~dv~20020617, ~bvs~20030429
	void Initialize();

	// Cursor Movement operations:
	bool PointCursorAtTail();
	bool PointCursorAtNext();
	bool PointCursorAtHead();
	bool SetCursor(int iNodeID);
	CNode* SetCursor(CNode *pNode);
	CNode * GetCursor();

	// Search operations:
	bool ContainsNode(CNode *pNode);
	CNode* GetNodeById(int id);
	CNode* GetNodeByName(const char* name);
	CNode* GetNodeByIndex(int index);

	// Access operations:
	CNode* GetBuffer();
	CNode* GetLast();
	CNode* GetTail() const;
	CNode * GetHead() const;
	CNode* GetPointerTo(EnumListPointerType eTypeOfPointer) const;

	// Buffer-related list operations:
	virtual bool Paste();//~bvs~20021130:  made virtual
	virtual bool Queue();//~bvs~20021130:  made virtual
	virtual bool Push();//~bvs~20021130:  made virtual
	virtual bool SortInFromBuffer();//~bvs~20021130:  made virtual
	bool Chop();
	bool Cut();
	bool Pop();
	bool Delete();
	int Destroy();
	bool PurgeBuffer();
	CNode* TakeFromBuffer();

	// Information functions:
	int GetCount() const;
	bool HasOnlyOne() { return GetCount() == 1; } //~bvs~20010904
	bool HasMoreThanOne() { return GetCount() > 1; } //~bvs~20010904
	bool IsLocked() {return (m_flags&IS_LOCKED) ? true : false;}
	bool IsEmpty() const { return m_pHead == NULL; } //~bvs~20010904
	bool IsCursorValid();
	bool IsSomethingInTheBuffer();
	bool IsSomethingInTheList();
	bool InitiateList(CNode *pNode=NULL);
	int PrintList();

	// Overloaded operators:
	RList& operator << (RList &sourceList);
	RList& operator >> (CNode **pNode);
	RList& operator << (CNode *pNode);
	RList& operator + (CNode* pNode) { SortIn(pNode); return *this; }
	RList& operator += (CNode* pNode) { Queue(pNode); return *this; }

	// Construction & Destruction
	RList();
	virtual ~RList();

protected:

	EnumQueueType m_eTypeOfQueue;
	int m_flags;
	bool LeaveAndUnlockTheDoor();
	bool EnterAndLockTheDoor();
	virtual bool PlaceIntoBuffer(CNode* pNode); //~bvs~20021130:  made virtual
	CNode* m_pBuffer;
	CNode* m_pCursor;
	CNode* m_pTail;
	CNode* m_pHead;

private:
	virtual bool PushFromBufferIntoList();
	//CCriticalSection m_criticalSection;
};

#endif // !defined(AFX_RLIST_H__A7EB8DED_FD55_467A_B118_735C71A1EC9A__INCLUDED_)
