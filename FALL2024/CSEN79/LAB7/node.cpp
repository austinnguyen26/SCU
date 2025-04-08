/*
Austin Nguyen | anguyen10@scu.edu
CSEN79 Lab 7 node.cpp
Description: Implementation of the node class in node.h
*/

#ifndef ITEM_CPP
#define ITEM_CPP

#include "node.h"

namespace coen79_lab7
{
    node::node(const std::string &itemName, const float &newPrice, node *nextNode) {
        name = itemName;
        price = newPrice;
        link = nextNode;
    }
    
    void node::setName(const std::string &newName) {
        name = newName;
    }
    
    void node::setPrice(const float &newPrice) {
        price = newPrice;
    }
    
    void node::setLink(node *new_next) {
        link = new_next;
    }
    
    node* node::getLink() {
        return link;
    }
    
    const node* node::getLink() const {
        return link;
    }
    
    std::string node::getName() const{
        return name;
    }
    
    float node::getPrice() const {
        return price;
    }
    
    
    void list_init(node*& head, node*& tail, const std::string& newName, const float& newPrice) {
        head = new node(newName, newPrice, NULL);
        tail = head;
    }
    
//  void list_tail_insert(node*& tail_ptr, const std::string& newName, const float& newPrice)
//  Precondition: tail_ptr is the tail pointer of a non-empty linked list,
//      newName is a non_empty string
//  Postcondition: Adds a node to the end of the linked list.
    void list_tail_insert(node*& tail, const std::string &newName, const float &newPrice) {

        // IMPLEMENTATION...
        tail ->setLink(new node(newName, newPrice, NULL));
        tail = tail->getLink();
    }
    
//   void list_clear(node*& head_ptr)
//   Precondition: head_ptr is the head pointer of a linked list.
//   Postcondition: All nodes of the list have been returned to the heap,
//     and the head_ptr is now NULL.
    void list_clear(node*& head) {
        // IMPLEMENTATION...
        while(head != NULL){
            list_head_remove(head);
        }
    }
    
    
    void list_copy(const node *old_head, node* &new_head, node* &new_tail) {
        new_head = NULL;
        new_tail = new_head;
        
        const node *cur = old_head;
        while (cur != NULL) {
            if (new_head == NULL) {
                new_head = new node(cur->getName(), cur->getPrice());
                new_tail = new_head;
            }
            else {
                new_tail->setLink(new node(cur->getName(), cur->getPrice()));
                new_tail = new_tail->getLink();
            }
            cur=cur->getLink();
        }
    }
    
//   void list_head_remove(node*& head_ptr)
//   Precondition: head_ptr is the head pointer of a linked list, with at
//     least one node.
//   Postcondition: The head node has been removed and returned to the heap;
//     head_ptr is now the head pointer of the new, shorter linked list.
    void list_head_remove(node*& head) {
        // IMPLEMENTATION...
        node *remove = head;
        head = head->getLink();
        delete remove;
    }
    
    
    void list_print(node *head) {
        node *cur = head;
        while (cur != NULL) {
            std::cout << "- " << cur->getName() << ", where the price is $" << cur->getPrice() << std::endl;
            cur = cur->getLink();
        }
    }
    
    
    bool list_contains_item(node *head_ptr, const std::string& newName) {
        return (list_search(head_ptr, newName) != NULL);
    }
    
    
    node* list_search(node* head_ptr, const std::string& target)
    {
        node *cursor;
        
        for (cursor = head_ptr; cursor != NULL; cursor = cursor->getLink( ))
            if (target == cursor->getName( ))
                return cursor;
        return NULL;
    }
    
    
    const node* list_search(const node* head_ptr, const std::string & target)
    {
        const node *cursor;
        
        for (cursor = head_ptr; cursor != NULL; cursor = cursor->getLink( ))
            if (target == cursor->getName( ))
                return cursor;
        return NULL;
    }
    
}

#endif
