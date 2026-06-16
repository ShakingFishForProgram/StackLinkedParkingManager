#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <iostream>

using namespace std;

template <typename T>
class Node{
    public:
        T value;
        Node* prev;
        Node* next;

        Node(const T& value=T(), Node* prev=nullptr, Node* next=nullptr ) {
            this->value = value;
            this->prev = prev;
            this->next = next;
        }
};

template <typename T>
class DoublyLinkedList {
    private:
        Node<T>* firstNode;
        int size;

        bool checkIndex(int index) {
            return ((index < this->size) && (index >= 0));
        }

    public:
        DoublyLinkedList() {
            this->firstNode = nullptr;
            this->size = 0;
        }

        DoublyLinkedList(const DoublyLinkedList<T>& otherObj) {
            this->firstNode = nullptr;
            this->size = 0;

            Node<T>* currentOtherObjNode = otherObj.firstNode;
            Node<T>* currentThisNode = nullptr;
            Node<T>* prevThisNode = nullptr;

            while(currentOtherObjNode != nullptr) {
                currentThisNode = new Node<T>(currentOtherObjNode->value, prevThisNode, nullptr);
                if(prevThisNode == nullptr) {
                    this->firstNode = currentThisNode;
                } else {
                    prevThisNode->next = currentThisNode;
                }
                prevThisNode = currentThisNode;
                currentOtherObjNode = currentOtherObjNode->next;
            }

            this->size = otherObj.size;

        }

        DoublyLinkedList<T>& operator= (const DoublyLinkedList<T>& otherObj) {
            if (this == & otherObj) {
                return *this;
            }

            if(this->firstNode != nullptr) {
                Node<T>* currentDeleteNode = this->firstNode;
                Node<T>* nextDeleteNode = this->firstNode->next;
                while(true) {
                    delete currentDeleteNode;
                    currentDeleteNode = nextDeleteNode;
                    if (currentDeleteNode == nullptr) {
                        break;
                    }
                    nextDeleteNode = nextDeleteNode->next;
                }
            }

            this->firstNode = nullptr;
            this->size = 0;

            Node<T>* currentOtherObjNode = otherObj.firstNode;
            Node<T>* currentThisNode = nullptr;
            Node<T>* prevThisNode = nullptr;

            while(currentOtherObjNode != nullptr) {
                currentThisNode = new Node<T>(currentOtherObjNode->value, prevThisNode, nullptr);
                if(prevThisNode == nullptr) {
                    this->firstNode = currentThisNode;
                } else {
                    prevThisNode->next = currentThisNode;
                }
                prevThisNode = currentThisNode;
                currentOtherObjNode = currentOtherObjNode->next;
            }

            this->size = otherObj.size;

            return *this;
        }

        ~DoublyLinkedList(){
            if(this->firstNode != nullptr) {
                Node<T>* currentDeleteNode = this->firstNode;
                Node<T>* nextDeleteNode = this->firstNode->next;
                while(true) {
                    delete currentDeleteNode;
                    currentDeleteNode = nextDeleteNode;
                    if (currentDeleteNode == nullptr) {
                        break;
                    }
                    nextDeleteNode = nextDeleteNode->next;
                }
            }
        }

        bool isEmpty() const {
            return size == 0;
        }

        int getSize() const {
            return this->size;
        }

        bool get(int index, T& element) {
            if(!this->checkIndex(index)) {
                return false;
            }
            int currentIndex = 0;
            Node<T>* currentNode = this->firstNode;
            while (currentIndex < index) {
                currentNode = currentNode->next;
                currentIndex ++;
            }
            element = currentNode->value;
            return true;
        }

        int indexOf(const T& element) const {
            Node<T>* currentNode = this->firstNode;
            int currentIndex = 0;
            while (currentNode != nullptr) {
                if(currentNode->value == element) {
                    break;
                }
                currentNode= currentNode->next;
                currentIndex ++;
            }
            if(currentNode != nullptr) {
                if(currentNode->value == element) {
                    return currentIndex;
                }
            }
            return -1;
        }

        bool erase(int index) {
            if(!this->checkIndex(index)) {
                return false;
            }

            if (index == 0) {
                Node<T>* targetNode = this->firstNode;
                this->firstNode = this->firstNode->next;
                if(this->firstNode != nullptr) {
                    this->firstNode->prev = nullptr;
                }
                delete targetNode;
            } else {
                Node<T>* currentNode = this->firstNode;
                int currentIndex = 0;
                while (currentIndex < index) {
                    currentNode = currentNode->next;
                    currentIndex ++;
                }

                Node<T>* targetNode = currentNode;

                Node<T>* prevNode = targetNode->prev;
                Node<T>* nextNode = targetNode->next;

                if (prevNode != nullptr) {
                    prevNode->next = nextNode;
                }

                if (nextNode != nullptr) {
                    nextNode->prev = prevNode;
                }
                delete targetNode;
            }

            size --;
            return true;
        }

        bool insert (int index, const T& element) {
            if(index < 0 || index > size) {
                return false;
            }

            Node<T>* newNode = new Node<T>(element);

            if(index == 0) {
                newNode->next = this->firstNode;
                this->firstNode = newNode;
                if(this->firstNode->next != nullptr) {
                    this->firstNode->next->prev = this->firstNode;
                }
            } else {
                Node<T>* currentNode = this->firstNode;
                int currentIndex = 0;
                while (currentIndex < index-1) {
                    currentNode = currentNode->next;
                    currentIndex ++;
                }

                Node<T>* prevNode = currentNode;

                Node<T>* nextNode = prevNode->next;

                if(prevNode != nullptr) {
                    prevNode->next = newNode;
                }

                newNode->next = nextNode;
                newNode->prev = prevNode;

                if(nextNode != nullptr) {
                    nextNode->prev = newNode;
                }
            }

            size ++;

            return true;
        }

        void showDoublyLinkedList() const {
            if (this->size == 0) {
                cout << "Empyt Doubly Linked List!";
            } else {
                Node<T>* currentNode = this->firstNode;
                while (currentNode != nullptr) {
                    cout << currentNode->value << " ";
                    currentNode = currentNode->next;
                }
            }
        }



};


#endif