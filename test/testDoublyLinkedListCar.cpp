#include <iostream>
#include <string>
#include "DoublyLinkedList.h"
#include "Car.h"

using namespace std;

void printSeparator() {
    cout << "----------------------------------------" << endl;
}

void showStatus(DoublyLinkedList<Car>& list, const string& label) {
    cout << "[" << label << "] ";
    list.showDoublyLinkedList();
    cout << "(size: " << list.getSize() << ")" << endl;
}

int main() {

    // Test 1: Create empty list
    printSeparator();
    cout << "Test 1: Create empty DoublyLinkedList<Car>" << endl;
    DoublyLinkedList<Car> list;
    cout << "isEmpty: " << (list.isEmpty() ? "true" : "false") << " (expected: true)" << endl;
    cout << "size: " << list.getSize() << " (expected: 0)" << endl;

    // Test 2: insert at head
    printSeparator();
    cout << "Test 2: insert at head: CarC, CarB, CarA" << endl;
    Car carA("YueA0001");
    Car carB("JingB0002");
    Car carC("HuC0003");
    Car carD("SuD0004");
    Car carE("MinE0005");
    Car carX("YueX9999");

    list.insert(0, carC);
    list.insert(0, carB);
    list.insert(0, carA);
    showStatus(list, "after inserts");

    // Test 3: insert at tail
    printSeparator();
    cout << "Test 3: insert at tail: carD, carE" << endl;
    list.insert(list.getSize(), carD);
    list.insert(list.getSize(), carE);
    showStatus(list, "after tail inserts");
    cout << "expected: YueA0001 JingB0002 HuC0003 SuD0004 MinE0005" << endl;

    // Test 4: insert at middle
    printSeparator();
    cout << "Test 4: insert carX at index 2" << endl;
    list.insert(2, carX);
    showStatus(list, "after insert carX");
    cout << "expected: YueA0001 JingB0002 YueX9999 HuC0003 SuD0004 MinE0005" << endl;

    // Test 5: get each element
    printSeparator();
    cout << "Test 5: get each element" << endl;
    string expected[] = {"YueA0001", "JingB0002", "YueX9999", "HuC0003", "SuD0004", "MinE0005"};
    bool allCorrect = true;
    for (int i = 0; i < list.getSize(); i++) {
        Car val("");
        list.get(i, val);
        bool match = (val == expected[i]);
        cout << "  [" << i << "] ";
        cout << val;
        cout << " (expected: " << expected[i] << ") " << (match ? "ok" : "FAIL") << endl;
        if (!match) allCorrect = false;
    }
    cout << (allCorrect ? "All get tests passed." : "Some get tests FAILED!") << endl;

    // Test 6: get out of bounds
    printSeparator();
    cout << "Test 6: get out of bounds" << endl;
    Car dummy("");
    cout << "get at index -1: " << (list.get(-1, dummy) ? "ok" : "fail") << " (expected: fail)" << endl;
    cout << "get at index 6: " << (list.get(6, dummy) ? "ok" : "fail") << " (expected: fail)" << endl;

    // Test 7: indexOf existing element
    printSeparator();
    cout << "Test 7: indexOf existing elements" << endl;
    cout << "indexOf carX: " << list.indexOf(carX) << " (expected: 2)" << endl;
    cout << "indexOf carA: " << list.indexOf(carA) << " (expected: 0)" << endl;
    cout << "indexOf carE: " << list.indexOf(carE) << " (expected: 5)" << endl;

    // Test 8: indexOf non-existing element
    printSeparator();
    cout << "Test 8: indexOf non-existing element" << endl;
    Car notExist("NOT_EXIST");
    cout << "indexOf NOT_EXIST: " << list.indexOf(notExist) << " (expected: -1)" << endl;

    // Test 9: erase middle (index 2, carX)
    printSeparator();
    cout << "Test 9: erase middle (index 2, which is carX)" << endl;
    cout << "erase result: " << (list.erase(2) ? "true" : "false") << " (expected: true)" << endl;
    showStatus(list, "after erase middle");
    cout << "expected: YueA0001 JingB0002 HuC0003 SuD0004 MinE0005" << endl;

    // Test 10: erase head (index 0, carA)
    printSeparator();
    cout << "Test 10: erase head (index 0, which is carA)" << endl;
    cout << "erase result: " << (list.erase(0) ? "true" : "false") << " (expected: true)" << endl;
    showStatus(list, "after erase head");
    cout << "expected: JingB0002 HuC0003 SuD0004 MinE0005" << endl;

    // Test 11: erase tail (index 3, carE)
    printSeparator();
    cout << "Test 11: erase tail (index 3, which is carE)" << endl;
    cout << "erase result: " << (list.erase(3) ? "true" : "false") << " (expected: true)" << endl;
    showStatus(list, "after erase tail");
    cout << "expected: JingB0002 HuC0003 SuD0004" << endl;

    // Test 12: erase out of bounds
    printSeparator();
    cout << "Test 12: erase out of bounds" << endl;
    cout << "erase at index -1: " << (list.erase(-1) ? "ok" : "fail") << " (expected: fail)" << endl;
    cout << "erase at index 5: " << (list.erase(5) ? "ok" : "fail") << " (expected: fail)" << endl;

    // Test 13: Copy constructor
    printSeparator();
    cout << "Test 13: Copy constructor" << endl;
    DoublyLinkedList<Car> listCopy(list);
    cout << "Original: ";
    showStatus(list, "orig");
    cout << "Copy:     ";
    showStatus(listCopy, "copy");

    Car val("");
    listCopy.erase(0);
    listCopy.get(0, val);
    cout << "After erase head from copy:" << endl;
    cout << "Copy first: " << val << " (expected: HuC0003)" << endl;
    list.get(0, val);
    cout << "Original first: " << val << " (expected: JingB0002, unchanged)" << endl;

    // Test 14: Copy assignment
    printSeparator();
    cout << "Test 14: Copy assignment" << endl;
    DoublyLinkedList<Car> listAssign;
    listAssign.insert(0, Car("TEMP"));
    listAssign = list;
    cout << "Original:  ";
    showStatus(list, "orig");
    cout << "Assigned:  ";
    showStatus(listAssign, "assign");

    listAssign.erase(0);
    listAssign.get(0, val);
    cout << "After erase head from assigned:" << endl;
    cout << "Assigned first: " << val << " (expected: HuC0003)" << endl;
    list.get(0, val);
    cout << "Original first: " << val << " (expected: JingB0002, unchanged)" << endl;

    // Test 15: clear all
    printSeparator();
    cout << "Test 15: Clear all elements" << endl;
    while (list.getSize() > 0) {
        list.erase(0);
    }
    cout << "isEmpty after clear: " << (list.isEmpty() ? "true" : "false") << " (expected: true)" << endl;
    showStatus(list, "after clear");

    // Test 16: insert into empty list
    printSeparator();
    cout << "Test 16: insert into empty list" << endl;
    cout << "insert(0, carA): " << (list.insert(0, carA) ? "ok" : "fail") << " (expected: ok)" << endl;
    showStatus(list, "after insert into empty");
    cout << "expected: YueA0001" << endl;

    printSeparator();
    cout << "All tests completed!" << endl;

    return 0;
}
