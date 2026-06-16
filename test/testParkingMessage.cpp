#include <iostream>
#include <string>
#include <windows.h>
#include "ParkingStructure.h"

using namespace std;

void printSeparator() {
    cout << "----------------------------------------" << endl;
}

void printCarInfo(CarInfo& info) {
    cout << "  ID: " << info.id
         << ", Time: " << info.totalTime << "s"
         << ", Price: " << info.price << " yuan" << endl;
}

double getPriceFromStackMsg(ParkingDualStackInterface& msg, const string& key) {
    for (int i = 0; i < msg.size; i++) {
        if (msg.container[i].id == key) {
            return msg.container[i].price;
        }
    }
    return -1;
}

double getPriceFromListMsg(ParkingDoublyLinkedListInterface& msg, const string& key) {
    for (int i = 0; i < msg.size; i++) {
        if (msg.container[i].id == key) {
            return msg.container[i].price;
        }
    }
    return -1;
}

int main() {

    printSeparator();
    cout << "Prepare: Create ParkingStructure with capacity 5, unitPrice=5" << endl;
    ParkingStructure ps(5, 5);

    Car carA("YueA0001");
    Car carB("JingB0002");
    Car carC("HuC0003");
    Car carD("SuD0004");
    Car carE("MinE0005");
    Car carF("LuF0006");
    Car carG("YuG0007");

    Car leftCar("");

    // =============================================
    // Phase 1: 基础入场 + 信息查询
    // =============================================

    printSeparator();
    cout << "Test 1: Enter 5 cars (A, B, C, D, E) - park full" << endl;
    ps.insertPark(carA);
    ps.insertPark(carB);
    ps.insertPark(carC);
    ps.insertPark(carD);
    ps.insertPark(carE);
    ps.showPark();
    cout << endl;

    cout << "Sleep 10 seconds..." << endl;
    Sleep(10000);

    printSeparator();
    cout << "Test 2: Enter 2 cars (F, G) - waiting list" << endl;
    ps.insertPark(carF);
    ps.insertPark(carG);
    ps.showPark();
    cout << endl;

    cout << "Sleep 5 seconds..." << endl;
    Sleep(5000);

    printSeparator();
    cout << "Test 3: Add 1 hour + print both structures" << endl;
    ps.addExtraTime(0, 0, 1);
    cout << "--- Stack Info ---" << endl;
    ParkingDualStackInterface& stackMsg1 = ps.getStackMessage();
    for (int i = 0; i < stackMsg1.size; i++) {
        cout << "  Car " << i << ": ";
        printCarInfo(stackMsg1.container[i]);
    }
    cout << "--- Waiting List Info ---" << endl;
    ParkingDoublyLinkedListInterface& listMsg1 = ps.getLinkedListMessage();
    cout << "  Waiting list size: " << listMsg1.size << endl;
    for (int i = 0; i < listMsg1.size; i++) {
        cout << "  Car " << i << ": ";
        printCarInfo(listMsg1.container[i]);
    }
    cout << "  (waiting cars: time=0, price=0)" << endl;
    cout << endl;

    cout << "Sleep 5 seconds..." << endl;
    Sleep(5000);

    printSeparator();
    cout << "Test 4: Add another 1 hour + print both structures" << endl;
    ps.addExtraTime(0, 0, 1);
    cout << "--- Stack Info ---" << endl;
    ParkingDualStackInterface& stackMsg2 = ps.getStackMessage();
    for (int i = 0; i < stackMsg2.size; i++) {
        cout << "  Car " << i << ": ";
        printCarInfo(stackMsg2.container[i]);
    }
    cout << "--- Waiting List Info ---" << endl;
    ParkingDoublyLinkedListInterface& listMsg2 = ps.getLinkedListMessage();
    cout << "  Waiting list size: " << listMsg2.size << endl;
    for (int i = 0; i < listMsg2.size; i++) {
        cout << "  Car " << i << ": ";
        printCarInfo(listMsg2.container[i]);
    }
    cout << endl;

    printSeparator();
    cout << "Test 5: Print both structures (no extra time added)" << endl;
    cout << "--- Stack Info ---" << endl;
    ParkingDualStackInterface& stackMsg3 = ps.getStackMessage();
    for (int i = 0; i < stackMsg3.size; i++) {
        cout << "  Car " << i << ": ";
        printCarInfo(stackMsg3.container[i]);
    }
    cout << "--- Waiting List Info ---" << endl;
    ParkingDoublyLinkedListInterface& listMsg3 = ps.getLinkedListMessage();
    cout << "  Waiting list size: " << listMsg3.size << endl;
    for (int i = 0; i < listMsg3.size; i++) {
        cout << "  Car " << i << ": ";
        printCarInfo(listMsg3.container[i]);
    }
    cout << endl;

    // =============================================
    // Phase 2: 离场 + 便道补入 + 延时计费
    // =============================================

    cout << "Sleep 3 seconds..." << endl;
    Sleep(3000);

    printSeparator();
    cout << "Test 6: Get B price -> Leave B (middle) -> F enters park" << endl;
    ParkingDualStackInterface& msgForB = ps.getStackMessage();
    double bPrice = getPriceFromStackMsg(msgForB, "JingB0002");
    bool result = ps.leavePark("JingB0002", leftCar);
    cout << "  Result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "  Left car: " << leftCar << endl;
    cout << "  Parking time: " << leftCar.getPakingTime() << "s" << endl;
    cout << "  Price: " << bPrice << " yuan" << endl;
    ps.showPark();
    cout << endl;

    cout << "Sleep 5 seconds..." << endl;
    Sleep(5000);

    printSeparator();
    cout << "Test 7: Add 1 hour + print both structures (F now in park)" << endl;
    ps.addExtraTime(0, 0, 1);
    cout << "--- Stack Info ---" << endl;
    ParkingDualStackInterface& stackMsg4 = ps.getStackMessage();
    for (int i = 0; i < stackMsg4.size; i++) {
        cout << "  Car " << i << ": ";
        printCarInfo(stackMsg4.container[i]);
        if (stackMsg4.container[i].id == "LuF0006") {
            cout << "       (F entered park recently, time ~1h from stack)" << endl;
        }
    }
    cout << "--- Waiting List Info ---" << endl;
    ParkingDoublyLinkedListInterface& listMsg4 = ps.getLinkedListMessage();
    cout << "  Waiting list size: " << listMsg4.size << endl;
    for (int i = 0; i < listMsg4.size; i++) {
        cout << "  Car " << i << ": ";
        printCarInfo(listMsg4.container[i]);
    }
    cout << endl;

    cout << "Sleep 3 seconds..." << endl;
    Sleep(3000);

    printSeparator();
    cout << "Test 8: Get F price -> Leave F from stack" << endl;
    ParkingDualStackInterface& msgForF = ps.getStackMessage();
    double fPrice = getPriceFromStackMsg(msgForF, "LuF0006");
    result = ps.leavePark("LuF0006", leftCar);
    cout << "  Result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "  Left car: " << leftCar << endl;
    cout << "  Parking time: " << leftCar.getPakingTime() << "s (includes stack time)" << endl;
    cout << "  Price: " << fPrice << " yuan" << endl;
    ps.showPark();
    cout << endl;

    cout << "Sleep 3 seconds..." << endl;
    Sleep(3000);

    printSeparator();
    cout << "Test 9: Add 1 hour" << endl;
    ps.addExtraTime(0, 0, 1);
    cout << "Extra time added." << endl;
    cout << endl;

    printSeparator();
    cout << "Test 10: Print both structures" << endl;
    cout << "--- Stack Info ---" << endl;
    ParkingDualStackInterface& stackMsg5 = ps.getStackMessage();
    for (int i = 0; i < stackMsg5.size; i++) {
        cout << "  Car " << i << ": ";
        printCarInfo(stackMsg5.container[i]);
    }
    cout << "--- Waiting List Info ---" << endl;
    ParkingDoublyLinkedListInterface& listMsg5 = ps.getLinkedListMessage();
    cout << "  Waiting list size: " << listMsg5.size << endl;
    for (int i = 0; i < listMsg5.size; i++) {
        cout << "  Car " << i << ": ";
        printCarInfo(listMsg5.container[i]);
    }
    cout << endl;

    cout << "Sleep 2 seconds..." << endl;
    Sleep(2000);

    printSeparator();
    cout << "Test 11: Get G price -> Leave G from stack (was waiting, now in park)" << endl;
    ParkingDualStackInterface& msgForG = ps.getStackMessage();
    double gPrice = getPriceFromStackMsg(msgForG, "YuG0007");
    result = ps.leavePark("YuG0007", leftCar);
    cout << "  Result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "  Left car: " << leftCar << endl;
    cout << "  Parking time: " << leftCar.getPakingTime() << "s" << endl;
    cout << "  Price: " << gPrice << " yuan" << endl;
    ps.showPark();
    cout << endl;

    // =============================================
    // Phase 3: 剩余车辆操作
    // =============================================

    cout << "Sleep 5 seconds..." << endl;
    Sleep(5000);

    printSeparator();
    cout << "Test 12: Add 1 hour" << endl;
    ps.addExtraTime(0, 0, 1);
    cout << "Extra time added." << endl;
    cout << endl;

    printSeparator();
    cout << "Test 13: Print both structures" << endl;
    cout << "--- Stack Info ---" << endl;
    ParkingDualStackInterface& stackMsg6 = ps.getStackMessage();
    for (int i = 0; i < stackMsg6.size; i++) {
        cout << "  Car " << i << ": ";
        printCarInfo(stackMsg6.container[i]);
    }
    cout << "--- Waiting List Info ---" << endl;
    ParkingDoublyLinkedListInterface& listMsg6 = ps.getLinkedListMessage();
    if (listMsg6.size > 0) {
        cout << "  Waiting list size: " << listMsg6.size << endl;
        for (int i = 0; i < listMsg6.size; i++) {
            cout << "  Car " << i << ": ";
            printCarInfo(listMsg6.container[i]);
        }
    } else {
        cout << "  Waiting list is empty." << endl;
    }
    cout << endl;

    printSeparator();
    cout << "Test 14: Get C price -> Leave C from stack" << endl;
    ParkingDualStackInterface& msgForC = ps.getStackMessage();
    double cPrice = getPriceFromStackMsg(msgForC, "HuC0003");
    result = ps.leavePark("HuC0003", leftCar);
    cout << "  Result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "  Left car: " << leftCar << endl;
    cout << "  Parking time: " << leftCar.getPakingTime() << "s" << endl;
    cout << "  Price: " << cPrice << " yuan" << endl;
    ps.showPark();
    cout << endl;

    cout << "Sleep 5 seconds..." << endl;
    Sleep(5000);

    printSeparator();
    cout << "Test 15: Add 1 hour" << endl;
    ps.addExtraTime(0, 0, 1);
    cout << "Extra time added." << endl;
    cout << endl;

    printSeparator();
    cout << "Test 16: Print both structures" << endl;
    cout << "--- Stack Info ---" << endl;
    ParkingDualStackInterface& stackMsg7 = ps.getStackMessage();
    for (int i = 0; i < stackMsg7.size; i++) {
        cout << "  Car " << i << ": ";
        printCarInfo(stackMsg7.container[i]);
    }
    cout << "--- Waiting List Info ---" << endl;
    ParkingDoublyLinkedListInterface& listMsg7 = ps.getLinkedListMessage();
    if (listMsg7.size > 0) {
        cout << "  Waiting list size: " << listMsg7.size << endl;
        for (int i = 0; i < listMsg7.size; i++) {
            cout << "  Car " << i << ": ";
            printCarInfo(listMsg7.container[i]);
        }
    } else {
        cout << "  Waiting list is empty." << endl;
    }
    cout << endl;

    // =============================================
    // Phase 4: 最终清空
    // =============================================

    printSeparator();
    cout << "Test 17: Get A price -> Leave A from stack" << endl;
    ParkingDualStackInterface& msgForA = ps.getStackMessage();
    double aPrice = getPriceFromStackMsg(msgForA, "YueA0001");
    result = ps.leavePark("YueA0001", leftCar);
    cout << "  Result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "  Left car: " << leftCar << endl;
    cout << "  Parking time: " << leftCar.getPakingTime() << "s" << endl;
    cout << "  Price: " << aPrice << " yuan" << endl;
    ps.showPark();
    cout << endl;

    printSeparator();
    cout << "Test 18: Get D price -> Leave D from stack" << endl;
    ParkingDualStackInterface& msgForD = ps.getStackMessage();
    double dPrice = getPriceFromStackMsg(msgForD, "SuD0004");
    result = ps.leavePark("SuD0004", leftCar);
    cout << "  Result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "  Left car: " << leftCar << endl;
    cout << "  Parking time: " << leftCar.getPakingTime() << "s" << endl;
    cout << "  Price: " << dPrice << " yuan" << endl;
    ps.showPark();
    cout << endl;

    printSeparator();
    cout << "Test 19: Get E price -> Leave E from stack" << endl;
    ParkingDualStackInterface& msgForE = ps.getStackMessage();
    double ePrice = getPriceFromStackMsg(msgForE, "MinE0005");
    result = ps.leavePark("MinE0005", leftCar);
    cout << "  Result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "  Left car: " << leftCar << endl;
    cout << "  Parking time: " << leftCar.getPakingTime() << "s" << endl;
    cout << "  Price: " << ePrice << " yuan" << endl;
    ps.showPark();
    cout << endl;

    printSeparator();
    cout << "Test 20: Final status - both structures should be empty" << endl;
    ParkingDualStackInterface& stackMsg8 = ps.getStackMessage();
    if (stackMsg8.size == 0) {
        cout << "  Stack is empty." << endl;
    } else {
        for (int i = 0; i < stackMsg8.size; i++) {
            cout << "  Car " << i << ": ";
            printCarInfo(stackMsg8.container[i]);
        }
    }
    ParkingDoublyLinkedListInterface& listMsg8 = ps.getLinkedListMessage();
    if (listMsg8.size == 0) {
        cout << "  Waiting list is empty." << endl;
    } else {
        cout << "  Waiting list size: " << listMsg8.size << endl;
        for (int i = 0; i < listMsg8.size; i++) {
            cout << "  Car " << i << ": ";
            printCarInfo(listMsg8.container[i]);
        }
    }
    ps.showPark();
    cout << endl;

    printSeparator();
    cout << "All tests completed!" << endl;

    return 0;
}
