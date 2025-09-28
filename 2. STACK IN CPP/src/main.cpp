#include "../include/Stack.h"
#include <iostream>

using namespace std;

int main() {
    Stack stack;
    int n, value;

    cout << "How many number you want to enter: ";
    cin >> n;

    cout << "Enter " << n << " values:" << endl;
    for (int i = 0; i < n; ++i) {
        cin >> value;
        stack.push(value);
    }

    cout << "Your numbers in reverse: " << endl;
    while (!stack.isEmpty()) {
        cout << stack.pop() << " ";
    }
    cout << endl;

}