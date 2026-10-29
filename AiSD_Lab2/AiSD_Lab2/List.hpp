#include <iostream>
#include <stdexcept>

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;
};

template <typename T>
struct List {

    Node<T>* head = NULL;
    int _size = 0;

    void add(T elem) {
        add(size(), elem);
    }

    void add(int index, T elem) {
        if (index >= 0 && index <= size()) {

            if (head == NULL) {
                if (index > 0) {
                    throw std::runtime_error("Out of bounds");
                }
                else {
                    head = new Node<T>();
                    head->data = elem;
                    head->next = NULL;
                    head->prev = NULL;
                    _size++;
                }
            }
            else if (index == 0) {
                Node<T>* newNode = new Node<T>();
                newNode->data = elem;
                newNode->next = head;
                newNode->prev = NULL;
                head->prev = newNode;
                head = newNode;
                _size++;
            }
            else {
                Node<T>* node = findNode(index - 1);
                Node<T>* newNode = new Node<T>();
                newNode->data = elem;
                newNode->prev = node;
                newNode->next = node->next;
                if (node->next != NULL) {
                    node->next->prev = newNode;
                }
                node->next = newNode;
                _size++;
            }
        }
        else {
            throw std::runtime_error("Out of bounds");
        }
    }

    T remove(int index) {
        if (index < 0  index >= _size) {
            throw std::runtime_error("Out of bounds");
        }
        if (index == 0) {
            Node<T>* del = head;
            head = del->next;
            if (head != NULL)
            {
                head->prev = NULL;
            }
            T data = del->data;
            delete del;
            _size--;
            return data;
        }
        else
        {
            Node<T>* node = findNode(index - 1);
            Node<T>* del = node->next;

            node->next = del->next;
            if (del->next != NULL)
            {
                del->next->prev = node;
            }
            T data = del->data;
            delete del;
            _size--;
            return data;
        }
    }

    T find(int index) {
        Node<T>* node = findNode(index);
        if (node != NULL) {
            return node->data;
        }
        else {
            throw std::runtime_error("Element not found");
        }
    }



    void print() {
        Node<T>* current = head;

        while (current != NULL) {
            std::cout << current->data;

            if (current->next != NULL) {
                std::cout << ", ";
            }

            current = current->next;
        }

        std::cout << std::endl;
    }



    Node<T>* findNode(int index) {
        if (0 > index  index >= _size) {
            throw std::runtime_error("Out of bounds");
        }
        Node<T>* current = head;
        int count = 0;
        while (current != NULL && count < index) {
            current = current->next;
            count++;
        }
        return current;
    }

    int size() {
        if (head == NULL) {
            return 0;
        }
        else {
            return _size;
        }
    }

    ~List() {
        while (head != NULL) {
            Node<T>* del = head;
            head = head->next;
            delete del;
        }
    }
};

template <typename T>
struct ListIterator {

    Node<T>* cur;

    ListIterator(List<T>* l) {
        cur = l->head;
    }

    bool hasNext() {
        return cur != NULL;
    }

    T next() {
        Node<T>* next = cur;
        cur = cur->next;
        return next->data;
    }

};