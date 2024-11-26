#include <iostream>

template<typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;
    Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

template<typename T>
class LinkedList {
public:
    Node<T>* head;
    Node<T>* tail;

    LinkedList() : head(nullptr), tail(nullptr) {}

    void push_tail(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = newNode;
            tail = newNode;
            head->next = head;
            head->prev = head;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
            tail = newNode;
        }
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

    // Деструктор для освобождения памяти
    ~LinkedList() {
        if (head) {
            Node<T>* current = head;
            do {
                Node<T>* toDelete = current;
                current = current->next;
                delete toDelete;
            } while (current != head);
        }
    }
};

// Функция для сложения двух двусвязныхциклических списков
LinkedList<int> addLists(const LinkedList<int>& a, const LinkedList<int>& b) {
    LinkedList<int> result;
    Node<int>* nodeA = a.head;
    Node<int>* nodeB = b.head;

    int carry = 0;

    // Дополним списки нулями до одной длины
    do {
        int sum = carry;
        if (nodeA) {
            sum += nodeA->data;
            nodeA = nodeA->next;
        }
        if (nodeB) {
            sum += nodeB->data;
            nodeB = nodeB->next;
        }
        carry = sum / 10; // перенос
        result.push_tail(sum % 10); // добавляем младший разряд
    } while (nodeA != a.head || nodeB != b.head || carry != 0);

    return result;
}

// Функция для умножения двух двусвязных циклических списков
LinkedList<int> multiplyLists(const LinkedList<int>& a, const LinkedList<int>& b) {
    LinkedList<int> result;

    Node<int>* nodeB = b.head;
    int bIndex = 0;

    // Умножаем каждый цвет из второго списка на первый и сдвигаем
    do {
        LinkedList<int> tempResult;
        Node<int>* nodeA = a.head;
        int carry = 0;

        // Умножаем текущую цифру b на все цифры a
        do {
            int product = (nodeA->data * nodeB->data) + carry;
            carry = product / 10;
            tempResult.push_tail(product % 10);
            nodeA = nodeA->next;
        } while (nodeA != a.head);
        
        // Если остался перенос из последнего умножения, добавляем его
        if (carry > 0) {
            tempResult.push_tail(carry);
        }

        // Сдвигаем временный результат влево
        for (int i = 0; i < bIndex; ++i) {
            tempResult.push_tail(0);
        }
        
        // Добавляем временный результат к окончательному результату
        result = addLists(result, tempResult);

        nodeB = nodeB->next;
        bIndex++;
    } while (nodeB != b.head);

    return result;
}

// Оператор вывода для двусвязного списка
std::ostream& operator<<(std::ostream& os, const LinkedList<int>& list) {
    list.print();
    return os;
}

// Пример использования
int main() {
    LinkedList<int> num1;
    LinkedList<int> num2;

    // Пример: число 123456
    num1.push_tail(6);
    num1.push_tail(5);
    num1.push_tail(4);
    num1.push_tail(3);
    num1.push_tail(2);
    num1.push_tail(1);

    // Пример: число 789
    num2.push_tail(9);
    num2.push_tail(8);
    num2.push_tail(7);

    std::cout << "Number 1: " << num1 << std::endl;
    std::cout << "Number 2: " << num2 << std::endl;

    LinkedList<int> sum = addLists(num1, num2);
    LinkedList<int> product = multiplyLists(num1, num2);

    std::cout << "Sum: " << sum << std::endl;
    std::cout << "Product: " << product << std::endl;

    return 0;
}