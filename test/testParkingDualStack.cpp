#include <iostream>
#include <string>
#include <windows.h>
#include "ParkingDualStack.h"

using namespace std;

void printSeparator() {
    cout << "----------------------------------------" << endl;
}

void showStatus(ParkingDualStack& pds, const string& label) {
    cout << "[" << label << "] Left:  ";
    pds.showLeftStack();
    cout << endl;
    cout << "[" << label << "] Right: ";
    pds.showRightStack();
    cout << endl;
}

int main() {

    // Test 1: Create parking with capacity 3
    printSeparator();
    cout << "Test 1: Create ParkingDualStack with capacity 3" << endl;
    ParkingDualStack park(3);
    showStatus(park, "init");

    // Test 2: push 3 cars
    printSeparator();
    cout << "Test 2: Push 3 cars (YueA, JingB, HuC)" << endl;
    Car car1("YueA12345");
    Car car2("JingB67890");
    Car car3("HuC11111");

    cout << "push YueA: " << (park.pushDualStack(car1) ? "ok" : "fail") << endl;
    cout << "push JingB: " << (park.pushDualStack(car2) ? "ok" : "fail") << endl;
    cout << "push HuC: " << (park.pushDualStack(car3) ? "ok" : "fail") << endl;
    showStatus(park, "after 3 pushes");

    cout << "Waiting 15 seconds..." << endl;
    Sleep(15000);

    // Test 3: push 4th car (should fail, full)
    printSeparator();
    cout << "Test 3: Push 4th car (should fail)" << endl;
    Car car4("SuD22222");
    cout << "push SuD (full): " << (park.pushDualStack(car4) ? "ok" : "fail") << " (expected: fail)" << endl;

    // Test 4: popDualStack middle element "JingB"
    printSeparator();
    cout << "Test 4: popDualStack(\"JingB67890\")" << endl;
    Car popped;
    bool result = park.popDualStack("JingB67890", popped);
    cout << "result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "popped car: " << popped << endl;
    cout << "parking time: " << popped.getPakingTime() << " seconds" << endl;
    showStatus(park, "after pop JingB");

    cout << "Waiting 15 seconds..." << endl;
    Sleep(15000);

    // Test 5: popDualStack bottom element "YueA"
    printSeparator();
    cout << "Test 5: popDualStack(\"YueA12345\")" << endl;
    result = park.popDualStack("YueA12345", popped);
    cout << "result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "popped car: " << popped << endl;
    cout << "parking time: " << popped.getPakingTime() << " seconds" << endl;
    showStatus(park, "after pop YueA");

    // Test 6: popDualStack non-existent
    printSeparator();
    cout << "Test 6: popDualStack(\"NOT_EXIST\")" << endl;
    result = park.popDualStack("NOT_EXIST", popped);
    cout << "result: " << (result ? "true" : "false") << " (expected: false)" << endl;
    showStatus(park, "after not_exist (no change)");

    cout << "Waiting 20 seconds..." << endl;
    Sleep(20000);

    // Test 7: pop remaining and check parking time
    printSeparator();
    cout << "Test 7: Pop remaining HuC and check parking time" << endl;
    Sleep(1000);
    result = park.popDualStack("HuC11111", popped);
    cout << "result: " << (result ? "true" : "false") << " (expected: true)" << endl;
    cout << "popped car: " << popped << endl;
    cout << "parking time: " << popped.getPakingTime() << " seconds (expected: >= 1)" << endl;
    showStatus(park, "after pop HuC");

    // Test 8: Copy constructor
    printSeparator();
    cout << "Test 8: Copy constructor" << endl;
    Car c1("CopyA");
    Car c2("CopyB");
    Car c3("CopyC");
    park.pushDualStack(c1);
    park.pushDualStack(c2);
    park.pushDualStack(c3);
    cout << "Original: ";
    showStatus(park, "original");

    ParkingDualStack parkCopy(park);
    cout << "Copy:     ";
    showStatus(parkCopy, "copy");

    Car copyPopped;
    parkCopy.popDualStack("CopyB", copyPopped);
    cout << "After pop CopyB from copy:" << endl;
    cout << "Copy:     ";
    showStatus(parkCopy, "copy");
    cout << "Original: ";
    showStatus(park, "original (unchanged)");

    Car dummy;
    while(park.popLeftStack(dummy)) {}

    // Test 9: Copy assignment
    printSeparator();
    cout << "Test 9: Copy assignment" << endl;
    Car a1("AsnA");
    Car a2("AsnB");
    park.pushDualStack(a1);
    park.pushDualStack(a2);
    ParkingDualStack parkAssign(1);
    parkAssign = park;

    cout << "Waiting 10 seconds..." << endl;
    Sleep(10000);

    cout << "Original: ";
    showStatus(park, "original");
    cout << "Assigned: ";
    showStatus(parkAssign, "assigned");

    Car assignPopped;
    parkAssign.popDualStack("AsnA", assignPopped);
    cout << "After pop AsnA from assigned:" << endl;
    cout << "Assigned: ";
    showStatus(parkAssign, "assigned");
    cout << "Original: ";
    showStatus(park, "original (unchanged)");

    printSeparator();
    cout << "All tests completed!" << endl;

    return 0;
}
