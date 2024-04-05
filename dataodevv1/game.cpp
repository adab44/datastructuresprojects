


#include <iostream>
#include "doublelinklist.h"
#include "objects.h"

using namespace std;
//made all those structures from the lecture to not use STL lib.
template <typename T>
class Node2 
{ // Node class for a single linked list
public:
    T data;
    Node2* next;

    Node2(T val) : data(val), next(nullptr) {} // Constructor
};

template <typename T>
class List 
{ // List class using Node2
public:
    Node2<T>* head;

    List() : head(nullptr) {} // Constructor

    ~List() 
    { // Destructor to clear 
        clear();
    }

    void addFront(const T& value) 
    { // Add a new element to the front 
        Node2<T>* newNode = new Node2<T>(value);
        newNode->next = head;
        head = newNode;
    }

    T removeFront() 
    { // Remove and return the front element 
        if (head == nullptr) {
            throw out_of_range("List is empty");
        }
        Node2<T>* oldHead = head;
        head = head->next;
        T value = oldHead->data;
        delete oldHead;
        return value;
    }

    bool isEmpty() const 
    { // Check if the list is empty
        return head == nullptr;
    }

    void clear() { // Clear the list
        while (!isEmpty()) 
        {
            removeFront();
        }
    }
};

template <typename T>
class Stack 
{ // Stack class using the List
private:
    List<T> data;
    int elemcount;

public:
    Stack() : elemcount(0) {} // Constructor

    bool isEmpty() const 
    { // Check if the stack is empty
        return data.isEmpty();
    }

    void push(const T& e) 
    { // Push an element to the stack
        data.addFront(e);
        elemcount++;
    }

    T pop() 
    { // Pop and return the top element of the stack
        if (isEmpty()) 
        {
            throw out_of_range("Stack is empty");
        }
        elemcount--;
        return data.removeFront();
    }

    T& top() 
    { // Get the top element of the stack
        if (isEmpty()) 
        {
            throw out_of_range("Stack is empty");
        }
        return data.head->data;
    }
};


// Structure to represent the states from the game
class State {
public:
    GameState gameState;
    int action;
    int object;
    int room_id;

    // Constructor
    State(const GameState& gs, int act, int obj, int room)
        : gameState(gs), action(act), object(obj), room_id(room) {}

};


void dfs() 
{ // Depthfirst search function to find a winning path
    Stack<State> states; // Stack to hold the states for DFS
    GameState initial_state; 
    int count = 0;
    initial_state.create_init_state(); // Initialize the game state
    State initial_new_state = {initial_state, 0, 0, 0};
    State new_ = {initial_state, 0, 0, 0};

    states.push(initial_new_state); // Starting with  initial_state

    while (!states.isEmpty()) 
    { // DFS loop
        State current = states.top(); // Get the current state
        states.pop();

        if (current.gameState.win) 
        { // Check if the current state is a winning state
            cout << "The solution to the game has been found" << endl;
            return;
        }

        if (current.gameState.lose) { // Skip the state if it's a losing state
            continue;
        }

        current.gameState.print_situation(); // Print the current game situation

        // Calculate the total number of objects in the current room and inventory
        int total_objects = current.gameState.rooms.get(current.room_id)->room_objects.elemcount + current.gameState.inventory.elemcount;

        for (int action = 1; action <= 5; action++) 
        { // Explore all possible actions and objects
            for (int object = 0; object < total_objects; object++) 
            {
                GameState new_state = current.gameState;
                int result;

                // Execute the action on the object and get the result
                if (object < current.gameState.rooms.get(current.room_id)->room_objects.elemcount) 
                {
                    result = new_state.advance(action, object);
                } else 
                {
                    int inventory_index = object - current.gameState.rooms.get(current.room_id)->room_objects.elemcount;
                    result = new_state.advance(action, inventory_index);
                }

                // Push the new state to the stack if the action was successful
                if (result != 0) 
                {
                    State new_ = {new_state, action, object, new_state.room_id};
                    states.push(new_);
                }
            }
        }

        // Reset the search after a certain number of iterations without a win
        if (((states.isEmpty()) || (count < 20)) && (new_.gameState.win == false)) 
        {
            states.push(initial_new_state);
            count++;
        }
    }
    cout << "Fail" << endl;
}

int main() {
    dfs();
    return 0;
} 