#include "List.h"

#include "Log.h"

List::List() {
  head = 0;
  tail = 0;
  length = 0;
}

Node* List::createNode(void* data) {
  Node* node = new Node;
  node->data = data;
  node->next = 0;
  node->prev = 0;
  return node;
}

void List::insertStart(void* data) {
  Node* node = List::createNode(data);
  node->next = head;
  head->prev = node;
  head = node;
  length++;
}

void List::insertEnd(void* data) {
  Node* node = List::createNode(data);
  if(head == 0) {
    head = node;
    tail = node;
  } else {
    tail->next = node;
    node->prev = tail;
    tail = node;
  }
  length++;
}

void List::deleteStart() {
  Node* temp = head;
  head = temp->next;
  head->prev = 0;
  length--;
  delete temp;
}

void List::deleteEnd() {
  Node* temp = tail;
  tail = temp->prev;
  tail->next = 0;
  length--;
  delete temp;
}

Iterator* List::getIteratorStart() {
  return new Iterator(head);
}

Iterator* List::getIteratorEnd() {
  return new Iterator(tail);
}

Iterator::Iterator(Node* head) {
  curr = head;
  start = 1;
}

void* Iterator::next() {
  if(start) {
    start = 0;
    return curr;
  }
  curr = curr->next;
  return curr->data;
}

void* Iterator::prev() {
  if(start) {
    start = 0;
    return curr;
  }
  curr = curr->prev;
  return curr->data;
}
