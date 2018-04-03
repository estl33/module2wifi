#ifndef LIST_H_
#define LIST_H_

struct Node {
  void* data;
  Node* next;
  Node* prev;
};

class Iterator;

class List {
  Node* head;
  Node* tail;
public:
  List();
  int length;
  void insertStart(void* data);
  void insertEnd(void* data);
  void deleteStart();
  void deleteEnd();
  Iterator* getIteratorStart();
  Iterator* getIteratorEnd();
private:
  Node* createNode(void* data);
};

class Iterator {
  friend class List;
  int start;
  Node* curr;
public:
  void* next();
  void* prev();
private:
  Iterator(Node* head);
};

#endif
