#include <iostream>
#include <string>
#include "DualStack.h"

using namespace std;

void printSeparator() {
    cout << "----------------------------------------" << endl;
}

void showStatus(DualStack<string>& ds, const string& label) {
    cout << "[" << label << "] Left:  ";
    ds.showLeftStack();
    cout << endl;
    cout << "[" << label << "] Right: ";
    ds.showRightStack();
    cout << endl;
}

int main() {

    // Test 1: Construct + isEmpty
    printSeparator();
    cout << "Test 1: Create DualStack with capacity 5" << endl;
    DualStack<string> ds(5);
    showStatus(ds, "init");

    // Test 2: pushLeftStack A, B, C
    printSeparator();
    cout << "Test 2: pushLeftStack A, B, C" << endl;
    ds.pushLeftStack("A");
    ds.pushLeftStack("B");
    ds.pushLeftStack("C");
    showStatus(ds, "after push A,B,C");

    // Test 3: popLeftStack
    printSeparator();
    cout << "Test 3: popLeftStack (should pop C)" << endl;
    string val;
    ds.popLeftStack(val);
    cout << "popped: " << val << " (expected: C)" << endl;
    showStatus(ds, "after pop C");

    // Test 4: pushRightStack X, Y
    printSeparator();
    cout << "Test 4: pushRightStack X, Y" << endl;
    ds.pushRightStack("X");
    ds.pushRightStack("Y");
    showStatus(ds, "after push X,Y");

    // Test 5: popRightStack
    printSeparator();
    cout << "Test 5: popRightStack (should pop Y)" << endl;
    ds.popRightStack(val);
    cout << "popped: " << val << " (expected: Y)" << endl;
    showStatus(ds, "after pop Y");

    // Test 6: pushDualStack + isFull boundary
    printSeparator();
    cout << "Test 6: pushDualStack D, E (fill to max)" << endl;
    ds.pushDualStack("D");
    cout << "push D ok, isFull: " << (ds.isFull() ? "true" : "false") << " (expected: false)" << endl;
    ds.pushDualStack("E");
    cout << "push E ok, isFull: " << (ds.isFull() ? "true" : "false") << " (expected: true)" << endl;
    cout << "push F (should fail): " << (ds.pushDualStack("F") ? "ok" : "fail") << " (expected: fail)" << endl;
    showStatus(ds, "after push D,E");

    // Test 7: popDualStack middle element "B"
    printSeparator();
    cout << "Test 7: popDualStack(\"B\") from [A,B,D,E]" << endl;
    string popped;
    bool result = ds.popDualStack("B", popped);
    cout << "result: " << (result ? "true" : "false") << " (expected: true), popped: " << popped << " (expected: B)" << endl;
    showStatus(ds, "after pop B");

    // Test 8: popDualStack bottom element "A"
    printSeparator();
    cout << "Test 8: popDualStack(\"A\") from [A,D,E]" << endl;
    result = ds.popDualStack("A", popped);
    cout << "result: " << (result ? "true" : "false") << " (expected: true), popped: " << popped << " (expected: A)" << endl;
    showStatus(ds, "after pop A");

    printSeparator();
    cout << "Test 8.5: popLeftStack to clear remaining elements" << endl;
    string x;
    ds.popLeftStack(x);
    cout << "popped: " << x << endl;
    showStatus(ds, "after clear X");

    // Test 9: popDualStack top element "E"
    printSeparator();
    cout << "Test 9: popDualStack(\"E\") from [D,E]" << endl;
    result = ds.popDualStack("E", popped);
    cout << "result: " << (result ? "true" : "false") << " (expected: true), popped: " << popped << " (expected: E)" << endl;
    showStatus(ds, "after pop E");

    // Test 10: popDualStack non-existent value
    printSeparator();
    cout << "Test 10: popDualStack(\"NOT_EXIST\") from [D]" << endl;
    result = ds.popDualStack("NOT_EXIST", popped);
    cout << "result: " << (result ? "true" : "false") << " (expected: false)" << endl;
    showStatus(ds, "after not_exist (no change)");

    // Test 11: pop/top on empty stack
    printSeparator();
    cout << "Test 11: Pop and top on empty stack" << endl;
    ds.popLeftStack(val);
    ds.popLeftStack(val);
    cout << "after popping remaining elements:" << endl;
    showStatus(ds, "after clear");

    string emptyVal;
    cout << "popLeft on empty: " << (ds.popLeftStack(emptyVal) ? "ok" : "fail") << " (expected: fail)" << endl;
    cout << "popRight on empty: " << (ds.popRightStack(emptyVal) ? "ok" : "fail") << " (expected: fail)" << endl;
    cout << "topLeft on empty: " << (ds.topLeftStack(emptyVal) ? "ok" : "fail") << " (expected: fail)" << endl;
    cout << "topRight on empty: " << (ds.topRightStack(emptyVal) ? "ok" : "fail") << " (expected: fail)" << endl;

    // Test 12: topLeftStack / topRightStack
    printSeparator();
    cout << "Test 12: top without popping" << endl;
    ds.pushLeftStack("TOP1");
    ds.pushRightStack("TOP2");
    showStatus(ds, "after push TOP1, TOP2");
    string topVal;
    ds.topLeftStack(topVal);  cout << "topLeft: " << topVal << " (expected: TOP1)" << endl;
    ds.topRightStack(topVal); cout << "topRight: " << topVal << " (expected: TOP2)" << endl;
    showStatus(ds, "after top (no change)");

    ds.popLeftStack(val);
    ds.popRightStack(val);
    showStatus(ds, "after pop TOP1, TOP2");

    // Test 13: Copy constructor
    printSeparator();
    cout << "Test 13: Copy constructor" << endl;
    ds.pushDualStack("CPY1");
    ds.pushDualStack("CPY2");
    ds.pushDualStack("CPY3");
    DualStack<string> dsCopy(ds);

    cout << "Original: ";
    showStatus(ds, "original");
    cout << "Copy:     ";
    showStatus(dsCopy, "copy");

    dsCopy.popLeftStack(val);
    cout << "After pop from copy (pop " << val << "):" << endl;
    cout << "Copy:     ";
    showStatus(dsCopy, "copy");
    cout << "Original: ";
    showStatus(ds, "original");
    cout << "(original unchanged)" << endl;

    while(!ds.isLeftStackEmpty()) { string t; ds.popLeftStack(t); }

    // Test 14: Copy assignment
    printSeparator();
    cout << "Test 14: Copy assignment operator" << endl;
    ds.pushDualStack("ASN1");
    ds.pushDualStack("ASN2");
    DualStack<string> dsAssign(1);
    dsAssign = ds;

    cout << "Original: ";
    showStatus(ds, "original");
    cout << "Assigned: ";
    showStatus(dsAssign, "assigned");

    dsAssign.popLeftStack(val);
    cout << "After pop from assigned (pop " << val << "):" << endl;
    cout << "Assigned: ";
    showStatus(dsAssign, "assigned");
    cout << "Original: ";
    showStatus(ds, "original");
    cout << "(original unchanged)" << endl;

    while(!ds.isLeftStackEmpty()) { string t; ds.popLeftStack(t); }

    // Test 15: show empty stacks
    printSeparator();
    cout << "Test 15: Show empty stacks" << endl;
    showStatus(ds, "empty");

    printSeparator();
    cout << "All tests completed!" << endl;

    return 0;
}
