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

    LinkedList(const T* number, size_t length) {
        for (size_t i = 0; i < length; ++i) {
            push_tail(number[i]);
        }
    }

    ~LinkedList() {
        clear();
    }

    void push_tail(T value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = newNode;
            tail = newNode;
            head->next = head;
            head->prev = head;
        } else {
            newNode->prev = tail;
            newNode->next = head;
            tail->next = newNode;
            head->prev = newNode;
            tail = newNode;
        }
    }

    void clear() {
        if (!head) return;
        Node<T>* current = head;
        do {
            Node<T>* toDelete = current;
            current = current->next;
            delete toDelete;
        } while (current != head);
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

    Node<T>* get_head() const {
        return head;
    }

    Node<T>* get_tail() const {
        return tail;
    }

    bool is_empty() const {
        return head == nullptr;
    }
};

LinkedList<int> add(LinkedList<int>& num1, LinkedList<int>& num2) {
    LinkedList<int> result;
    Node<int>* p1 = num1.get_tail();
    Node<int>* p2 = num2.get_tail();

    int carry = 0;

    do {
        int sum = (p1->data + p2->data + carry);
        result.push_tail(sum % 10);
        carry = sum / 10;

        p1 = (p1 == num1.get_head()) ? num1.get_tail() : p1->prev;
        p2 = (p2 == num2.get_head()) ? num2.get_tail() : p2->prev;
    } while (p1 != num1.get_tail() || p2 != num2.get_tail() || carry != 0);

    return result;
}

LinkedList<int> multiply(LinkedList<int>& num1, LinkedList<int>& num2) {
    if (num1.is_empty() || num2.is_empty()) {
        return LinkedList<int>(new int[1]{0}, 1); // Zero result
    }

    LinkedList<int> result;
    Node<int>* p1 = num1.get_tail();
    int num2_length = 0;

    // First, we count the number of digits in num2
    Node<int>* temp = num2.get_tail();
    do {
        num2_length++;
        temp = (temp == num2.get_head()) ? num2.get_tail() : temp->prev;
    } while (temp != num2.get_tail());

    // Array to store intermediate results based on multipliers
    int* tempResult = new int[num2_length + 1](); // +1 for carry

    int position = 0;
    do {
        int carry = 0;
        Node<int>* p2 = num2.get_tail();
        for (int i = 0; i < position; ++i) {
            tempResult[i] = 0; // Press the carry for the next digit
        }

        do {
            int product = (p1->data * p2->data + carry);
            tempResult[position] += product % 10;
            carry = product / 10;

            p2 = (p2 == num2.get_head()) ? num2.get_tail() : p2->prev;
            position++;
        } while (p2 != num2.get_tail());

        tempResult[position] += carry;
        
        // Now push to result from tempResult
        for (int i = 0; i < num2_length; ++i) {
            result.push_tail(tempResult[i]);
        }
        
        position++;
        p1 = (p1 == num1.get_head()) ? num1.get_tail() : p1->prev;
    } while (p1 != num1.get_tail());

    delete[] tempResult;

    // The final result may require adjustment (to handle carries properly)
    return result;
}

// Оператор вывода для LinkedList
std::ostream& operator<<(std::ostream& os, const LinkedList<int>& list) {
    list.print();
    return os;
}

int main() {
    LinkedList<int> num1(new int[3]{2, 1, 3}, 3); // 213
    LinkedList<int> num2(new int[3]{3, 4, 5}, 3); // 345

    LinkedList<int> sum = add(num1, num2);
    LinkedList<int> product = multiply(num1, num2);

    std::cout << "Sum: " << sum << std::endl;        // Ожидаем 558
    std::cout << "Product: " << product << std::endl; // Ожидаем 73395

    return 0;
}