#include <iostream>
#include <Windows.h>
#include <limits>
#include <string>
#include <sstream>
#include <cctype>
#undef max
using namespace std;


void clearStream() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.sync();
}


struct Node {
    size_t data;
    int prior;
    Node* next;
    Node(size_t inputData, int inputPrior) {
        data = inputData;
        prior = inputPrior;
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
    void pushFront(size_t data, int prior){
        Node* node = new Node(data, prior);
        node->next = head;
        head = node;
        if (tail == nullptr) tail = node;
    }

    Node* getHead(){
        return head;
    }

};

//функции для нотации
int Prior(char c){
    switch (c){
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '(':
        case ')':
            return 0;
        default:
            return -1;
    }
}

void getStr(string &input, Stack &output, Stack &symbols){
    clearStream();
    getline(cin, input);
    string currentNumber = "";
    for (char c : input) {
        if (isdigit(c)) {
            currentNumber += c;
        }
        else if (currentNumber != "") {
            output.pushFront(stoi(currentNumber), -1);
            currentNumber = "";
        }

        if (!isdigit(c)) {
            symbols.pushFront(static_cast<char>(c), Prior(c));
        }
    }

    if (currentNumber != "") {
        output.pushFront(stoi(currentNumber), -1);
    }

    for(Node* ptr = symbols.head; ptr != nullptr; ptr = ptr->next){
        
    }
}



//.....................................................................
int main() {
    SetConsoleOutputCP(CP_UTF8);
    Stack symbols;
    Stack output;
    string input;
    int choise;
    symbols.createStack();
    output.createStack();
    while (true){
        cin >> choise;
        switch (choise){
            case 1:
                getStr(input, output, symbols);

                break;
            case 2:
                output.clearStack();
                symbols.clearStack();
                exit(0);
        }
    }
}
