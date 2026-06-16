#ifndef PARKINGSTRUCTURE_H
#define PARKINGSTRUCTURE_H

#include <string>
#include <iostream>
#include "Car.h"
#include "ParkingDualStack.h"
#include "ParkingDoublyLinkedList.h"
#include "ParkingInterfaces.h"

class ParkingStructure {
    private:
        ParkingDualStack* pds;
        ParkingDoublyLinkedList* pdll;

        double unitPrice;

        int size;
        int numberForParkingDualStack;
        int numberForDoublyLinkedList;

    public:
        ParkingStructure(int size, double unitPrice=1) {
            this->size = size;
            this->unitPrice = unitPrice;

            pds = new ParkingDualStack(this->size);
            pdll = new ParkingDoublyLinkedList();

            this->numberForParkingDualStack = 0;
            this->numberForDoublyLinkedList = 0;
        }

        ParkingStructure(const ParkingStructure& otherObj) {
            this->size = otherObj.size;
            this->unitPrice = otherObj.unitPrice;

            this->pds = new ParkingDualStack(*otherObj.pds);
            this->pdll = new ParkingDoublyLinkedList(*otherObj.pdll);

            this->numberForParkingDualStack = otherObj.numberForParkingDualStack;
            this->numberForDoublyLinkedList = otherObj.numberForDoublyLinkedList;
        }

        ParkingStructure& operator= (const ParkingStructure& otherObj) {
            if (this == &otherObj) {
                return *this;
            }

            delete pds;
            delete pdll;

            this->size = otherObj.size;
            this->unitPrice = otherObj.unitPrice;

            this->pds = new ParkingDualStack(*otherObj.pds);
            this->pdll = new ParkingDoublyLinkedList(*otherObj.pdll);

            this->numberForParkingDualStack = otherObj.numberForParkingDualStack;
            this->numberForDoublyLinkedList = otherObj.numberForDoublyLinkedList;

            return *this;
        }

        ~ParkingStructure() {


            delete pds;
            delete pdll;
        }


        bool insertPark (Car& insertCar) {

            if(!pds->isFull()) { // 第一步：检查栈是不是满的，如果不满，直接入栈
                pds->pushDualStack(insertCar);
            } else { // 第二步：如果栈是满的，则进入双向链表
                pdll->push(insertCar);
            }

            return true;
        }

        bool leavePark(string key, Car& leftCar) {
            bool isExistInStack = false;
            // 如果双栈里面存在该元素
            isExistInStack = pds->popDualStack(key, leftCar);
            if(isExistInStack && (!pdll->isEmpty())) {
                Car* targetCar = new Car("");
                pdll->get(0, *targetCar);
                pdll->erase(0);
                pds->pushDualStack(*targetCar);
                delete targetCar;
                return true;
            } else {
                // 如果双栈里面不存在该元素，查找双向链表
                int indexOfCar = pdll->indexOfString(key);
                if(indexOfCar >= 0) {
                    pdll->get(indexOfCar, leftCar);
                    pdll->erase(indexOfCar);
                    return true;
                }
                return true;
            }

            return false;
        }

        void showPark() const {
            cout << "Stack: \n";
            pds->showLeftStack();
            cout << "\nLinkedList: \n";
            pdll->showDoublyLinkedList();
        }

        bool addExtraTime(int seconds=0, int minutes=0, int hours=0) {
            this->pds->addExtraTime(seconds, minutes, hours);
            this->pdll->addExtraTime(seconds, minutes, hours);

            return true;
        }

        void setUnitPrice(double unitPrice) {
            this->unitPrice = unitPrice;
        }

        double getUnitPrice() {
            return this->unitPrice;
        }

        void calculateTotalPrice(ParkingInterface& pi) {
            for(int i=0; i<pi.size; i++) {
                pi.container[i].price = (double)pi.container[i].totalTime * this->unitPrice / 3600;
            }
        }

        double calculatePrice(time_t time) {
            return time * this->unitPrice / 3600;
        }

        ParkingDualStackInterface& getStackMessage() {
            ParkingDualStackInterface* msg = new ParkingDualStackInterface(this->pds->getParkingMessage());
            this->calculateTotalPrice(*msg);
            return *msg;
        }

        ParkingDoublyLinkedListInterface& getLinkedListMessage() {
            ParkingDoublyLinkedListInterface* msg = new ParkingDoublyLinkedListInterface(this->pdll->getParkingMessage());
            for(int i=0; i<msg->size; i++) {
                msg->container[i].price = 0;
            }
            return *msg;
        }

};


#endif