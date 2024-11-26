#include <iostream>

// Структура для узла двусвязного циклического списка
struct Node {
    int data;
    Node* next;
    Node* prev;

    Node(int value) : data(value), next(this), prev(this) {}
};

// Функция для добавления узла в конец списка
void append(Node*& head, int data) {
    Node* newNode = new Node(data);
    if (!head) {
        head = newNode;
        return;
    }
    Node* tail = head->prev;
    tail->next = newNode;
    newNode->prev = tail;
    newNode->next = head;
    head->prev = newNode;
}

// Функция для вывода числа, представленного списком
void printList(Node* head) {
    if (!head) return;
    Node* temp = head;
    do {
        std::cout << temp->data;
        temp = temp->next;
    } while (temp != head);
    std::cout << std::endl;
}

// Функция для сложения двух чисел, представленных списками
Node* addNumbers(Node* num1, Node* num2) {
    Node *result = nullptr, *tail1 = num1->prev, *tail2 = num2->prev;
    int carry = 0;
    do {
        int sum = tail1->data + tail2->data + carry;
        carry = sum / 10;
        append(result, sum % 10);
        tail1 = tail1->prev;
        tail2 = tail2->prev;
    } while (tail1 != num1->prev && tail2 != num2->prev);
    // Если какие-то числа закончились, но всё ещё есть перенос
    while (tail1 != num1->prev) {
        int sum = tail1->data + carry;
        carry = sum / 10;
        append(result, sum % 10);
        tail1 = tail1->prev;
    }
    while (tail2 != num2->prev) {
        int sum = tail2->data + carry;
        carry = sum / 10;
        append(result, sum % 10);
        tail2 = tail2->prev;
    }
    if (carry > 0) {
        append(result, carry);
    }
    return result;
}

// Функция для умножения двух чисел, представленных списками
Node* multiplyNumbers(Node* num1, Node* num2) {
    Node* result = nullptr;
    Node* tail2 = num2->prev;
    int shift = 0;

    do {
        Node* product(nullptr);
        int carry = 0;
        for (Node* tail1 = num1->prev; tail1 != num1; tail1 = tail1->prev) {
            int mul = tail1->data * tail2->data + carry;
            carry = mul / 10;
            append(product, mul % 10);
        }
        if (carry > 0) {
            append(product, carry);
        }
        for (int i = 0; i < shift; ++i) {
            append(product, 0);
        }
        if (!result) {
            result = product;
        } else {
            result = addNumbers(result, product);
        }
        tail2 = tail2->prev;
        ++shift;
    } while (tail2 != num2->prev);

    return result;
}

// Тестовая функция
int main() {
    // Создание первого числа
    Node* num1 = nullptr;
    append(num1, 3);
    append(num1, 2);
    append(num1, 1);  // 123

    // Создание второго числа
    Node* num2 = nullptr;
    append(num2, 6);
    append(num2, 5);
    append(num2, 4);  // 456

    // Сложение чисел
    Node* sum = addNumbers(num1, num2);
    std::cout << "Сумма: ";
    printList(sum);

    // Умножение чисел
    Node* product = multiplyNumbers(num1, num2);
    std::cout << "Произведение: ";
    printList(product);

    return 0;
}