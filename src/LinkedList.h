#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Node.h"
#include <iostream>

template <typename T>
class LinkedList{
 private:
  typedef Node<T> node_type;
  typedef node_type* node_ptr;
  
  node_ptr front_ptr;
  int listSize{0};
  int ID;
  
 public:
  LinkedList();

  void addtoFront(node_ptr newnodeptr);
  void addtoBack(node_ptr newnodeptr);
  void delFront();
  void delBack();
  void delID(int ID);
  int genID();
  void printNames();
  void printIDs();
  node_ptr getNodebyID(int ID);
  int size();
  node_ptr getNodei(int i);
  node_ptr getNodebyPos(int x,int y);
};

template <typename T>
LinkedList<T>::LinkedList(){
  this->front_ptr = nullptr;
}

template <typename T>
int LinkedList<T>::genID(){
  int temp;
  temp = this->ID;
  this->ID++;
  return temp;
}

template <typename T>
void LinkedList<T>::addtoFront(node_ptr newnodeptr){
  newnodeptr->ID = genID();
  if(front_ptr == nullptr){
    front_ptr = newnodeptr;
    front_ptr->next = nullptr;
  }
  else{
    node_ptr temp = new node_type;
    temp = front_ptr;
    front_ptr = newnodeptr;
    front_ptr->next = temp;
  }
  listSize++;
}

template <typename T>
void LinkedList<T>::addtoBack(node_ptr newnodeptr){
  newnodeptr->ID = genID();
  if(front_ptr == nullptr){
    front_ptr = newnodeptr;
    front_ptr->next = nullptr;
  }
  else{
    node_ptr temp = new node_type;
    temp = front_ptr;
    for(int i = 1; i < listSize; i++){
      temp = temp->next;
    }
    temp->next = newnodeptr;
    newnodeptr->next = nullptr;
  }
  listSize++;
}

template <typename T>
void LinkedList<T>::printNames(){
  if(listSize == 0){return;};
  node_ptr temp;
  temp = front_ptr;
  for(int i = 1; i <= listSize; i++){
    std::cout << temp->name << std::endl;
    temp = temp->next;
  }
}

template <typename T>
void LinkedList<T>::printIDs(){
  if(listSize == 0){return;};
  node_ptr temp;
  temp = front_ptr;
  for(int i = 1; i <= listSize; i++){
    std::cout << temp->ID << std::endl;
    temp = temp->next;
  }
}

template <typename T>
void LinkedList<T>::delFront(){
  node_ptr prev = new node_type;
  if(front_ptr == nullptr){return;}
  else{
    prev = front_ptr;
    front_ptr = front_ptr->next;
    delete prev;
  }
  listSize--;
}

template <typename T>
void LinkedList<T>::delBack(){
  if(front_ptr == nullptr){return;}
  else{
    node_ptr temp = new node_type;
    node_ptr prev = new node_type;
    temp = front_ptr;
    for(int i=1; i <= listSize; i++){
      prev = temp;
      temp = temp->next;
    }
    prev->next = nullptr;
    delete temp;
  }
  listSize--;
}

template <typename T>
void LinkedList<T>::delID(int ID){
  bool found{false};
  if(front_ptr == nullptr){return;}
  else{
    node_ptr temp = new node_type;
    node_ptr prev = new node_type;
    temp = front_ptr;
    prev = temp;
    if(front_ptr->ID == ID){delFront();}
    while(found == false && temp->next != nullptr){
      if(temp->next->ID == ID){
	prev = temp;
	temp = temp->next;
	prev->next = temp->next;
	delete temp;
	found = true;
      }
      else{
	prev = temp;
	temp = temp->next;
      }
    }
  }
  listSize--;
}

template <typename T>
Node<T>* LinkedList<T>::getNodebyID(int ID){
  if(front_ptr == nullptr){return 0;}
  else{
    node_ptr temp = new node_type;
    temp = front_ptr;
    while(temp->next != nullptr){
      if(temp->ID == ID){
	return temp;
      }
      temp = temp->next;
    }
    return 0;
  }
}

template <typename T>
int LinkedList<T>::size(){return listSize;}

template <typename T>
Node<T>* LinkedList<T>::getNodei(int i){
  node_ptr temp = new node_type;
  temp = front_ptr;
  for(int j = 1; j < i; j++){
    temp = temp->next;
  }
  return temp;
}

template <typename T>
Node<T>* LinkedList<T>::getNodebyPos(int x, int y){
  node_ptr temp = new node_type;
  temp = front_ptr;
  if(temp->data.getX() == x && temp->data.getY() == y)
    return temp;
  for(int i=1; i <= listSize-1; i++){
    temp = temp->next;
    if(temp->data.getX() == x && temp->data.getY() == y){
      return temp;
    }
  }
}

#endif //LINKED_LIST
