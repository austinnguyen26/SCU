/*
* Austin Nguyen, anguyen10@scu.edu
* Lab 6 roster.cxx (2 of 3)
* Description: Implementation of functions defined in roster.h using linked lists
*/

#include "roster.h"

namespace csen79 {

    Student::Student(ID_t temp, std::string fn, std::string ln)
        : id(temp), firstName(fn), lastName(ln) {
    }


	std::ostream& operator<<(std::ostream& os, const Student &t) {
		os << "ID:" << t.id << " " << "First Name: " << t.firstName << " " << "Last Name: " << t.lastName;
		return os;
	}

    void Roster::insert(Student &rec) {
        // Duplicate Check
        for (Node* current = head; current != nullptr; current = current->next) {
            if (current->data.getID() == rec.getID()) {
                std::cout << "Duplicate ID found for ID: " << rec.getID() << ". Skipping insertion." << std::endl;
                return;  // Skip insertion if duplicate is found
            }
        }
        // Insertion
        Node* newNode = new Node(rec);      // allocating memory
        if(!head) {                         // checks if head ptr is NULL (list does not exist)
            head = tail = newNode;          // initializes new list
        } else {                            // insertion when list exists
            tail->next = newNode;
            tail = newNode;
        }
        ++size;
        // Print statement (Debugging to track students)
        std::cout << "Inserted: " << rec.getFirstName() << " " << rec.getLastName() << std::endl;
    }


    void Roster::erase(Student::ID_t id){
        Node *current = head;
        Node *prev = nullptr;
        while(current != nullptr){
            if(current->data.getID() == id){
            // ID Found! Proceeds to deletion
                if(prev){                       // checks that previous node exists and is valid 
                                                // makes sure that deletion isn't at start of list
                    prev->next = current ->next;
                }else{
                    head = current->next;       // at start of list, head ptr points to current -> next
                }
                if(current == tail){            // if delete last node of list, re-point tail pointer
                    tail = prev;
                }
                delete current;
                --size;
                return;
            }
            prev = current;
            current = current -> next;
        }
        // ID not found, print message
        std::cout << "No student with ID " << id << " found, cannot erase." << std::endl;
    }

    Node* Roster::begin(void){
        return head;
    }

    Node* Roster::next(Node* current) {
        if (current) {          // checks if 'current' ptr is valid aka not nullptr
            return current->next;
        }
        return nullptr;
    }

    Node* Roster::end(void){
        return nullptr;
    }

    Roster::~Roster() {  
        Node* current = head;
        while (current) {
            Node* nextNode = current->next;  // Save pointer to next node
            delete current;                   // Delete current node
            current = nextNode;               // Move to the next node
        }
        head = tail = nullptr;                
        std::cout << "Linked List has been properly deleted" << std::endl;
    }
}