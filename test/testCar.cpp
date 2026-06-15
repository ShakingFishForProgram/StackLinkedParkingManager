#include <iostream>
#include <windows.h>
#include "Car.h"

using namespace std;

static void printSeparator() {
    cout << "----------------------------------------" << endl;
}

int main() {
    // 1. Create vehicles
    printSeparator();
    cout << "Test 1: Create vehicles" << endl;
    Car car1("YueA12345");
    Car car2("JingB67890");
    Car car3("HuC11111");
    cout << "Created 3 vehicles" << endl;

    // 2. Print initial car info (not in park)
    printSeparator();
    cout << "Test 2: Print initial info (not in park)" << endl;
    cout << car1 << endl;

    // 3. Vehicle enters park
    printSeparator();
    cout << "Test 3: Vehicle enters park" << endl;
    car1.inPark();
    cout << car1 << endl;

    // 4. Simulate parking for 3 seconds
    printSeparator();
    cout << "Test 4: Simulate parking for 3 seconds" << endl;
    for (int i = 3; i > 0; --i) {
        Sleep(1000);
        cout << i << " seconds remaining..." << endl;
    }

    // 5. Vehicle leaves park
    printSeparator();
    cout << "Test 5: Vehicle leaves park" << endl;
    car1.outPark();
    cout << car1 << endl;

    // 6. Verify parking duration
    printSeparator();
    cout << "Test 6: Calculate parking duration" << endl;
    time_t duration = car1.getPakingTime();
    cout << "Parking duration: " << duration << " seconds" << endl;
    if (duration >= 2 && duration <= 5) {
        cout << "Duration is reasonable, test passed." << endl;
    } else {
        cout << "Duration abnormal, please check!" << endl;
    }

    // 7. Test multiple entries and exits
    printSeparator();
    cout << "Test 7: Multiple entries and exits (3 cycles)" << endl;
    for (int i = 1; i <= 3; ++i) {
        cout << "Cycle " << i << ":" << endl;
        car2.inPark();
        Sleep(500);
        car2.outPark();
        cout << "  Parking duration: " << car2.getPakingTime() << " seconds" << endl;
    }

    // 8. Test multiple vehicles in park simultaneously
    printSeparator();
    cout << "Test 8: Multiple vehicles in park simultaneously" << endl;
    car1.inPark();
    car2.inPark();
    car3.inPark();
    Sleep(2000);
    cout << "All vehicles have entered and stayed for 2 seconds" << endl;
    cout << car1 << endl;
    cout << car2 << endl;
    cout << car3 << endl;

    car1.outPark();
    car2.outPark();
    car3.outPark();
    cout << "Parking durations:" << endl;
    cout << "  car1: " << car1.getPakingTime() << " seconds" << endl;
    cout << "  car2: " << car2.getPakingTime() << " seconds" << endl;
    cout << "  car3: " << car3.getPakingTime() << " seconds" << endl;

    // 9. Boundary test: rapid entry and exit
    printSeparator();
    cout << "Test 9: Rapid entry and exit (no wait)" << endl;
    car1.inPark();
    car1.outPark();
    cout << "Parking duration: " << car1.getPakingTime() << " seconds" << endl;
    if (car1.getPakingTime() == 0) {
        cout << "Rapid entry/exit test passed." << endl;
    }

    printSeparator();
    cout << "All tests completed!" << endl;

    return 0;
}
