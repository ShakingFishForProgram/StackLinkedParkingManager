#ifndef PARKINGDUALSTACK_H
#define PARKINGDUALSTACK_H

#include "DualStack.h"
#include "Car.h"

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

};

#endif