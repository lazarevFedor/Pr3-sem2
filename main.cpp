#include <iostream>
#include <Windows.h>
#include <limits>
#undef max
using namespace std;


void clearStream() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.sync();
}


struct Node {
    int data;
    Node* next;
    Node(int inputData) {
        data = inputData;
        next = nullptr;
    }
};


struct Stack {
    Node* head, * tail;

    void createStack(){
        head = tail = nullptr;
    }

    void clearStack(){
        while (head != nullptr) popFront();
    }

    void popFront(){
        if (head == nullptr) return;
        if (head == tail){
            delete tail;
            head = tail = nullptr;
            return;
        }
        Node* node = head;
        head = node->next;
        delete node;
    }

    void pushFront(int data){
        Node* node = new Node(data);
        node->next = head;
        head = node;
        if (tail == nullptr) tail = node;
    }

    Node* getHead(){
        return head;
    }

    void printStack(){
        for (Node *ptr = head; ptr != nullptr; ptr = ptr->next)
            cout << ptr->data << " ";
        cout << "\n";
    }

    void fillStack(){
        int number;
        while (cin >> number) {
            pushFront(number);
            if (cin.peek() == '\n') {
                break;
            }
        }
        clearStream();
    }
};


int main() {
    SetConsoleOutputCP(CP_UTF8);
    Stack stack;
    stack.createStack();
    stack.fillStack();
    stack.printStack();
    return 0;
}
