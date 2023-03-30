#include "Set.h"
#include <iostream>

using namespace std;

Set::Set() {
    head = new Node; //head points to dummy node
    //make next and prev pointers point to dummy node
    head->next = head;
    head->prev = head;
    m_size = 0; // set m_size to 0 because Set is empty
}

Set::Set(const Set& src) {
    //assign src's size to this Set's size
    m_size = src.m_size;
    //allocate new space in memory for this Set’s linked list
    head = new Node;
    // Make dummy node’s next and prev pointers point to dummy node
    head->next = head;
    head->prev = head;
    //create a temp ptr pointing to newly added linked list
    Node* temp = head->next;
    //create a temp ptr pointing to src’s lists
    Node* tempSrc = src.head->next;
    for (; tempSrc != src.head; tempSrc = tempSrc->next) {
        Node* valPtr = new Node; //create new valPtr to assign src to this
        valPtr->m_value = tempSrc->m_value; //assign src's item to the new one
        valPtr->next = temp->next; //make newly added node’s next point to dummy node
        valPtr->prev = temp; //make newly added node’s prev point to previous node
        temp->next = valPtr; //connect prev node to new node
        head->prev = valPtr; //complete circle by connecting first and last nodes
        temp = valPtr;
    }
}

Set& Set::operator=(const Set& rhs) {
    if (this != &rhs) {
        //create temp Set to store values of rhs
        //swap sets
        Set temp(rhs);
        swap(temp);
    }
    return *this;
}

Set::~Set() {
    while (!empty()) { // while Set's m_size != 0, meaning it is not empty, enter the while loop
        Node* temp = head->next;//create temp ptr to point to the first item to be deleted
        head->next = temp->next; //adjust ptr so head ptr pointers to the next node; if only one m_value item in linked list, point to head.
        temp->next->prev = head; //complete circle by making temp->next->prev = head
        //make soon-to-be-deleted temp node’s pointers equal to nullptr so that they cannot be incorrectly accessed
        temp->next = nullptr; //
        temp->prev = nullptr;
        delete temp; //delete current m_value item
        m_size--; //decrement m_size
    }
    //delete dummy node
    delete head;
}

bool Set::empty() const {
    return size() == 0; // return true if m_size = 0
}

int Set::size() const {
    return m_size; // return size of Set
}

bool Set::insert(const ItemType& value) {
    if (contains(value)) { //return false if value already exists in Set
        return false;
    }
    if (this->empty()) { //check if Set is empty
        Node* p = new Node; //create new Node p to insert new value after dummy Node
        p->m_value = value; //set p->m_value to value
        //connect nodes p and head
        p->next = head;
        p->prev = head;
        head->next = p;
        head->prev = p;
        m_size++; //increment size
        return true;
    }
    Node* q = head->next; //create new Node q to insert new value in the middle of Set, q = head->next
    while (q != head) { //q = head would complete the circle
        if (q->m_value > value) { //check if m_value > value
            Node* z = new Node; //create new Node z to place in Set
            z->m_value = value; //set z->m_value to value
            //connect nodes q and z
            z->next = q;
            z->prev = q->prev;
            q->prev->next = z;
            q->prev = z;
            m_size++; //increment size
            return true;
        }
        q = q->next; //increment q to q->next if q->m_value is not greater than value
    }
    Node* end = new Node; //create new Node end to place new Node at the end of the Set
    end->m_value = value; //set end->m_value to value
    //connect nodes end and head
    end->next = head;
    end->prev = head->prev;
    head->prev->next = end;
    head->prev = end;
    m_size++; //increment size
    return true;
}

bool Set::erase(const ItemType& value) {
    if (!contains(value)) {
        return false; //return false if list doesn’t contain value
    }
    //utilizing next, traverse to position in linked list where value is located
    Node* temp = head->next;
    //make temp pointer point to location of value in linked list
    while (temp != head && temp->m_value != value) {
        temp = temp->next;
    }
    //delete value
    (temp->prev)->next = temp->next;
    (temp->next)->prev = temp->prev;
    //make soon-to-be-deleted node temp's pointers equal to nullptr
    temp->next = nullptr;
    temp->prev = nullptr;
    delete temp;
    temp = nullptr; //set deleted node temp to nullptr
    m_size--; //decrement size
    return true;
}

bool Set::contains(const ItemType& value) const {
    Node* n = head->next; //create new node pointing to head->next
    while (n != head) { //iterate through linked list until circle is complete
        if (n->m_value == value) { //check if linked list already contains value. if it does, return true.
            return true;
        }
        n = n->next; //increment n to n->next
    }
    return false; //return false if value is not found in linked list
}

bool Set::get(int pos, ItemType& value) const
{
    if (pos < 0 || pos >= m_size) { //checks if pos is valid
        return false;
    }
    //create temp pointer to locate position of value
    Node* temp = head->next;
    int tempIndex = 0;
    //stop iterating when temp node pointer points to node at position of value
    for (; temp != head && tempIndex < pos; temp = temp->next, tempIndex++) {
        ;
    }
    value = temp->m_value; //assign that node’s m_value to the value in the parameter
    return true;
}

void Set::swap(Set& other) {
    //Swap sizes
    int tempSize; //create tempSize variable to swap sizes
    tempSize = other.m_size;
    other.m_size = m_size;
    m_size = tempSize;
    //Swap head pointers to the linked list
    Node* swapHeadNodes = head; //create swapHeadNodes Node to swap head nodes
    head = other.head;
    other.head = swapHeadNodes;
}

void unite(const Set& s1, const Set& s2, Set& result) {
    Set tempSet; //create a new empty set called tempSet
    for (int j = 0; j < s1.size(); j++) {
        ItemType val;
        s1.get(j, val);
        tempSet.insert(val); //insert s1’s list into tempSet; utilize insert function to avoid duplicates
    }
    for (int k = 0; k < s2.size(); k++) {
        ItemType val;
        s2.get(k, val);
        tempSet.insert(val); //insert s2’s list into tempSet; utilize insert function to avoid duplicates
    }
    result = tempSet; //assign tempSet to result
}

void butNot(const Set& s1, const Set& s2, Set& result) {
    Set tempSet; //create a new empty set called tempSet
    //use unite function to unite Set s1 and Set s2 into tempSet
    unite(s1, s2, tempSet);
    for (int i = 0; i < s2.size(); i++) {
        ItemType item;
        s2.get(i, item);
        tempSet.erase(item); //erase items of s2 in tempSet
    }
    result = tempSet; //assign tempSet to result
}

void Set::dump() const
{
    cerr << "Size of the Set: " << m_size << endl; //print out size of Set
    cerr << "Items in the Set: " << endl; //print out Items in Set
    Node* temp = head->next;
    for (; temp != head; temp = temp->next) {
        cerr << temp->m_value << " "; //print out individual values in Set
    }
    cerr << endl;
}