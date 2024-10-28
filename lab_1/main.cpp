#include <iostream>
#include <iomanip>
#include <utility>
#include <cmath>
#include <stdexcept>
#include <random>
#include <cassert>

static const double EPSILON = 0.0001f;

template<typename T>
class UniqueSet {
private:
    T* elements;
    size_t size;
    size_t capacity;
    

    void expand() {
        capacity *= 2;
        T* new_elements = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            new_elements[i] = elements[i];
        }
        delete[] elements;
        elements = new_elements;
    }

    bool isEqual(const T& a, const T& b) const {
        if constexpr (std::is_floating_point<T>::value) {
            return std::fabs(a - b) < EPSILON;
        }
        else {
            return a == b;
        }
    }

public:
    UniqueSet() : size(0), capacity(10) {
        elements = new T[capacity];
    }

    UniqueSet(const T* arr, size_t n) : size(0), capacity(n > 10 ? n : 10) {
        elements = new T[capacity];
        for (size_t i = 0; i < n; ++i) {
            add(arr[i]);
        }
    }

    UniqueSet(size_t n, float min, float max) : size(0), capacity(n > 10 ? n : 10) {
        elements = new T[capacity];
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(min, max);

        for (size_t i = 0; i < n; ++i) {
            add(dis(gen));
        }
    }

    UniqueSet(const UniqueSet& other) : size(other.size), capacity(other.capacity) {
        elements = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            elements[i] = other.elements[i];
        }
    }

    ~UniqueSet() {
        delete[] elements;
    }

    size_t getSize() const { return size; }

    T& operator[](size_t index) {
        if (index >= size) throw std::out_of_range("Index out of range");
        return elements[index];
    }

    UniqueSet operator+(const UniqueSet& other) const {
        UniqueSet result(*this);
        for (size_t i = 0; i < other.size; ++i) {
            result.add(other.elements[i]);
        }
        return result;
    }

    UniqueSet operator-(const UniqueSet& other) const {
        UniqueSet result;
        for (size_t i = 0; i < size; ++i) {
            if (!other.contains(elements[i])) {
                result.add(elements[i]);
            }
        }
        return result;
    }

    UniqueSet& operator+=(const T& value) {
        add(value);
        return *this;
    }

    UniqueSet& operator-=(const T& value) {
        remove(value);
        return *this;
    }

    void add(const T& value) {
        if (!contains(value)) {
            if (size >= capacity) {
                expand();
            }
            elements[size++] = value;
        }
    }

    void remove(const T& value) {
        for (size_t i = 0; i < size; ++i) {
            if (isEqual(elements[i], value)) {
                elements[i] = elements[--size];
                return;
            }
        }
    }

    bool contains(const T& value) const {
        for (size_t i = 0; i < size; ++i) {
            if (isEqual(elements[i], value)) {
                return true;
            }
        }
        return false;
    }

    UniqueSet intersection(const UniqueSet& other) const {
        UniqueSet result;
        for (size_t i = 0; i < size; ++i) {
            if (other.contains(elements[i])) {
                result.add(elements[i]);
            }
        }
        return result;
    }

    bool operator==(const UniqueSet& other) const {
        if (size != other.size) return false;
        for (size_t i = 0; i < size; ++i) {
            if (!other.contains(elements[i])) return false;
        }
        return true;
    }

    bool operator!=(const UniqueSet& other) const {
        return !(*this == other);
    }

    void print() const {
        for (size_t i = 0; i < size; ++i) {
            std::cout << std::fixed << std::setprecision(2) << elements[i] << " ";
        }
    }
};

void testUniqueSet() {
    // Создаем первое множество
    UniqueSet<int> set1;
    set1 += (1);
    set1 += (2);
    set1 += (3);
    std::cout << "Set 1: ";
    set1.print();
    std::cout << "\n";

    // Создаем второе множество
    UniqueSet<int> set2;
    set2 += (3);
    set2 += (4);
    set2 += (5);
    std::cout << "Set 2: ";
    set2.print();
    std::cout << "\n";

    // Обращение по индексу
    std::cout << "Element at the given index: " << set1[1] << std::endl;
    
    // Объединение множеств
    UniqueSet<int> unionSet = set1 + set2;
    std::cout << "Union of Set 1 and Set 2: ";
    unionSet.print();
    std::cout << "\n";

    // Разность множеств
    UniqueSet<int> differenceSet = set1 - set2;
    std::cout << "Difference of Set 1 and Set 2: ";
    differenceSet.print();
    std::cout << "\n";

    // Пересечение множеств
    UniqueSet<int> intersectionSet = set1.intersection(set2);
    std::cout << "Intersection of Set 1 and Set 2: ";
    intersectionSet.print();
    std::cout << "\n";

    // Сравнение множеств
    std::cout << "Set 1 and Set 2 are equal: " << (set1 == set2 ? "True" : "False") << std::endl;

    // Проверка наличия в множестве
    std::cout << "Set 1: " << (set1.contains(2) ? "True" : "False") << std::endl;

    // Добавляем повторный элемент
    set1 += (2);
    std::cout << "After adding 2 to Set 1 again: ";
    set1.print();
    std::cout << "\n";

    // Реализация задания 1
    std::cout << "Task1: ";
    UniqueSet<int> taskSet = unionSet - intersectionSet;
    taskSet.print();
    std::cout << "\n";
}

int main() {
    testUniqueSet();
    return 0;
}