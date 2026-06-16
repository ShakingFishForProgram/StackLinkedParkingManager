#include <iostream>
#include <string>
#include "DoublyLinkedList.h"

using namespace std;

void printSeparator() {
    cout << "----------------------------------------" << endl;
}

void showStatus(DoublyLinkedList<string>& list, const string& label) {
    cout << "[" << label << "] ";
    list.showDoublyLinkedList();
    cout << "(size: " << list.getSize() << ")" << endl;
}

int main() {

    // Test 1: Create empty list
    printSeparator();
    cout << "Test 1: Create empty DoublyLinkedList" << endl;
    DoublyLinkedList<string> list;
    cout << "isEmpty: " << (list.isEmpty() ? "true" : "false") << " (expected: true)" << endl;
    cout << "size: " << list.getSize() << " (expected: 0)" << endl;

    // Test 2: insert at head
    printSeparator();
    cout << "Test 2: insert at head: C, B, A" << endl;
    list.insert(0, "C");
    list.insert(0, "B");
    list.insert(0, "A");
    showStatus(list, "after inserts");

    // Test 3: insert at tail
    printSeparator();
    cout << "Test 3: insert at tail: D, E" << endl;
    list.insert(list.getSize(), "D");
    list.insert(list.getSize(), "E");
    showStatus(list, "after tail inserts");
    cout << "expected: A B C D E" << endl;

    // Test 4: insert at middle
    printSeparator();
    cout << "Test 4: insert \"X\" at index 2" << endl;
    list.insert(2, "X");
    showStatus(list, "after insert X");
    cout << "expected: A B X C D E" << endl;

    // Test 5: get each element
    printSeparator();
    cout << "Test 5: get each element" << endl;
    string expected[] = {"A", "B", "X", "C", "D", "E"};
    bool allCorrect = true;
    for (int i = 0; i < list.getSize(); i++) {
        string val;
        list.get(i, val);
        bool match = (val == expected[i]);
        cout << "  [" << i << "] " << val << " (expected: " << expected[i] << ") " << (match ? "ok" : "FAIL") << endl;
        if (!match) allCorrect = false;
    }
    cout << (allCorrect ? "All get tests passed." : "Some get tests FAILED!") << endl;

    // Test 6: get out of bounds
    printSeparator();
    cout << "Test 6: get out of bounds" << endl;
    string dummy;
    cout << "get at index -1: " << (list.get(-1, dummy) ? "ok" : "fail") << " (expected: fail)" << endl;
    cout << "get at index 6: " << (list.get(6, dummy) ? "ok" : "fail") << " (expected: fail)" << endl;

    // Test 7: indexOf existing element
    printSeparator();
    cout << "Test 7: indexOf existing elements" << endl;
    cout << "indexOf \"X\": " << list.indexOf("X") << " (expected: 2)" << endl;
    cout << "indexOf \"A\": " << list.indexOf("A") << " (expected: 0)" << endl;
    cout << "indexOf \"E\": " << list.indexOf("E") << " (expected: 5)" << endl;

    // Test 8: indexOf non-existing element
    printSeparator();
    cout << "Test 8: indexOf non-existing element" << endl;
    cout << "indexOf \"NOT\": " << list.indexOf("NOT") << " (expected: -1)" << endl;

    // Test 9: erase middle
    printSeparator();
    cout << "Test 9: erase middle (index 2, which is X)" << endl;
    cout << "erase result: " << (list.erase(2) ? "true" : "false") << " (expected: true)" << endl;
    showStatus(list, "after erase middle");
    cout << "expected: A B C D E" << endl;

    // Test 10: erase head
    printSeparator();
    cout << "Test 10: erase head (index 0, which is A)" << endl;
    cout << "erase result: " << (list.erase(0) ? "true" : "false") << " (expected: true)" << endl;
    showStatus(list, "after erase head");
    cout << "expected: B C D E" << endl;

    // Test 11: erase tail
    printSeparator();
    cout << "Test 11: erase tail (index 3, which is E)" << endl;
    cout << "erase result: " << (list.erase(3) ? "true" : "false") << " (expected: true)" << endl;
    showStatus(list, "after erase tail");
    cout << "expected: B C D" << endl;

    // Test 12: erase out of bounds
    printSeparator();
    cout << "Test 12: erase out of bounds" << endl;
    cout << "erase at index -1: " << (list.erase(-1) ? "ok" : "fail") << " (expected: fail)" << endl;
    cout << "erase at index 5: " << (list.erase(5) ? "ok" : "fail") << " (expected: fail)" << endl;

    // Test 13: Copy constructor
    printSeparator();
    cout << "Test 13: Copy constructor" << endl;
    DoublyLinkedList<string> listCopy(list);
    cout << "Original: ";
    showStatus(list, "orig");
    cout << "Copy:     ";
    showStatus(listCopy, "copy");

    string val;
    listCopy.erase(0);
    listCopy.get(0, val);
    cout << "After erase head from copy:" << endl;
    cout << "Copy first: " << val << " (expected: C)" << endl;
    list.get(0, val);
    cout << "Original first: " << val << " (expected: B, unchanged)" << endl;

    // Test 14: Copy assignment
    printSeparator();
    cout << "Test 14: Copy assignment" << endl;
    DoublyLinkedList<string> listAssign;
    listAssign.insert(0, "TEMP");
    listAssign = list;
    cout << "Original:  ";
    showStatus(list, "orig");
    cout << "Assigned:  ";
    showStatus(listAssign, "assign");

    listAssign.erase(0);
    listAssign.get(0, val);
    cout << "After erase head from assigned:" << endl;
    cout << "Assigned first: " << val << " (expected: C)" << endl;
    list.get(0, val);
    cout << "Original first: " << val << " (expected: B, unchanged)" << endl;

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
    cout << "insert(0, \"Z\"): " << (list.insert(0, "Z") ? "ok" : "fail") << " (expected: ok)" << endl;
    showStatus(list, "after insert into empty");
    cout << "expected: Z" << endl;

    printSeparator();
    cout << "All tests completed!" << endl;

    return 0;
}
