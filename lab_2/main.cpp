#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

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

    void clear() {
        if (head) {
            Node<T>* current = head;
            do {
                Node<T>* toDelete = current;
                current = current->next;
                delete toDelete;
            } while (current != head);
        }
        head = tail = nullptr;
    }

public:
    LinkedList() : head(nullptr), tail(nullptr) {}

    LinkedList(const LinkedList<T>& other) : head(nullptr), tail(nullptr) {
        if (other.head) {
            Node<T>* current = other.head;
            do {
                push_tail(current->data);
                current = current->next;
            } while (current != other.head);
        }
    }

    LinkedList(int size, unsigned int seed) : head(nullptr), tail(nullptr) {
        std::srand(seed);
        for (int i = 0; i < size; ++i) {
            push_tail(std::rand() % 100);
        }
    }

    ~LinkedList() {
        clear();
    }

    Node<T>* getHead() const {
        return head;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this != &other) {
            clear();
            if (other.head) {
                Node<T>* current = other.head;
                do {
                    push_tail(current->data);
                    current = current->next;
                } while (current != other.head);
            }
        }
        return *this;
    }

    void push_tail(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = tail = newNode;
            head->next = head->prev = head;
        }
        else {
            newNode->prev = tail;
            newNode->next = head;
            tail->next = newNode;
            head->prev = newNode;
            tail = newNode;
        }
    }

    void push_head(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        if (!head) {
            head = tail = newNode;
            head->next = head->prev = head;
        }
        else {
            newNode->next = head;
            newNode->prev = tail;
            tail->next = newNode;
            head->prev = newNode;
            head = newNode;
        }
    }

    void pop_head() {
        if (!head) throw std::runtime_error("Список пуст");
        if (head == tail) {
            delete head;
            head = tail = nullptr;
        }
        else {
            Node<T>* toDelete = head;
            head = head->next;
            head->prev = tail;
            tail->next = head;
            delete toDelete;
        }
    }

    void pop_tail() {
        if (!tail) throw std::runtime_error("Список пуст");
        if (head == tail) {
            delete tail;
            head = tail = nullptr;
        }
        else {
            Node<T>* toDelete = tail;
            tail = tail->prev;
            tail->next = head;
            head->prev = tail;
            delete toDelete;
        }
    }

    void push_tail(const LinkedList<T>& other) {
        if (other.head) {
            Node<T>* current = other.head;
            do {
                push_tail(current->data);
                current = current->next;
            } while (current != other.head);
        }
    }

    void push_head(const LinkedList<T>& other) {
        if (other.head) {
            Node<T>* current = other.tail;
            do {
                push_head(current->data);
                current = current->prev;
            } while (current != other.tail);
        }
    }

    void delete_node(const T& value) {
        if (!head) return;
        Node<T>* current = head;
        do {
            if (current->data == value) {
                Node<T>* toDelete = current;
                if (current == head) {
                    pop_head();
                    current = head;
                }
                else if (current == tail) {
                    pop_tail();
                    current = nullptr;
                }
                else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                    current = current->next;
                    delete toDelete;
                }
            }
            else {
                current = current->next;
            }
        } while (current != head && current != nullptr);
    }


    T& operator[](size_t index) {
        if (index < 0 || !head) throw std::out_of_range("Out of index");
        Node<T>* current = head;
        size_t count = 0;
        do {
            if (count == index) return current->data;
            current = current->next;
            count++;
        } while (current != head);
        throw std::out_of_range("Out of index");
    }

    const T& operator[](size_t index) const {
        return const_cast<const T&>(operator[](index));
    }

    friend std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list) {
        if (!list.head) {
            return os;
        }
        Node<T>* current = list.head;
        do {
            os << current->data << " ";
            current = current->next;
        } while (current != list.head);
        return os;
    }

    void reverse() {
        if (!head || head->next == head) {
            return;
        }

        Node<T>* current = head;
        Node<T>* temp = nullptr;
        do {
            temp = current->prev;
            current->prev = current->next;
            current->next = temp;
            current = current->prev;
        } while (current != head);

        head = temp->prev;
    }
};

LinkedList<int> add(const LinkedList<int>& num1, const LinkedList<int>& num2) {
    LinkedList<int> result;
    Node<int>* p1 = num1.getHead();
    Node<int>* p2 = num2.getHead();
    int carry = 0;

    if (p1 == nullptr) return num2;
    if (p2 == nullptr) return num1;

    do {
        int val1 = p1 ? p1->data : 0;
        int val2 = p2 ? p2->data : 0;
        int sum = val1 + val2 + carry;
        carry = sum / 10;
        result.push_tail(sum % 10);

        if (p1) p1 = (p1->next == num1.getHead()) ? nullptr : p1->next;
        if (p2) p2 = (p2->next == num2.getHead()) ? nullptr : p2->next;
    } while (p1 || p2 || carry != 0);

    return result;
}

LinkedList<int> mult(const LinkedList<int>& num1, const LinkedList<int>& num2) {
    LinkedList<int> result;
    if (num1.isEmpty() || num2.isEmpty()) {
        result.push_tail(0);
        return result;
    }
    long long number1 = 0;
    Node<int>* p1 = num1.getHead();
    do {
        number1 = number1 * 10 + p1->data;
        p1 = p1->next;
    } while (p1 != num1.getHead());

    long long number2 = 0;
    Node<int>* p2 = num2.getHead();
    do {
        number2 = number2 * 10 + p2->data;
        p2 = p2->next;
    } while (p2 != num2.getHead());

    long long product = number1 * number2;

    if (product == 0) {
        result.push_tail(0);
    }
    else {
        while (product > 0) {
            result.push_tail(product % 10);
            product /= 10;
        }
    }
    result.reverse();
    return result;
}

int main() {
    LinkedList<int> list1;
    list1.push_tail(1);
    list1.push_tail(2);
    list1.push_tail(3);

    LinkedList<int> list2(5, 42);
    std::cout << list2 << std::endl;

    LinkedList<int> list3 = list2;
    std::cout << list3 << std::endl;

    list1.push_head(0);
    std::cout << list1 << std::endl;

    list1.pop_tail();
    std::cout << list1 << std::endl;

    list1.delete_node(2);
    std::cout << list1 << std::endl;

    list1.push_tail(list2);
    std::cout << list1 << std::endl;
    list1.push_head(list2);
    std::cout << list1 << std::endl;

    try {
        std::cout << "Element 1: " << list1[1] << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    LinkedList<int> list4;
    list4.push_tail(4);
    list4.push_tail(3);
    std::cout << list4 << std::endl;

    LinkedList<int> list5;
    list5.push_tail(2);
    list5.push_tail(1);
    std::cout << list5 << std::endl;

    LinkedList<int> sum = add(list4, list5);
    LinkedList<int> product = mult(list4, list5);

    std::cout << sum << std::endl;
    std::cout << product << std::endl;

    return 0;
}