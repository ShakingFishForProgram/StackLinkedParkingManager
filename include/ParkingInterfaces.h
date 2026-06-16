#ifndef PARKINGINTERFACES_H
#define PARKINGINTERFACES_H

#include <string>

using namespace std;

struct CarInfo {
    string id;
    int totalTime;
    double price;

    CarInfo(string id="", int totalTime=0) {
        this->id = id;
        this->totalTime = totalTime;
    }
};

struct ParkingInterface {
    CarInfo* container;
    int size;

    ParkingInterface(int size) {
        this->size = size;
        this->container = new CarInfo[this->size];
    }

    ParkingInterface(const ParkingInterface& otherObj) {
        this->size = otherObj.size;
        this->container = new CarInfo[this->size];

        for(int i=0; i<this->size; i++) {
            this->container[i] = otherObj.container[i];
        }
    }

    ParkingInterface& operator= (const ParkingInterface& otherObj) {
        if (this == & otherObj) {
            return *this;
        }

        delete [] this->container;

        this->size = otherObj.size;
        this->container = new CarInfo[this->size];

        for(int i=0; i<this->size; i++) {
            this->container[i] = otherObj.container[i];
        }

        return *this;
    }

    ~ParkingInterface() {
        delete [] this->container;
    }

};

struct ParkingDualStackInterface : public ParkingInterface {
    const string _STRUCT_NAME = "Dual Stack";

    ParkingDualStackInterface(int size) : ParkingInterface(size) {}
};

struct ParkingDoublyLinkedListInterface : public ParkingInterface {
    const string _STRUCT_NAME = "Doubly Linked List";

    ParkingDoublyLinkedListInterface(int size) : ParkingInterface(size) {}
};


#endif
