#include <iostream>
#include <string>
#include <windows.h>
#include "ParkingStructure.h"

using namespace std;

void printSeparator() {
    cout << "----------------------------------------" << endl;
}

int main() {

    // Test 1: Create parking with capacity 5
    printSeparator();
    cout << "Test 1: Create ParkingStructure with capacity 5" << endl;
    ParkingStructure ps(5);
    ps.showPark();
    cout << endl;

    // Test 2: Enter 5 cars (A, B, C, D, E) -> all go to stack
    printSeparator();
    cout << "Test 2: Enter 5 cars (A, B, C, D, E)" << endl;
    Car carA("YueA0001");
    Car carB("JingB0002");
    Car carC("HuC0003");
    Car carD("SuD0004");
    Car carE("MinE0005");

    cout << "Enter A: " << (ps.insertPark(carA) ? "ok" : "fail") << endl;
    cout << "Enter B: " << (ps.insertPark(carB) ? "ok" : "fail") << endl;
    cout << "Enter C: " << (ps.insertPark(carC) ? "ok" : "fail") << endl;
    cout << "Enter D: " << (ps.insertPark(carD) ? "ok" : "fail") << endl;
    cout << "Enter E: " << (ps.insertPark(carE) ? "ok" : "fail") << endl;
    ps.showPark();
    cout << endl;

    // Test 3: Enter 3 more cars (F, G, H) -> all go to waiting list
    printSeparator();
    cout << "Test 3: Enter 3 more cars (F, G, H) -> waiting list" << endl;
    Car carF("LuF0006");
    Car carG("YuG0007");
    Car carH("EH0008");

    cout << "Enter F: " << (ps.insertPark(carF) ? "ok" : "fail") << " (expected: ok, in waiting)" << endl;
    cout << "Enter G: " << (ps.insertPark(carG) ? "ok" : "fail") << " (expected: ok, in waiting)" << endl;
    cout << "Enter H: " << (ps.insertPark(carH) ? "ok" : "fail") << " (expected: ok, in waiting)" << endl;
    ps.showPark();
    cout << endl;

    // Test 4: Leave B (middle of stack) -> waiting car F enters park
    printSeparator();
    cout << "Test 4: Leave B (middle of stack)" << endl;
    Car leftCar("");
    bool result = ps.leavePark("JingB0002", leftCar);
    cout << "result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "left car: " << leftCar << endl;
    cout << "parking time: " << leftCar.getPakingTime() << " seconds" << endl;
    ps.showPark();
    cout << endl;

    // Test 5: Leave A (bottom of stack) -> waiting car G enters park
    printSeparator();
    cout << "Test 5: Leave A (bottom of stack)" << endl;
    result = ps.leavePark("YueA0001", leftCar);
    cout << "result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "left car: " << leftCar << endl;
    cout << "parking time: " << leftCar.getPakingTime() << " seconds" << endl;
    ps.showPark();
    cout << endl;

    // Test 6: Leave H (in waiting list, no fee)
    printSeparator();
    cout << "Test 6: Leave H (in waiting list, no parking fee)" << endl;
    result = ps.leavePark("EH0008", leftCar);
    cout << "result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "left car: " << leftCar << endl;
    cout << "parking time: " << leftCar.getPakingTime() << " seconds (expected: 0, no fee)" << endl;
    ps.showPark();
    cout << endl;

    // Test 7: Leave non-existing car
    printSeparator();
    cout << "Test 7: Leave non-existing car" << endl;
    result = ps.leavePark("NOT_EXIST", leftCar);
    cout << "result: " << (result ? "true" : "false") << " (expected: false)" << endl;
    ps.showPark();
    cout << endl;

    // Test 8: Leave F (now at top of stack, was from waiting list)
    printSeparator();
    cout << "Test 8: Leave F (was in waiting, now in park)" << endl;
    result = ps.leavePark("LuF0006", leftCar);
    cout << "result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "left car: " << leftCar << endl;
    cout << "parking time: " << leftCar.getPakingTime() << " seconds" << endl;
    ps.showPark();
    cout << endl;

    // Test 9: Leave remaining cars (C, D, E, G) one by one
    printSeparator();
    cout << "Test 9: Leave remaining cars" << endl;

    result = ps.leavePark("HuC0003", leftCar);
    cout << "Leave C: result=" << result << ", car=" << leftCar << ", time=" << leftCar.getPakingTime() << "s" << endl;
    ps.showPark();
    cout << endl;

    result = ps.leavePark("SuD0004", leftCar);
    cout << "Leave D: result=" << result << ", car=" << leftCar << ", time=" << leftCar.getPakingTime() << "s" << endl;
    ps.showPark();
    cout << endl;

    result = ps.leavePark("YuG0007", leftCar);
    cout << "Leave G: result=" << result << ", car=" << leftCar << ", time=" << leftCar.getPakingTime() << "s" << endl;
    ps.showPark();
    cout << endl;

    result = ps.leavePark("MinE0005", leftCar);
    cout << "Leave E: result=" << result << ", car=" << leftCar << ", time=" << leftCar.getPakingTime() << "s" << endl;
    ps.showPark();
    cout << endl;

    // Test 10: Try to leave from empty parking
    printSeparator();
    cout << "Test 10: Leave from empty parking" << endl;
    result = ps.leavePark("YueA0001", leftCar);
    cout << "result: " << (result ? "true" : "false") << " (expected: false)" << endl;
    ps.showPark();
    cout << endl;

    printSeparator();
    cout << "All tests completed!" << endl;

    return 0;
}
