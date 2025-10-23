#include "Stack.h"
#include <iostream>

using namespace std;

void runFunction1(Stack stack) {
    cout << "Inside runFunction1" << endl;
    stack.push(21);
    stack.pop();
}

void runFunction2(Stack& stack) {
    cout << "Inside runFunction2" << endl;
    stack.push(21);
    stack.pop();
}

int main() {
    Stack s1;
    s1.push(1);
    s1.push(2);
    s1.push(3);

    Stack s2(s1);

    Stack s3 = s1;

    Stack s4;
    s4.push(10);
    s4 = s1;

    runFunction1(s1);
    runFunction2(s1);

    return 0;
}