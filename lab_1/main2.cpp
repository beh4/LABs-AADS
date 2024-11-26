#include <iostream>
#include <stdexcept>

template<typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;

    Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

template<typename T>
class LinkedList {
private:
    Node<T>* head;
    Node<T>* tail;

public:
    LinkedList() : head(nullptr), tail(nullptr) {}

    ~LinkedList() {
        clear();
    }

    void push_tail(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = tail = newNode;
            head->next = head->prev = head;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
            tail = newNode;
        }
    }

    void clear() {
        while (head) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            if (head == temp) break; // якщо остался один элемент
        }
        head = tail = nullptr;
    }

    void print() const {
        if (!head) {
            std::cout << "0";
            return;
        }
        Node<T>* current = head;
        do {
            std::cout << current->data;
            current = current->next;
        } while (current != head);
    }

    // Позволяет получить голову списка
    Node<T>* getHead() const {
        return head;
    }

    // Получить размер списка
    int size() const {
        if (!head) return 0;

        Node<T>* current = head;
        int count = 0;
        do {
            count++;
            current = current->next;
        } while (current != head);
        return count;
    }
};

// Функция для сложения двух чисел, представленных в виде связных списков
LinkedList<int> addNumbers(const LinkedList<int>& num1, const LinkedList<int>& num2) {
    LinkedList<int> result;
    Node<int>* p1 = num1.getHead();
    Node<int>* p2 = num2.getHead();
    
    int carry = 0;
    int size1 = num1.size();
    int size2 = num2.size();
    int maxSize = std::max(size1, size2);
    
    for (int i = 0; i < maxSize || carry; ++i) {
        int digit1 = (i < size1) ? p1->data : 0;
        int digit2 = (i < size2) ? p2->data : 0;
        
        int sum = digit1 + digit2 + carry;
        carry = sum / 10;
        result.push_tail(sum % 10);

        if (i < size1) p1 = p1->next;
        if (i < size2) p2 = p2->next;
    }
    
    return result;
}

// Функция для умножения двух чисел, представленных в виде связных списков
LinkedList<int> multiplyNumbers(const LinkedList<int>& num1, const LinkedList<int>& num2) {
    LinkedList<int> result;
    if (!num1.getHead() || !num2.getHead()) {
        result.push_tail(0);
        return result;
    }

    Node<int>* p1 = num1.getHead();
    Node<int>* p2 = num2.getHead();
    
    int size1 = num1.size();
    int size2 = num2.size();
    int carry = 0;
    
    for (int i = 0; i < size2; ++i) {
        carry = 0;
        LinkedList<int> tempResult;
        
        // Умножаем каждую цифру второго числа на первое
        for (int j = 0; j < i; ++j) {
            tempResult.push_tail(0);  // Добавляем ведущие нули
        }

        Node<int>* currentP1 = p1;
        for (int j = 0; j < size1; ++j) {
            int product = (currentP1->data) * (p2->data) + carry;
            carry = product / 10;
            tempResult.push_tail(product % 10);
            currentP1 = currentP1->next;
        }
        if (carry) {
            tempResult.push_tail(carry);
        }

        // Добавляем результаты частичных произведений
        result = addNumbers(result, tempResult);
        p2 = p2->next;
    }

    return result;
}

// Оператор вывода для LinkedList
std::ostream& operator<<(std::ostream& os, const LinkedList<int>& list) {
    list.print();
    return os;
}

int main() {
    LinkedList<int> num1;
    num1.push_tail(1);
    num1.push_tail(2); // Число 21

    LinkedList<int> num2;
    num2.push_tail(3);
    num2.push_tail(4); // Число 43

    LinkedList<int> sum = addNumbers(num1, num2);
    LinkedList<int> product = multiplyNumbers(num1, num2);

    std::cout << "Сумма: " << sum << std::endl;         // Ожидаем 64
    std::cout << "Произведение: " << product << std::endl; // Ожидаем 903

    return 0;
}