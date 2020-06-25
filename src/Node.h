#ifndef NODE_H
#define NODE_H

template <typename T>
class Node{
 public:
  typedef T type_value;
  
  type_value data;
  const char* name;
  int ID;
  Node* next;
 public: 
  Node(type_value object, const char* name);
  Node(){};
};

template <typename T>
Node<T>::Node(type_value object,const char* name){
  this->data = object;
  this->name = name;
  this->next = nullptr;
}

#endif //NODE_H
