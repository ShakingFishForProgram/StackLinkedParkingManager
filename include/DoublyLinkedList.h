#ifndef DOUBLYLINKEDLIST_H
#define DOUBLYLINKEDLIST_H

#include <iostream>
#include <string>

using namespace std;

// 双向链表节点模板
// 每个节点包含数据、前驱指针和后继指针
template <typename T>
class Node{
    public:
        T value;             // 节点存储的数据
        Node* prev;          // 指向前一个节点的指针
        Node* next;          // 指向后一个节点的指针

        // 构造函数：初始化数据和前后指针
        Node(const T& value=T(), Node* prev=nullptr, Node* next=nullptr ) {
            this->value = value;
            this->prev = prev;
            this->next = next;
        }
};

// 双向链表模板类
// 支持按索引增删查操作，尾部追加
template <typename T>
class DoublyLinkedList {
    protected:
        Node<T>* firstNode;  // 头节点指针（第一个节点）
        Node<T>* lastNode; // 尾节点指针
        int size;            // 链表当前大小

        // 检查索引是否有效（0 ≤ index < size）
        bool checkIndex(int index) {
            return ((index < this->size) && (index >= 0));
        }

    public:
        // 构造函数：创建空链表
        DoublyLinkedList() {
            this->firstNode = nullptr;
            this->lastNode = nullptr;
            this->size = 0;
        }

        // 拷贝构造函数：深拷贝整个链表
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
            this->lastNode = prevThisNode;

            this->size = otherObj.size;

        }

        // 赋值运算符：深拷贝
        DoublyLinkedList<T>& operator= (const DoublyLinkedList<T>& otherObj) {
            if (this == & otherObj) {
                return *this;
            }

            // 释放原有节点
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

            // 复制新节点
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
            this->lastNode = prevThisNode;

            this->size = otherObj.size;

            return *this;
        }

        // 析构函数：释放所有节点
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

        // 判断链表是否为空
        bool isEmpty() const {
            return size == 0;
        }

        // 返回链表当前大小
        int getSize() const {
            return this->size;
        }

        // 获取指定索引处的元素，存入 element，成功返回 true
        bool get(int index, T& element) {
            if(!this->checkIndex(index)) {
                return false;
            }
            if(index < this->size / 2) {
                int currentIndex = 0;
                Node<T>* currentNode = this->firstNode;
                while (currentIndex < index) {
                    currentNode = currentNode->next;
                    currentIndex ++;
                }
                element = currentNode->value;
            } else {
                int currentIndex = this->size-1;
                Node<T>* currentNode = this->lastNode;
                while (currentIndex > index) {
                    currentNode = currentNode->prev;
                    currentIndex --;
                }
                element = currentNode->value;
            }
            return true;
        }

        // 查找元素在链表中的索引，不存在返回 -1
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

        // 删除指定索引处的节点，成功返回 true
        bool erase(int index) {
            if(!this->checkIndex(index)) {
                return false;
            }

            if (this->size == 1) {
                Node<T>* targetNode = this->firstNode;
                this->firstNode = nullptr;
                this->lastNode = nullptr;

                delete targetNode;
            } else if (index == 0) {
                // 删除头节点
                Node<T>* targetNode = this->firstNode;
                this->firstNode = this->firstNode->next;
                this->firstNode->prev = nullptr;
                delete targetNode;
            } else if (index == this->size-1) {
                // 删除尾节点
                Node<T>* targetNode = this->lastNode;
                this->lastNode = this->lastNode->prev;
                this->lastNode->next = nullptr;
                delete targetNode;
            } else {
                // 删除中间节点（该节点一定有前驱节点和后继结点）
                Node<T>* currentNode = this->firstNode;
                int currentIndex = 0;

                if (index < this->size/2) {
                    while (currentIndex < index) {
                        currentNode = currentNode->next;
                        currentIndex ++;
                    }
                } else {
                    currentNode = this->lastNode;
                    currentIndex = this->size-1;
                    while (currentIndex > index) {
                        currentNode = currentNode->prev;
                        currentIndex --;
                    }
                }

                Node<T>* targetNode = currentNode;
                Node<T>* prevNode = targetNode->prev;
                Node<T>* nextNode = targetNode->next;

                // 前驱节点指向后继节点
                if (prevNode != nullptr) {
                    prevNode->next = nextNode;
                }
                // 后继节点指向前驱节点
                if (nextNode != nullptr) {
                    nextNode->prev = prevNode;
                }
                delete targetNode;
            }

            size --;
            return true;
        }

        // 在指定索引处插入元素，成功返回 true
        bool insert (int index, const T& element) {
            if(index < 0 || index > size) {
                return false;
            }

            Node<T>* newNode = new Node<T>(element);

            if (this->isEmpty()) {
                this->firstNode = newNode;
                this->lastNode = newNode;
            } else if(index == 0) {
                // 节点在头部插入
                newNode->next = this->firstNode;
                this->firstNode->prev = newNode;
                this->firstNode = newNode;
            } else if(index == size) {
                // 节点在尾部插入
                newNode->prev = this->lastNode;
                this->lastNode->next = newNode;
                this->lastNode = newNode;
            } else {
                // 节点在中间插入（插入该节点之前index位置有节点且该节点一定有前驱节点）
                Node<T>* currentNode = this->firstNode;
                int currentIndex = 0;

                if (index < this->size / 2) {
                    while (currentIndex < index) {
                        currentNode = currentNode->next;
                        currentIndex ++;
                    }
                } else {
                    currentNode = this->lastNode;
                    int currentIndex = this->size-1;
                    while (currentIndex > index) {
                        currentNode = currentNode->prev;
                        currentIndex --;
                    }
                }

                Node<T>* nextNode = currentNode;
                Node<T>* prevNode = nextNode->prev;
                newNode->next = nextNode;
                newNode->prev = prevNode;
                prevNode->next = newNode;
                nextNode->prev = newNode;


            }

            this->size ++;
            return true;
        }

        // 追加元素到链表末尾
        bool push(const T& element) {
            return this->insert(this->size, element);
        }

        // 显示链表所有元素
        void showDoublyLinkedList() const {
            if (this->size == 0) {
                cout << "Empty Doubly Linked List!";
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
