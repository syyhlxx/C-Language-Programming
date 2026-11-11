// linked_list_modern.cpp
// Modernized version of linked_list_2001.cpp (original ~maa~20011101, ~bvs~20020305).
// Keeps the original design: a CNode class, a hand-built doubly linked list,
// raw char* names, and the Klingon test data. 

#include <iostream>
#include <cstring>   // Needed for std::strlen and std::strcpy
#include <string>    // Needed for std::string

class CNode
{
public:
    long int m_id = 0;              // Members start with safe defaults
    char*    m_cpName = nullptr;    // so nothing holds garbage values
    CNode*   m_pPrevious = nullptr;
    CNode*   m_pNext = nullptr;

    CNode() = default;

    // Destructor frees the name memory this node owns.
    ~CNode()
    {
        delete[] m_cpName;
    }

    // Copying a node would make two nodes share (and both free)
    // the same name memory, so copying is turned off.
    CNode(const CNode&) = delete;
    CNode& operator=(const CNode&) = delete;

    long int GetId() const
    {
        return m_id;
    }

    // Returns "" instead of a null pointer when no name is set,
    // so printing an unnamed node is safe. Marked const like GetId.
    const char* GetName() const
    {
        return m_cpName ? m_cpName : "";
    }

    void SetId(long int id)
    {
        m_id = id;
    }

    
    void SetName(const char* name)
    {
        delete[] m_cpName;
        m_cpName = nullptr;

        if (name)
        {
            std::size_t len = std::strlen(name) + 1;   // +1 for the '\0' terminator
            m_cpName = new char[len];
            std::memcpy(m_cpName, name, len);           // copies the text and the terminator
        }
    }
    
    // Convenience overload so a std::string still works, like szSetName intended.
    void SetName(const std::string& name)
    {
        SetName(name.c_str());
    }
};

// The "create node, set fields, hook it to the tail" block that was
// repeated for every node in main() is now one function.
// It returns the new node so the caller can update its tail pointer.
CNode* AppendNode(CNode*& head, CNode*& tail, long int id, const char* name)
{
    CNode* node = new CNode();
    node->SetId(id);
    node->SetName(name);
    node->m_pPrevious = tail;   // nullptr when the list is empty

    if (tail)
        tail->m_pNext = node;
    else
        head = node;            // first node is both head and tail

    tail = node;
    return node;
}

void printForward(const CNode* head)
{
    for (const CNode* visitor = head; visitor != nullptr; visitor = visitor->m_pNext)
        std::cout << visitor->GetId() << " " << visitor->GetName() << '\n';
}

void printInReverse(const CNode* tail)
{
    for (const CNode* visitor = tail; visitor != nullptr; visitor = visitor->m_pPrevious)
        std::cout << visitor->GetId() << " " << visitor->GetName() << '\n';
}

// Walks the list and deletes every node (each node's destructor
// frees its name). The original never freed anything.
void freeList(CNode*& head, CNode*& tail)
{
    while (head)
    {
        CNode* next = head->m_pNext;
        delete head;
        head = next;
    }
    tail = nullptr;
}

int main()
{
    CNode* Head = nullptr;   // Start empty, not uninitialized
    CNode* Tail = nullptr;

    // CHANGE 9: the IDs were written 000, 001, 010, 011, 100. In C++ a leading
    // 0 means octal, so 010 was 8 and 011 was 9. The 0b prefix (C++14) makes
    // them real binary: 0, 1, 2, 3, 4.
    AppendNode(Head, Tail, 0b000, "Qotmagh");
    AppendNode(Head, Tail, 0b001, "mI'lInnID");
    AppendNode(Head, Tail, 0b010, "'a'Setbur");
    AppendNode(Head, Tail, 0b011, "Quvar valer");
    AppendNode(Head, Tail, 0b100, "jol rojpuqloD");   // CHANGE 10: Quinary is now linked in

    std::cout << "Forward:\n";
    printForward(Head);

    std::cout << "\nReverse:\n";
    printInReverse(Tail);

    freeList(Head, Tail);

    // CHANGE 11: portable pause instead of Windows-only system("pause>0").
    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    return 0;
}
