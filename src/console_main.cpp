#include <iostream>
#include <string>
#include <sstream>
#include "Car.h"
#include "ParkingStructure.h"
#include "ParkingInterfaces.h"

using namespace std;

void menu();
bool carEntry(ParkingStructure& ps);
bool carLeave(ParkingStructure& ps, Car& leftCar);
bool addTime (ParkingStructure& ps);
void showParkingInfo(ParkingStructure& ps);
void setUnitPrice(ParkingStructure& ps);
void printCarInfo(const Car& car, ParkingStructure& ps);
int getIntInput();

int main() {
    int stackSize = 0;
    cout << "Enter the space of the parking stack: ";
    stackSize = getIntInput();

    int unitPrice = 0;
    cout << "Enter unit price: ";
    unitPrice = getIntInput();

    ParkingStructure* ps = new ParkingStructure(stackSize, unitPrice);

    int choice = 0;
    while (choice != 6) {
        menu();
        choice = getIntInput();

        if (choice == 1) {
            carEntry(*ps);
        } else if (choice == 2) {
            Car leftCar("");
            if (carLeave(*ps, leftCar)) {
                printCarInfo(leftCar, *ps);
            } else {
                cout << "Car doesn't exist!" << endl;
            }
        } else if (choice == 3) {
            addTime(*ps);
        } else if (choice == 4) {
            showParkingInfo(*ps);
        } else if (choice == 5) {
            setUnitPrice(*ps);
        } else if (choice != 6) {
            cout << "Invalid choice! Please enter 1-6." << endl;
        }
    }
    delete ps;
}

int getIntInput() {
    string input;
    while (true) {
        getline(cin, input);
        if (input.empty()) continue;
        stringstream ss(input);
        int value;
        if (ss >> value && ss.eof()) {
            return value;
        }
        cout << "Invalid input! Please enter a number: ";
    }
}

void menu() {
    cout << "\n\n\n\n\n";
    cout << "========================================" << endl;
    cout << "    Parking Management System" << endl;
    cout << "========================================" << endl;
    cout << "  1. Car Entry" << endl;
    cout << "  2. Car Leave" << endl;
    cout << "  3. Add Time" << endl;
    cout << "  4. Show Parking Info" << endl;
    cout << "  5. Set Unit Price" << endl;
    cout << "  6. Exit" << endl;
    cout << "========================================" << endl;
    cout << "  Enter your choice: ";
}

void printCarInfo(const Car& car, ParkingStructure& ps) {
    cout << "  Plate: " << car.getId() << endl;
    cout << "  Parking time: " << car.getPakingTime() << "s" << endl;
    cout << "  Fee: " << ps.calculatePrice(car.getPakingTime()) << "yuan" << endl;
}

bool carEntry(ParkingStructure& ps) {
    cout << "Enter your car's id: ";
    string carName;
    getline(cin, carName);

    Car newCar(carName);
    ps.insertPark(newCar);
    return true;
}

bool carLeave(ParkingStructure& ps, Car& leftCar) {
    cout << "Enter your car's id: ";
    string carName;
    getline(cin, carName);

    bool isCarExist = ps.leavePark(carName, leftCar);
    return isCarExist;
}

bool addTime(ParkingStructure& ps) {
    cout << "Enter the time you want to add" << endl;
    int hour, minute, second;
    cout << "hours: ";
    hour = getIntInput();
    cout << "minutes: ";
    minute = getIntInput();
    cout << "seconds: ";
    second = getIntInput();

    ps.addExtraTime(second, minute, hour);
    return true;
}

void showParkingInfo(ParkingStructure& ps) {
    cout << "========================================" << endl;
    cout << "         System Status" << endl;
    cout << "========================================" << endl;
    cout << "--- Parking Stack ---" << endl;
    ParkingDualStackInterface pdsi = ps.getStackMessage();
    if (pdsi.size == 0) {
        cout << "  (empty)" << endl;
    } else {
        for (int i = 0; i < pdsi.size; i++) {
            cout << "  [" << i << "] Plate: " << pdsi.container[i].id
                 << " | Time: " << pdsi.container[i].totalTime << "s"
                 << " | Fee: " << pdsi.container[i].price << " yuan" << endl;
        }
    }
    cout << endl;
    cout << "--- Waiting List ---" << endl;
    ParkingDoublyLinkedListInterface pdlli = ps.getLinkedListMessage();
    if (pdlli.size == 0) {
        cout << "  (empty)" << endl;
    } else {
        for (int i = 0; i < pdlli.size; i++) {
            cout << "  [" << i << "] Plate: " << pdlli.container[i].id << endl;
        }
    }
    cout << endl;
    cout << "Unit Price: " << ps.getUnitPrice() << " yuan/hour" << endl;
    cout << "========================================" << endl;
}

void setUnitPrice(ParkingStructure& ps) {
    cout << "Enter the unit price: ";
    int price = getIntInput();
    ps.setUnitPrice(price);
}
