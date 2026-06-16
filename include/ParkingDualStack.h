#ifndef PARKINGDUALSTACK_H
#define PARKINGDUALSTACK_H

#include "DualStack.h"
#include "Car.h"
#include "ParkingInterfaces.h"

class ParkingDualStack : public DualStack<Car> {
    public:
        ParkingDualStack(int size) : DualStack<Car> (size){}

        ParkingDualStack(const ParkingDualStack& otherObj) : DualStack<Car>(otherObj) {}

        ParkingDualStack& operator= (const ParkingDualStack& otherObj) {
            if(this == &otherObj) {
                return *this;
            }

            DualStack<Car>::operator=(otherObj);
            return *this;
        }

        bool pushDualStack(Car& obj) {
            obj.inPark();
            if (DualStack<Car>::pushDualStack(obj)) {
                return true;
            } else {
                return false;
            }
        }

        bool popDualStack(string value, Car& obj) {
            if (DualStack<Car>::popDualStack(value, obj)) {
                obj.outPark();
                return true;
            } else {
                return false;
            }
        }

        ParkingDualStackInterface& getParkingMessage() {
            ParkingDualStackInterface* pdsi = new ParkingDualStackInterface(this->leftNumber);
            for(int i=0; i<this->leftNumber; i++) {
                pdsi->container[i].id = this->container[i].getId();
                this->container[i].markMaintainingTimeStamp();
                pdsi->container[i].totalTime = this->container[i].getMaintainingTime();
            }

            return *pdsi;
        }

        bool addExtraTime(int seconds=0, int minutes=0, int hours=0) {
            for(int i=0; i<this->leftNumber; i++) {
                int extraTime = 3600*hours + 60*minutes + seconds;
                int originalTime = this->container[i].getExtraTime();
                this->container[i].setExtraTime(originalTime + extraTime);
            }

            for(int i=0; i<this->rightNumber; i++) {
                int extraTime = 3600*hours + 60*minutes + seconds;
                int originalTime = this->container[this->size-1-i].getExtraTime();
                this->container[this->size-1-i].setExtraTime(originalTime + extraTime);
            }
            return true;
        }

};

#endif