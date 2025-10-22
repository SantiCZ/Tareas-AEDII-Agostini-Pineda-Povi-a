#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <functional>
#include <iostream>
#include <algorithm>
#include <optional>   // <-- necesario para std::optional

using namespace sf;
using namespace std;

// Definimos un pedido (Order)
struct Order {
    int id;
    string description;
};

// Simple Heap con soporte MIN/MAX y metodos requeridos
struct Heap {
    vector<int> a;
    enum HeapType { MAX, MIN } type;

    Heap(HeapType t = MAX) : type(t) {}

    void push(int val) {
        a.push_back(val);
        push_heap(a.begin(), a.end(), comp());
    }

    void pop() {
        pop_heap(a.begin(), a.end(), comp());
        a.pop_back();
    }

    int top() {
        return a.front();
    }

    bool empty() {
        return a.empty();
    }

private:
    function<bool(int, int)> comp() {
        if (type == MAX) return less<int>();
        else return greater<int>();
    }
};

// Vector global de ordenes en procesamiento
std::vector<std::optional<Order>> processing;

// Funcion de prueba que usa "processing"
void addOrder(int id, const string& desc) {
    Order o{ id, desc };
    processing.push_back(o);
}

int main() {
    // Ejemplo de uso
    addOrder(1, "Pizza grande");
    addOrder(2, "Hamburguesa doble");

    for (auto& opt : processing) {
        if (opt.has_value()) {
            cout << "ID: " << opt->id
                << " - Desc: " << opt->description << endl;
        }
    }

    return 0;
}
