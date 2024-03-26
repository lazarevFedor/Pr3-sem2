#include <iostream>
#include <Windows.h>
#include <limits>
#include <string>
#include <sstream>
#undef max
using namespace std;


void clearStream() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.sync();
}


struct Node {
    string data;
    int prior;
    Node* next;
    Node(string inputData, int inputPrior) {
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
    void pushFront(string data, int prior){
        Node* node = new Node(data, prior);
        node->next = head;
        head = node;
        if (tail == nullptr) tail = node;
    }

    Node* getHead(){
        return head;
    }

    void printOutput(){
        string str;
        for (Node *ptr = head; ptr != nullptr; ptr = ptr->next) {
            str = ptr->data + " " + str;
        }
        cout << "\nВыходная строка: " << str << "\n";
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


void checkSymbol(Node* &ptr, char c, Stack &output, Stack &symbols, string &curr){
    if(ptr == nullptr || ptr->prior < Prior(c)){
        if (!curr.empty()){
            output.pushFront(curr, -1);
            curr = "";
        }
        curr.push_back(c);
        symbols.pushFront(curr, Prior(c));
        curr = "";
    } else {
        if (!curr.empty()){
            output.pushFront(curr, -1);
            curr = "";
        }
        while (ptr != nullptr && ptr->prior >= Prior(c)){
            output.pushFront(ptr->data, Prior(ptr->data[0]));
            symbols.popFront();
            ptr = symbols.head;
        }
        checkSymbol(ptr, c, output, symbols, curr);
    }
}


void brackets(Node* &ptr, char c, Stack &output, Stack &symbols, string &curr){
    if (c == '('){
        //output.pushFront(curr, -1);
        curr = "";
        curr = '(';
        //curr.push_back(c);
        symbols.pushFront(curr, 0);
        curr = "";
    } else {
        output.pushFront(curr, -1);
        curr = "";
        while (ptr != nullptr && ptr->data[0] != '('){
            output.pushFront(ptr->data, Prior(ptr->data[0]));
            symbols.popFront();
            ptr = symbols.head;
        }
        symbols.popFront();
    }
}


void getStr(string &input, Stack &output, Stack &symbols) {
    clearStream();
    getline(cin, input);
    string curr = "";
    Node *ptr = nullptr;
    for (int i = 0; i < input.length(); i++) {
        switch (Prior(input[i])) {
            case -1:
                curr.push_back(input[i]);
                break;
            case 1:
            case 2:
                ptr = symbols.getHead();
                checkSymbol(ptr, input[i], output, symbols, curr);
                break;
            case 0:
                ptr = symbols.getHead();
                brackets(ptr, input[i], output, symbols, curr);
                break;
        }
    }
    if(!curr.empty()){
        output.pushFront(curr, -1);
    }
    if (symbols.getHead() != nullptr){
        for (Node* p = symbols.head; p != nullptr; p = p->next){
            output.pushFront(p->data, -1);
        }
    }
}


void clearAll(Stack &symbols, Stack &output){
    output.clearStack();
    symbols.clearStack();
}

//.....................................................................


int operation(int a, int b, char c){
    switch (c){
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            cout << "\nНеправильный ввод строки!\n";
    }
}

int resultPN(string &input, Stack &output){
    clearStream();
    input = "";
    Node *ptr = nullptr;
    string curr = "";
    getline(cin, input);
    int result;
    for (int i = 0; i< input.length(); i++){
        if (input[i] != ' '){
            switch(Prior(input[i])){
                case -1:
                    curr.push_back(input[i]);
                    break;
                case 1:
                case 2:
                    result = stoi(output.getHead()->data);
                    output.popFront();
                    result = operation(stoi(output.getHead()->data), result, input[i]);
                    output.popFront();
                    curr = to_string(result);
                    output.pushFront(curr, -1);
                    curr = "";
                    break;
            }
        } else{
            if (!curr.empty()){
                output.pushFront(curr, -1);
                curr = "";
            }
        }
    }
    return result;
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
        cout << "Введите номер: ";
        cin >> choise;
        switch (choise){
            case 1:
                clearAll(symbols, output);
                getStr(input, output, symbols);
                output.printOutput();
                break;
            case 2:
                clearAll(symbols, output);
                cout << "\n" << resultPN(input, output) << "\n";
                break;
            case 3:
                clearAll(symbols, output);
                exit(0);
        }
        cout << "\n";
    }
}
