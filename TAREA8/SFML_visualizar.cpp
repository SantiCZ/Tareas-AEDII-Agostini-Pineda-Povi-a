#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>
#include <functional>
#include <iostream>
#include <algorithm>
using namespace sf;
using namespace std;
// Simple Heap con soporte MIN/MAX y metodos requeridos
struct Heap {
    vector<int> a;
    enum HeapType { MAX, MIN } type;
    Heap(HeapType t = MAX) : type(t) {}
    bool cmpVal(int x, int y) const { return (type == MAX) ? x > y : x < y; }
    void siftUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (cmpVal(a[i], a[p])) {
                swap(a[i], a[p]);
                i = p;
            }
            else break;
        }
    }
    // siftDown que limita al tamanio 'n'
    void siftDown(int i, int n) {
        while (true) {
            int l = 2 * i + 1;
            int r = 2 * i + 2;
            int best = i;
            if (l < n && cmpVal(a[l], a[best])) best = l;
            if (r < n && cmpVal(a[r], a[best])) best = r;
            if (best != i) {
                swap(a[i], a[best]);
                i = best;
            }
            else break;
        }
    }
    // insertar 
    void insert(int val) {
        a.push_back(val);
        siftUp(a.size() - 1);
    }
    // raiz 
    int raiz() const {
        if (a.empty()) throw runtime_error("heap vacio");
        return a[0];
    }
    // extraer raiz
    int extraerRaiz() {
        if (a.empty()) throw runtime_error("heap vacio");
        int ret = a[0];
        a[0] = a.back();
        a.pop_back();
        if (!a.empty()) siftDown(0, (int)a.size());
        return ret;
    }
    // heapify / build heap
    void heapify() {
        int n = (int)a.size();
        for (int i = n / 2 - 1; i >= 0; --i) siftDown(i, n);
    }
    int size() const { return (int)a.size(); }
    void setType(HeapType t) { type = t; heapify(); }
};
// Accion para visualizacion de heapsort paso a paso
struct Action {
    enum Type { COMPARE, SWAP, MARK_SORTED } type;
    int i, j;       // indices implicados
    int heapSize;   // tamaño del heap durante la accion (para marcar sorted)
    Action(Type t, int i_, int j_, int hs) : type(t), i(i_), j(j_), heapSize(hs) {}
};
// registrar siftDown con acciones (para visualizacion)
void siftDownRecord(vector<int>& b, int i, int n, function<bool(int, int)> cmp, vector<Action>& actions) {
    while (true) {
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        int best = i;
        if (l < n) {
            actions.emplace_back(Action::COMPARE, i, l, n);
            if (cmp(b[l], b[best])) best = l;
        }
        if (r < n) {
            actions.emplace_back(Action::COMPARE, i, r, n);
            if (cmp(b[r], b[best])) best = r;
        }
        if (best != i) {
            actions.emplace_back(Action::SWAP, i, best, n);
            swap(b[i], b[best]);
            i = best;
        }
        else break;
    }
}
// generar lista de acciones para heapsort a partir del array y tipo de heap
vector<Action> recordHeapsortActions(const vector<int>& arr, Heap::HeapType type) {
    int n = (int)arr.size();
    vector<int> b = arr;
    vector<Action> actions;
    function<bool(int, int)> cmp = (type == Heap::MAX) ?
        function<bool(int, int)>([](int x, int y) { return x > y; }) :
        function<bool(int, int)>([](int x, int y) { return x < y; });
    // build heap
    for (int i = n / 2 - 1; i >= 0; --i) siftDownRecord(b, i, n, cmp, actions);
    // heapsort
    for (int end = n - 1; end > 0; --end) {
        actions.emplace_back(Action::SWAP, 0, end, end);
        swap(b[0], b[end]);
        actions.emplace_back(Action::MARK_SORTED, -1, -1, end);
        siftDownRecord(b, 0, end, cmp, actions);
    }
    // todo: marcar el cero final como sorted
    actions.emplace_back(Action::MARK_SORTED, -1, -1, 0);
    return actions;
}
// ================== BOTON (simple para que no sea dificil de usar) ==================
struct Button {
    RectangleShape rect;
    Text label;
    function<void()> onClick;
    void draw(RenderWindow& win) {
        win.draw(rect);
        win.draw(label);
    }
    bool contains(Vector2f pt) { return rect.getGlobalBounds().contains(pt); }
};
// ================== DIBUJAR HEAP ==================
void drawHeap(RenderWindow& win, Heap& heap, Font& font,
    pair<int, int> compareIdx = { -1,-1 }, pair<int, int> swapIdx = { -1,-1 }, int sortedFrom = -1) {
    int n = heap.size();
    if (n == 0) return;
    float startY = 100;
    float dy = 70;
    vector<Vector2f> pos(n);
    for (int i = 0; i < n; i++) {
        int level = (int)floor(log2(i + 1));
        int first = (int)pow(2, level) - 1;
        int offset = i - first;
        int nodes = (int)pow(2, level);
        float sep = win.getSize().x / (nodes + 1);
        pos[i] = Vector2f(sep * (offset + 1), startY + dy * level);
    }
    // aristas
    for (int i = 0; i < n; i++) {
        int l = 2 * i + 1, r = 2 * i + 2;
        if (l < n) {
            Vector2f dir = pos[l] - pos[i];
            float length = sqrt(dir.x * dir.x + dir.y * dir.y);
            float angle = atan2(dir.y, dir.x) * 180.f / 3.14159265f;
            RectangleShape edge(Vector2f(length, 3));
            edge.setFillColor(Color(50, 50, 50));
            edge.setOrigin(0, 1.5f);
            edge.setPosition(pos[i]);
            edge.setRotation(angle);
            win.draw(edge);
        }
        if (r < n) {
            Vector2f dir = pos[r] - pos[i];
            float length = sqrt(dir.x * dir.x + dir.y * dir.y);
            float angle = atan2(dir.y, dir.x) * 180.f / 3.14159265f;
            RectangleShape edge(Vector2f(length, 3));
            edge.setFillColor(Color(50, 50, 50));
            edge.setOrigin(0, 1.5f);
            edge.setPosition(pos[i]);
            edge.setRotation(angle);
            win.draw(edge);
        }
    }
    // nodos
    for (int i = 0; i < n; i++) {
        CircleShape c(20);
        // base color
        Color fill(100, 149, 237);
        // comparar
        if (i == compareIdx.first || i == compareIdx.second) fill = Color(255, 215, 0); // amarillo
        //swap
        if (i == swapIdx.first || i == swapIdx.second) fill = Color(220, 20, 60); // rojo
        // sort
        if (sortedFrom >= 0 && i >= sortedFrom) fill = Color(144, 238, 144); // verde claro
        c.setFillColor(fill);
        c.setOutlineThickness(2);
        c.setOutlineColor(Color::Black);
        c.setOrigin(20, 20);
        c.setPosition(pos[i]);
        win.draw(c);
        Text t;
        t.setFont(font);
        t.setString(to_string(heap.a[i]));
        t.setCharacterSize(16);
        t.setFillColor(Color::White);
        FloatRect tb = t.getLocalBounds();
        t.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
        t.setPosition(pos[i].x, pos[i].y - 8);
        win.draw(t);
    }
    // vista del array
    for (int i = 0; i < n; i++) {
        RectangleShape box(Vector2f(30, 30));
        box.setFillColor(Color(220, 220, 220));
        if (sortedFrom >= 0 && i >= sortedFrom) box.setFillColor(Color(180, 230, 180));
        box.setOutlineColor(Color::Black);
        box.setOutlineThickness(1);
        box.setPosition(50 + i * 35, win.getSize().y - 60);
        win.draw(box);
        Text t;
        t.setFont(font);
        t.setString(to_string(heap.a[i]));
        t.setCharacterSize(14);
        t.setFillColor(Color::Black);
        FloatRect tb = t.getLocalBounds();
        t.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
        t.setPosition(50 + i * 35 + 15, win.getSize().y - 60 + 15 - 2);
        win.draw(t);
    }
}
// crear botones
Button makeButton(Font& font, const string& text, Vector2f pos, Vector2f size) {
    Button b;
    b.rect.setSize(size);
    b.rect.setFillColor(Color(200, 200, 200));
    b.rect.setOutlineColor(Color::Black);
    b.rect.setOutlineThickness(1);
    b.rect.setPosition(pos);
    b.label.setFont(font);
    b.label.setString(text);
    b.label.setCharacterSize(14);
    b.label.setFillColor(Color::Black);
    FloatRect tb = b.label.getLocalBounds();
    b.label.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    b.label.setPosition(pos.x + size.x / 2.f, pos.y + size.y / 2.f);
    return b;
}
// ================== MAIN ==================
int main() {
    RenderWindow win(VideoMode(1000, 700), "Heap Visualizer");
    Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cerr << "No se pudo cargar la fuente. Ajusta la ruta."<<endl;
        return 1;
    }
    Heap heap(Heap::MAX);
    // Dataset de la clase
    vector<int> dataset = { 10,20,30,15,50,12,40,7,18,45,1 };
    int insertIndex = 0;
    // Mensajes
    Text message;
    message.setFont(font);
    message.setCharacterSize(18);
    message.setFillColor(Color::Red);
    message.setPosition(10, 40);
    string messageTxt = "";
    bool showMessage = false;
    sf::Clock msgClock;
    // Botones
    vector<Button> buttons;
    float bx = 10, by = 10, bw = 140, bh = 30, gap = 8;
    buttons.push_back(makeButton(font, "Insert (A)", Vector2f(bx, by), Vector2f(bw, bh)));
    buttons.push_back(makeButton(font, "Extraer raiz", Vector2f(bx + (bw + gap) * 1, by), Vector2f(bw, bh)));
    buttons.push_back(makeButton(font, "Heapify", Vector2f(bx + (bw + gap) * 2, by), Vector2f(bw, bh)));
    buttons.push_back(makeButton(font, "Toggle Min/Max", Vector2f(bx + (bw + gap) * 3, by), Vector2f(bw, bh)));
    buttons.push_back(makeButton(font, "Start Heapsort", Vector2f(bx + (bw + gap) * 4, by), Vector2f(bw, bh)));
    buttons.push_back(makeButton(font, "Step Heapsort", Vector2f(bx + (bw + gap) * 5, by), Vector2f(bw, bh)));
    buttons.push_back(makeButton(font, "Auto Play", Vector2f(bx + (bw + gap) * 6, by), Vector2f(bw, bh)));
    // assignar acciones a botones
    buttons[0].onClick = [&] { // Insertar
        if (insertIndex < (int)dataset.size()) {
            int val = dataset[insertIndex++];
            heap.insert(val);
            messageTxt = string("Inserted ") + to_string(val);
            showMessage = true;
            msgClock.restart();
        }
        else {
            messageTxt = "Dataset completo";
            showMessage = true; msgClock.restart();
        }
        };
    buttons[1].onClick = [&] { // Extraer raiz
        if (heap.size() > 0) {
            int val = heap.extraerRaiz();
            messageTxt = string("Extracted ") + to_string(val);
            showMessage = true; msgClock.restart();
        }
        else {
            messageTxt = "Heap vacio"; showMessage = true; msgClock.restart();
        }
        };
    buttons[2].onClick = [&] { // Heapify
        heap.heapify();
        messageTxt = "Heapified"; showMessage = true; msgClock.restart();
        };
    buttons[3].onClick = [&] { // Min/Max
        heap.setType((heap.type == Heap::MAX) ? Heap::MIN : Heap::MAX);
        messageTxt = (heap.type == Heap::MAX) ? "Set MAX heap" : "Set MIN heap";
        showMessage = true; msgClock.restart();
        };
    // Heapsort acciones / estado
    vector<Action> actions;
    size_t actionIndex = 0;
    pair<int, int> compareIdx = { -1,-1 };
    pair<int, int> swapIdx = { -1,-1 };
    int sortedFrom = -1;
    bool autoPlay = false;
    Clock autoClock;
    float autoInterval = 0.4f; // segundos entre pasos
    auto resetHighlights = [&] { compareIdx = { -1,-1 }; swapIdx = { -1,-1 }; };
    auto processAction = [&](const Action& act) {
        resetHighlights();
        if (act.type == Action::COMPARE) {
            compareIdx = { act.i, act.j };
        }
        else if (act.type == Action::SWAP) {
            swapIdx = { act.i, act.j };
            if (act.i >= 0 && act.j >= 0 && act.i < heap.size() && act.j < heap.size()) swap(heap.a[act.i], heap.a[act.j]);
        }
        else if (act.type == Action::MARK_SORTED) {
            sortedFrom = act.heapSize;
        }
        };
    buttons[4].onClick = [&] { // Start Heapsort
        actions = recordHeapsortActions(heap.a, heap.type);
        actionIndex = 0;
        sortedFrom = heap.size();
        resetHighlights();
        autoPlay = false;
        messageTxt = "Heapsort started"; showMessage = true; msgClock.restart();
        };
    buttons[5].onClick = [&] { // heapsort paso a paso
        if (actionIndex < actions.size()) {
            processAction(actions[actionIndex++]);
        }
        };
    buttons[6].onClick = [&] { // Auto Play 
        autoPlay = !autoPlay;
        autoClock.restart();
        messageTxt = autoPlay ? "Auto play ON" : "Auto play OFF";
        showMessage = true; msgClock.restart();
        };
    // tecla A para insertar nodos hasta que se acabe el dataset
    while (win.isOpen()) {
        Event event;
        while (win.pollEvent(event)) {
            if (event.type == Event::Closed) win.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::A) {
                    if (insertIndex < (int)dataset.size()) {
                        int val = dataset[insertIndex++];
                        heap.insert(val);
                        messageTxt = string("Inserted ") + to_string(val);
                        showMessage = true; msgClock.restart();
                    }
                    else {
                        messageTxt = "Dataset completo"; showMessage = true; msgClock.restart();
                    }
                }
            }
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                Vector2f mp((float)event.mouseButton.x, (float)event.mouseButton.y);
                for (auto& b : buttons) if (b.contains(mp) && b.onClick) b.onClick();
            }
        }
        // auto play de heapsort
        if (autoPlay && actionIndex < actions.size()) {
            if (autoClock.getElapsedTime().asSeconds() > autoInterval) {
                processAction(actions[actionIndex++]);
                autoClock.restart();
            }
        }
        win.clear(Color::White);
        // botones
        for (auto& b : buttons) b.draw(win);
        // dibujar el heap
        drawHeap(win, heap, font, compareIdx, swapIdx, sortedFrom);
        // mensaje
        if (showMessage) {
            if (msgClock.getElapsedTime().asSeconds() > 2.0f) showMessage = false;
            else {
                message.setString(messageTxt);
                win.draw(message);
            }
        }
        win.display();
    }
    return 0;
}