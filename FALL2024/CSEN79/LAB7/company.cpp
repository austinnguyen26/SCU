// FILE: company.cpp
// CLASS implemented: company (see company.h for documentation)
/*
Austin Nguyen | anguyen10@scu.edu
CSEN79 Lab 7 company.cpp
Description: Implementation of functions in company.h
*/

#include <cassert>
#include "company.h"

//#define USEDEBUG

#ifdef USEDEBUG
#define Debug( x ) std::cout << x
#else
#define Debug( x )
#endif

namespace coen79_lab7
{
//  Default Constructor
//  company();
//  Postcondition: The company object is initialized with empty company name
    company::company() {
        this->company_name = "";
        this->head_ptr = NULL;
        this->tail_ptr = NULL;
    }
    
//  company(const std::string& company_name);
//   Precondition: company_name is a non-empty string
//   Postcondition: The company object is initialized with "company_name"
    company::company(const std::string& company_name) {
        assert(company_name.length() > 0);
        this->company_name = company_name;
        this->head_ptr = NULL;
        this->tail_ptr = NULL;
    }

//  Copy Constructor | Value Semantics
    company::company(const company &src) {
        Debug("Company copy constructor..." << std::endl);

        // IMPLEMENTATION...
        head_ptr = NULL;
        tail_ptr = NULL;
        company_name = src.company_name;
        list_copy(src.head_ptr, head_ptr, tail_ptr);
    }

//  Assignment Operator = | Value Semantics
    company& company::operator= (const company &src) {
        Debug("Company assignemnt operator..." << std::endl);

        // IMPLEMENTATION...
        if (this == &src){
            return *this;
        }
        list_clear(head_ptr);
        company_name = src.company_name;
        list_copy(src.head_ptr, head_ptr, tail_ptr);
        return *this;
    }
    
//  Destructor
    company::~company() {
        list_clear(head_ptr);
    }
    
//  Getter Functions (Given) just return whatever function asks for
    std::string company::get_name() const {
        return company_name;
    }
    
    const node *company::get_head() const {
        return head_ptr;
    }
    
    const node *company::get_tail() const {
        return tail_ptr;
    }
    
    node* company::get_head() {
        return head_ptr;
    }
    
    node* company::get_tail() {
        return tail_ptr;
    }
    
    void company::print_items() {
        list_print(head_ptr);
    }
    
    
//  bool insert(const std::string& name, const float &price);
//  Precondition: product_name is a non-empty string
//  Postcondition: Creates and inserts a new node (product) to the back of the linked list.
//      Returns true if successfully inserted, and returns false if there is a duplicate
    bool company::insert(const std::string& product_name, const float& price) {
        
        assert(product_name.length() > 0);

        if (list_contains_item(head_ptr, product_name)) {
            return false;
        }
        
        if (head_ptr == NULL) {
            // IMPLEMENTATION...
            list_init(head_ptr, tail_ptr, product_name, price);
        }
        else {
            // IMPLEMENTATION...
            list_tail_insert(tail_ptr, product_name, price);
        }
        
        return true;
    }

//  bool erase(const std::string& product_name)
//  Postcondition:  Erases the product that its name matches product_name. Returns true if the
//      product has been found and deleted, returns false otherwise
    bool company::erase(const std::string& product_name) {
        assert(product_name.length() > 0);

        // IMPLEMENTATION...
        node *prev = NULL;
        node *current = head_ptr;

        while(current != NULL){
            if(current->getName() == product_name){
                if (prev == NULL){
                    head_ptr = current->getLink();
                }else{
                    prev->setLink(current->getLink());
                }
                if(current == tail_ptr){
                    tail_ptr = prev;
                }
                delete current;
                return true;
            }
            prev = current;
            current = current->getLink();
        }
        return false;
    }
}
