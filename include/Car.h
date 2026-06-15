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

        Car() {
            this->id = "";
            this->isInPark = false;
            this->inStackTime = 0;
            this->outStackTime = 0;
        }

        void inPark() {
            this->isInPark = true;
            this->inStackTime = time(nullptr);
        }

        void outPark() {
            this->outStackTime = time(nullptr);
            this->isInPark = false;
        }

        time_t getPakingTime() const {
            return (this->outStackTime - this->inStackTime);
        }

        bool operator== (const Car& otherObj) const {
            return (this->id == otherObj.getId());
        }

        bool operator== (const string& otherObj) const {
            return (this->id == otherObj);
        }

        string getId() const {
            return this->id;
        }

        friend ostream& operator<<(ostream& os, const Car& car) {
            os << car.getId();
            return os;
        }

};

#endif // CAR_H
