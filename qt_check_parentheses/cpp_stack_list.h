#include <iostream>
#include <exception>

template <typename T>
class Stack{
public:
    explicit Stack():_size(0){}

    ~Stack(){
        while(!is_empty())
            pop();
    }

    void push(T const& val){
        Node* new_node = new Node(val);
        new_node->next = _top;
        _top = new_node;
        ++_size;
    }

    T pop(){
        if (!is_empty()){
            T result = _top->value;
            Node* next = _top->next;
            delete _top;
            _top = next;
            --_size;
            return result;
        } else {
            throw std::out_of_range("Stack is empty");
        }
    }

    T top(){
        if (!is_empty()){
            return _top->value;
        } else {
            throw std::out_of_range("Stack is empty");
        }
    }

    bool is_empty(){
        return _size == 0;
    }

    size_t size() const{
        return _size;
    }

private:
    struct Node{
        Node(T const& val):value(val){}
        T value;
        Node* next = nullptr;
    };

    Node* _top = nullptr;
    size_t _size;
};
