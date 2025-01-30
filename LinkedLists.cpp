#include <iostream>
using namespace std;

class Node {
public:
int value;
Node* Next;
};

void push(Node** head, int newValue){
    // 1. Prepare a new node.
    Node* newNode = new Node();
    newNode->value = newValue;

    // 2. Add it to the front of the head.
    newNode->Next = *head;

    // 3. Point head of list to point to new node.
    *head = newNode;

}

void enqueue(Node** head, int newValue){
    //1. Prepare a new Node.
    Node* newNode = new Node();
    newNode->value = newValue;
    newNode->Next = NULL;

    //2. If list is empty, newNode will be
    // the head node.
    if(*head == NULL){
        *head = newNode;
        return;
    }

   
    //3. Find the last node.
    Node* tail = *head;
    while(tail->Next !=NULL){
        tail = tail->Next;
    }
    //4. Insert newNode after last node (the end or tail...)
    tail->Next = newNode;
}

void printList(Node* nodePtr){

    while(nodePtr!=NULL){
        cout << nodePtr->value << endl;
        nodePtr = nodePtr->Next;
    }
}

int main()
{
    Node* head = new Node();
    Node* Node00 = new Node();
    Node* Node01 = new Node();

    head->value = 0;
    head->Next = Node00; 

    Node00->value = 11;
    Node00->Next = Node01;

    Node01->value = 111;
    Node01->Next = NULL;

    push(&head,999);
    enqueue(&head, 777);
    printList(head);

    system("pause>0");
}