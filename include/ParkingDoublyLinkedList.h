#ifndef PARKINGDOUBLYLINKEDLIST_H
#define PARKINGDOUBLYLINKEDLIST_H

#include <string>
#include "Car.h"
#include "DoublyLinkedList.h"
#include "ParkingInterfaces.h"

class ParkingDoublyLinkedList : public DoublyLinkedList<Car> {
    public:
        int indexOfString(const string key) {
            Node<Car>* currentNode = this->firstNode;
            int currentIndex = 0;
            while (currentNode != nullptr) {
                if(currentNode->value.getId() == key) {
                    break;
                }
                currentNode= currentNode->next;
                currentIndex ++;
            }
            if(currentNode != nullptr) {
                if(currentNode->value.getId() == key) {
                    return currentIndex;
                }
            }
            return -1;
        }

        ParkingDoublyLinkedListInterface& getPakingMessage() {
            ParkingDoublyLinkedListInterface* pdll = new ParkingDoublyLinkedListInterface(this->size);
            Node<Car>* currentNode = this->firstNode;
            for(int i=0; i<this->size; i++) {
                pdll->container[i].id = currentNode->value.getId();
                pdll->container[i].totalTime = currentNode->value.getPakingTime();
                pdll->container[i].calculatePrice();

                currentNode = currentNode->next;
            }

            return *pdll;
        }
};



#endif