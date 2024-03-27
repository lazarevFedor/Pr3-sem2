#include <iostream>
#include <Windows.h>
#include <limits>
#include <string>
#undef max
using namespace std;


void clearStream() {
    cin.clear();
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cin.sync();
}


void fillString(string &input){
    clearStream();
    getline(cin, input);
}


void mainMenu(){
    cout << "1) Преобразовать выражение\n" <<
    "2) Проверить корректность выражения\n" <<
    "3) Вычислить выражение\n" <<
    "4) Очистить консоль\n" <<
    "5) Выход\n-->> ";
}


void recordingMenu(){
    cout << "1) Обратная польская запись\n" <<
    "2) Польская запись\n" <<
    "3) Простая запись\n-->> ";
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
            if(ptr->data == "("){
                cout << "\nОшибка ввода строки! Отсутстует закрывающая скобка.\n";
                return;
            }
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
            if (isdigit(c)) return -1;
            else if (c >= 'A' && c <= 'Z') return -1;
            else if (c >= 'a' && c <= 'z') return -1;
            else return 404;
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


bool brackets(Node* &ptr, char c, Stack &output, Stack &symbols, string &curr){
    if (c == '('){
        //curr = "";
        curr = '(';
        symbols.pushFront(curr, 0);
        curr = "";
    } else {
        output.pushFront(curr, -1);
        curr = "";
        while (ptr != nullptr && ptr->data[0] != '('){
            output.pushFront(ptr->data, Prior(ptr->data[0]));
            symbols.popFront();
            ptr = symbols.head;
            if (ptr == nullptr){
                cout << "\nОшибка ввода строки! Открывающая скобка не найдена.\n";
                return false;
            }
        }
        symbols.popFront();
    }
}


bool convertToRPN(string &input, Stack &output, Stack &symbols) {
    //clearStream();
    //getline(cin, input);
    fillString(input);
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
                if (brackets(ptr, input[i], output, symbols, curr)) break;
                return false;
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
    return true;
}


void clearStacks(Stack &symbols, Stack &output){
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
            cout << "\nОшибка ввода строки!\n";
    }
}

bool resultRPN(string &input, Stack &output){
    input = "";
    string curr = "";
    fillString(input);
    int result;
    for (int i = 0; i < input.length(); i++){
        if (input[i] != ' '){
            switch(Prior(input[i])){
                case -1:
                    curr.push_back(input[i]);
                    break;
                case 1:
                case 2:
                    if (output.head == output.tail){
                        cout << "\nОшибка ввода строки! Недостаточно операндов.\n";
                        return false;
                    }
                    result = stoi(output.getHead()->data);
                    output.popFront();
                    result = operation(stoi(output.getHead()->data), result, input[i]);
                    output.popFront();
                    curr = to_string(result);
                    output.pushFront(curr, -1);
                    curr = "";
                    break;
                case 404:
                    cout << "\nОшибка ввода строки! Неправильно введен символ.\n";
                    return false;
            }
        } else{
            if (!curr.empty()){
                output.pushFront(curr, -1);
                curr = "";
            }
        }
    }
    if (output.getHead() != output.tail){
        cout << "\nОшибка ввода! Недостаточно операций.\n";
        return false;
    }
    return true;
}

//.....................................................................
int main() {
    SetConsoleOutputCP(CP_UTF8);

    Stack symbols, output;
    string input;
    short int choise;
    symbols.createStack();
    output.createStack();

    while (true){
        mainMenu();
        cin >> choise;
        switch (choise){
            case 1:
                clearStacks(symbols, output);
                recordingMenu();
                cin >> choise;
                if (choise == 1){
                    if (convertToRPN(input, output, symbols)) output.printOutput();
                }
                else if (choise == 2){
                    //convertToPN
                }
                else cout << "Неправильно введён номер!\n";
                break;
            case 2:
                clearStacks(symbols, output);
                recordingMenu();
                cin >> choise;
                if (choise == 1){
                    fillString(input);
                    if (resultRPN(input, output)){
                        cout << "\nВыражение корректно.\n";
                    }
                    else cout << "\nВыражение не корректно.\n";
                }
                else if (choise == 2){
                    fillString(input);
                    //resultPN
                }
                else if (choise == 3){
                    fillString(input);
                    convertToRPN(input, output, symbols);
                    clearStacks(symbols, output);
                    if (resultRPN(input, output)){
                        cout << "\nВыражение корректно.\n";
                    }
                    else cout << "\nВыражение не корректно.\n";
                }
                else cout << "Неправильно введён номер!\n";
                break;
            case 3:
                clearStacks(symbols, output);
                recordingMenu();
                cin >> choise;
                if (choise == 1){
                    if (resultRPN(input, output)){
                        cout << "\nРезультат вычислений: " << output.head->data;
                    }
                }
                else if (choise == 2){
                    //resultPN
                }
                else if (choise == 3){
                    fillString(input);
                    clearStacks(symbols, output);
                    convertToRPN(input, output, symbols);
                    clearStacks(symbols, output);
                    if (resultRPN(input, output)){
                        cout << "\nРезультат вычислений: " << output.head->data;
                    }
                }
                else cout << "Неправильно введён номер!\n";
                break;
            case 4:
                system("cls");
                break;
            case 5:
                clearStacks(symbols, output);
                exit(0);
            default:
                cout << "Неправильно введён номер!\n";
        }
        cout << "\n";
    }
}
