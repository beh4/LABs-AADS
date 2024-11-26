#include <iostream>
#include <vector>
#include <algorithm>

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
            if (head == temp) break; // если остался один элемент
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

    // Получение головы списка
    Node<T>* getHead() const {
        return head;
    }

    // Умножение двух двусвязных списков
    LinkedList<T> multiply(const LinkedList<T>& other) const {
        LinkedList<T> result;
        if (!this->head || !other.head) {
            result.push_tail(0);
            return result;
        }

        Node<T>* thisCurrent = this->head;
        Node<T>* otherCurrent = other.head;

        std::vector<int> res(this->size() + other.size(), 0);

        int thisIndex = 0;
        do {
            int carry = 0;
            int otherIndex = 0;
            do {
                int product = thisCurrent->data * otherCurrent->data + carry + res[thisIndex + otherIndex];
                carry = product / 10;
                res[thisIndex + otherIndex] = product % 10;
                otherIndex++;
                otherCurrent = otherCurrent->next;
            } while (otherCurrent != other.head);
            if (carry > 0) {
                res[thisIndex + otherIndex] += carry;
            }
            thisIndex++;
            thisCurrent = thisCurrent->next;
            otherCurrent = other.head;
        } while (thisCurrent != this->head);

        // Удаление ведущих нулей
        while (res.size() > 1 && res.back() == 0) {
            res.pop_back();
        }

        // Создание результата
        for (auto it = res.rbegin(); it != res.rend(); ++it) {
            result.push_tail(*it);
        }

        return result;
    }

    // Подсчет размера списка
    int size() const {
        if (!head) return 0;
        int count = 0;
        Node<T>* current = head;
        do {
            count++;
            current = current->next;
        } while (current != head);
        return count;
    }
};

int main() {
    LinkedList<int> num1;
    LinkedList<int> num2;

    // Пример заполнения: 123 (1 -> 2 -> 3)
    num1.push_tail(3);
    num1.push_tail(2);
    num1.push_tail(1);

    // Пример заполнения: 456 (4 -> 5 -> 6)
    num2.push_tail(6);
    num2.push_tail(5);
    num2.push_tail(4);

    LinkedList<int> result = num1.multiply(num2);

    std::cout << "Product: ";
    result.print(); // Должно вывести произведение 123 * 456 = 56088

    return 0;
}