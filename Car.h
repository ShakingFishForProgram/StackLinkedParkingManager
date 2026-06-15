#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <string>
#include <ctime>

using namespace std;

class Car {
    private:
        string id;
        bool isInPark;
        time_t inStackTime;
        time_t outStackTime;

    public:
        Car(string id) {
            this->id = id;
            this->isInPark = false;
            this->inStackTime = 0;
            this->outStackTime = 0;
        }

        void inPark() {
            this->isInPark = true;
            this->inStackTime = time(nullptr);
        }

        void outPark() {
            this->isInPark = false;
            this->outStackTime = time(nullptr);
        }

        time_t getPakingTime() {
            return (this->outStackTime - this->inStackTime);
        }

        friend ostream& operator<<(ostream& os, const Car& car) {
            os << "ID: " << car.id
               << "\nStatus: " << (car.isInPark ? "In Park" : "Out of Park")
               << "\nEntry Timestamp: " << car.inStackTime
               << "\nExit Timestamp: " << car.outStackTime;
            return os;
        }

};

#endif // CAR_H
