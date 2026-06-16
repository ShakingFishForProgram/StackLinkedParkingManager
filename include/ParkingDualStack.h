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

        ParkingDualStackInterface& getPakingMessage() {
            ParkingDualStackInterface* pdsi = new ParkingDualStackInterface(this->leftNumber);
            for(int i=0; i<this->leftNumber; i++) {
                pdsi->container[i].id = this->container[i].getId();
                pdsi->container[i].totalTime = this->container[i].getPakingTime();
                pdsi->container[i].calculatePrice();
            }

            return *pdsi;
        }

};

#endif