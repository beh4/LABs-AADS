#include <iostream>
#include <stdexcept>

// Структура для узла двусвязного циклического списка
template<typename T>
struct Node {
    T data;
    Node* prev;
    Node* next;
    
    Node(T val) : data(val), prev(this), next(this) {}
};

// Класс для двусвязного циклического списка
template<typename T>
class LinkedList {
public:
    Node<T>* head;

    LinkedList() : head(nullptr) {}

    ~LinkedList() {
        if (head) {
            Node<T>* current = head->next;
            while (current != head) {
                Node<T>* temp = current;
                current = current->next;
                delete temp;
            }
            delete head;
        }
    }

    // Добавление элемента в конец списка
    void append(T val) {
        if (!head) {
            head = new Node<T>(val);
        } else {
            Node<T>* tail = head->prev;
            Node<T>* newNode = new Node<T>(val);
            tail->next = newNode;
            newNode->prev = tail;
            newNode->next = head;
            head->prev = newNode;
        }
    }

    // Вывод списка (число) в обратном порядке в виде строки
    friend std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list) {
        if (!list.head) return os << "0";
        Node<T>* current = list.head->prev;
        do {
            os << current->data;
            current = current->prev;
        } while (current != list.head->prev);
        return os;
    }
};

// Функция для сложения двух чисел, представленных в виде списков
template<typename T>
LinkedList<T> add(const LinkedList<T>& num1, const LinkedList<T>& num2) {
    LinkedList<T> result;
    Node<T>* p1 = num1.head;
    Node<T>* p2 = num2.head;
    T carry = 0;

    if (p1 == nullptr) return num2;
    if (p2 == nullptr) return num1;

    do {
        T val1 = p1 ? p1->data : 0;
        T val2 = p2 ? p2->data : 0;
        T sum = val1 + val2 + carry;
        carry = sum / 10;
        result.append(sum % 10);
        
        if (p1) p1 = (p1->next == num1.head) ? nullptr : p1->next;
        if (p2) p2 = (p2->next == num2.head) ? nullptr : p2->next;
    } while (p1 || p2 || carry != 0);

    return result;
}

// Функция для умножения двух чисел, представленных в виде списков
template<typename T>
LinkedList<T> multiply(const LinkedList<T>& num1, const LinkedList<T>& num2) {
    LinkedList<T> result;
    if (!num1.head || !num2.head) return result;
    
    int num1_pos = 0;
    for (Node<T>* p1 = num1.head; p1 != nullptr; p1 = (p1->next == num1.head) ? nullptr : p1->next) {
        LinkedList<T> temp;
        T carry = 0;
        
        // Заполнение нулями в начале temp равное позиции узла p1
        for (int i = 0; i < num1_pos; ++i) temp.append(0);

        int num2_pos = 0;
        for (Node<T>* p2 = num2.head; p2 != nullptr; p2 = (p2->next == num2.head) ? nullptr : p2->next, ++num2_pos) {
            T product = p1->data * p2->data + carry;
            carry = product / 10;
            temp.append(product % 10);
        }
        
        if (carry) temp.append(carry);

        result = add(result, temp);
        ++num1_pos;
    }
    return result;
}

// Пример использования
int main() {
    LinkedList<int> num1;
    num1.append(3);
    num1.append(4);
    num1.append(2); // представляет число 243

    LinkedList<int> num2;
    num2.append(4);
    num2.append(6);
    num2.append(5); // представляет число 564

    LinkedList<int> sum = add(num1, num2);
    LinkedList<int> product = multiply(num1, num2);

    std::cout << "Сумма: " << sum << std::endl; // ожидаемый вывод: 807
    std::cout << "Произведение: " << product << std::endl; // ожидаемый вывод: 137052

    return 0;
}