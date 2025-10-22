// Auto-borrado de pedidos con tiempos por tipo unico y aviso de demoras por hora
// Compilar con SFML (Visual Studio): agregar dependencias de sfml-graphics, sfml-window, sfml-system
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <functional>
#include <fstream>
#include <windows.h>  
using namespace sf;
using namespace std;

// ================== CONFIGURACION ==================
struct ItemCfg {
    string nombre;
    int duracionSeg;
};
static map<string, int> DURACIONES = {
    {"lomito",       8},
    {"hamburguesa", 10},
    {"papas",        5},
    {"postre",       7},
    {"pizza",       15}
};
bool esHoraPico(int hour) {
    return (hour == 13) || (hour == 21);
}
int umbralDemoraPorHora(int hour) {
    if (hour == 13 || hour == 21) return 25;
    if (hour == 12 || hour == 14 || hour == 20 || hour == 22) return 20;
    return 18;
}

// ================== MODELO ==================
struct OrderItem {
    string tipo;
    int qty;
};
struct Order {
    int id = 0;
    vector<OrderItem> items;
    time_t timestamp = 0;
    int totalSeg = 0;
    bool marcadoDemora = false;
};

// ================== TIEMPO ==================
int horaLocal() {
    time_t now = time(nullptr);
    tm lt{};
#ifdef _WIN32
    localtime_s(&lt, &now);
#else
    lt = *localtime(&now);
#endif
    return lt.tm_hour;
}

int calcularTotalSeg(const vector<OrderItem>& items) {
    set<string> unicos;
    for (auto& it : items) unicos.insert(it.tipo);
    int total = 0;
    for (auto& t : unicos) {
        auto it = DURACIONES.find(t);
        if (it != DURACIONES.end()) total += it->second;
    }
    return total;
}

vector<OrderItem> generarItemsAleatorios(const string& tipoPreferente = "") {
    static vector<string> pool = { "lomito","hamburguesa","papas","postre","pizza" };
    int n = 1 + (rand() % 3);
    vector<OrderItem> out;
    auto pushOrInc = [&](const string& t) {
        auto it = find_if(out.begin(), out.end(), [&](const OrderItem& o) {return o.tipo == t; });
        if (it == out.end()) out.push_back({ t,1 });
        else it->qty++;
        };
    if (!tipoPreferente.empty()) {
        pushOrInc(tipoPreferente);
        n--;
    }
    while (n-- > 0) {
        string t = pool[rand() % pool.size()];
        pushOrInc(t);
    }
    return out;
}

// ================== BOTONES ==================
struct Boton {
    RectangleShape rect;
    Text label;
    function<void()> onClick;
    void draw(RenderWindow& win) { win.draw(rect); win.draw(label); }
    bool contains(Vector2f pt) { return rect.getGlobalBounds().contains(pt); }
};
Boton makeButton(Font& font, const string& text, Vector2f pos, Vector2f size, Color color) {
    Boton b;
    b.rect.setSize(size);
    b.rect.setFillColor(color);
    b.rect.setOutlineColor(Color::Black);
    b.rect.setOutlineThickness(2);
    b.rect.setPosition(pos);
    b.label.setFont(font);
    b.label.setString(text);
    b.label.setCharacterSize(16);
    b.label.setFillColor(Color::White);
    FloatRect tb = b.label.getLocalBounds();
    b.label.setOrigin(tb.left + tb.width / 2.f, tb.top + tb.height / 2.f);
    b.label.setPosition(pos.x + size.x / 2.f, pos.y + size.y / 2.f);
    return b;
}

// ================== ESTADO ==================
int nextOrderId = 1;
vector<Order> pedidos;
vector<string> mensajes;

// ================== LOGICA ==================
void agregarPedido(const string& tipo) {
    int h = horaLocal();
    if ((tipo == "lomito" || tipo == "hamburguesa") && h < 11) {
        cout << "No disponible " << tipo << " antes de las 11." << endl;
        return;
    }
    if (tipo == "pizza" && h < 19) {
        cout << "No disponible pizza antes de las 19." << endl;
        return;
    }
    auto items = generarItemsAleatorios(tipo);
    Order o;
    o.id = nextOrderId++;
    o.items = items;
    o.timestamp = time(nullptr);
    o.totalSeg = calcularTotalSeg(o.items);
    int umbral = umbralDemoraPorHora(h);
    o.marcadoDemora = (o.totalSeg > umbral);
    pedidos.push_back(o);
    if (esHoraPico(h)) {
        string msg = "[AVISO " + to_string(h) + ":00] Posible demora en el pedido.";
        mensajes.push_back(msg);
        cout << msg << endl;
    }
    cout << "Pedido #" << o.id << " agregado. Duracion " << o.totalSeg << "s. Items: ";
    for (auto& it : o.items) cout << it.tipo << "x" << it.qty << " ";
    cout << endl;
}

void actualizarYEliminarVencidos() {
    time_t now = time(nullptr);
    pedidos.erase(remove_if(pedidos.begin(), pedidos.end(), [&](const Order& o) {
        double elapsed = difftime(now, o.timestamp);
        if (elapsed >= o.totalSeg) {
            cout << "Pedido #" << o.id << " eliminado automaticamente ("
                << o.totalSeg << "s)." << endl;

            ofstream archivo("../pedidos_finalizados.txt", ios::app);
            if (archivo.is_open()) {
                tm lt{};
                localtime_s(&lt, &now);
                char buffer[30];
                strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &lt);

                archivo << "[Finalizado: " << buffer << "] Pedido #" << o.id << " → ";
                for (auto& it : o.items) {
                    archivo << it.tipo << "x" << it.qty << " ";
                }
                archivo << endl;
                archivo.close();

#ifdef _WIN32
                char cwd[MAX_PATH];
                GetCurrentDirectoryA(MAX_PATH, cwd);
                cout << "Archivo guardado en: " << cwd << "\\..\\pedidos_finalizados.txt" << endl;
#else
                cout << "Archivo guardado en ../pedidos_finalizados.txt" << endl;
#endif
            }
            return true;
        }
        return false;
        }), pedidos.end());
}

// ================== DIBUJO ==================
void drawPedidos(RenderWindow& w, Font& font) {
    // Panel de fondo
    RectangleShape panel({ 900.f, 450.f });
    panel.setFillColor(Color(245, 245, 245));
    panel.setOutlineColor(Color::Black);
    panel.setOutlineThickness(2);
    panel.setPosition(50.f, 100.f);
    w.draw(panel);

    float y = 120.f;
    Text titulo;
    titulo.setFont(font);
    titulo.setCharacterSize(22);
    titulo.setFillColor(Color::Black);
    titulo.setString("Pedidos en curso:");
    titulo.setPosition(60.f, 105.f);
    w.draw(titulo);

    time_t now = time(nullptr);
    for (auto& o : pedidos) {
        RectangleShape box({ 880.f, 50.f });
        box.setPosition(60.f, y + 30.f);
        box.setFillColor(o.marcadoDemora ? Color(255, 200, 200) : Color(255, 255, 255));
        box.setOutlineColor(Color::Black);
        box.setOutlineThickness(1.f);
        w.draw(box);

        string itemsStr;
        for (size_t i = 0; i < o.items.size(); ++i) {
            itemsStr += o.items[i].tipo + " x" + to_string(o.items[i].qty);
            if (i + 1 < o.items.size()) itemsStr += " | ";
        }
        int restante = max(0, o.totalSeg - (int)difftime(now, o.timestamp));

        Text txt;
        txt.setFont(font);
        txt.setCharacterSize(16);
        txt.setFillColor(Color::Black);
        txt.setString("Pedido #" + to_string(o.id) + " [" + itemsStr + "] Restante: " +
            to_string(restante) + "s");
        txt.setPosition(70.f, y + 40.f);
        w.draw(txt);

        y += 60.f;
    }

    // Panel de avisos
    RectangleShape avisoBox({ 900.f, 80.f });
    avisoBox.setFillColor(Color(255, 255, 153));
    avisoBox.setOutlineColor(Color::Red);
    avisoBox.setOutlineThickness(2);
    avisoBox.setPosition(50.f, w.getSize().y - 100.f);
    w.draw(avisoBox);

    for (size_t i = 0; i < mensajes.size() && i < 3; i++) {
        Text aviso;
        aviso.setFont(font);
        aviso.setCharacterSize(16);
        aviso.setFillColor(Color::Red);
        aviso.setString(mensajes[mensajes.size() - 1 - i]);
        aviso.setPosition(60.f, w.getSize().y - 90.f + (float)i * 20.f);
        w.draw(aviso);
    }
}

// ================== MAIN ==================
int main() {
    srand((unsigned)time(nullptr));
    RenderWindow win(VideoMode(1000, 700), "Pedidos - Auto Eliminacion");
    Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cerr << "No se pudo cargar la fuente." << endl;
        return 1;
    }

    // Colores personalizados
    Color colorLomito(205, 133, 63);
    Color colorHamb(255, 165, 0);
    Color colorPizza(220, 20, 60);
    Color colorPostre(255, 105, 180);
    Color colorPapas(255, 215, 0);

    vector<Boton> botones;
    float bx = 10, by = 10, bw = 150, bh = 40, gap = 8;
    botones.push_back(makeButton(font, "Nuevo (Lomito)", { bx, by }, { bw, bh }, colorLomito));
    botones.push_back(makeButton(font, "Nuevo (Hamburguesa)", { bx + (bw + gap), by }, { bw, bh }, colorHamb));
    botones.push_back(makeButton(font, "Nuevo (Pizza)", { bx + (bw + gap) * 2, by }, { bw, bh }, colorPizza));
    botones.push_back(makeButton(font, "Nuevo (Postre)", { bx + (bw + gap) * 3, by }, { bw, bh }, colorPostre));
    botones.push_back(makeButton(font, "Nuevo (Papas)", { bx + (bw + gap) * 4, by }, { bw, bh }, colorPapas));

    botones[0].onClick = [&] { agregarPedido("lomito"); };
    botones[1].onClick = [&] { agregarPedido("hamburguesa"); };
    botones[2].onClick = [&] { agregarPedido("pizza"); };
    botones[3].onClick = [&] { agregarPedido("postre"); };
    botones[4].onClick = [&] { agregarPedido("papas"); };

    auto drawBanner = [&](RenderWindow& w) {
        RectangleShape bar({ (float)w.getSize().x, 40.f });
        bar.setPosition(0.f, 0.f);
        bar.setFillColor(Color(173, 216, 230));
        w.draw(bar);

        Text t;
        t.setFont(font);
        t.setCharacterSize(20);
        t.setFillColor(Color::Black);
        t.setString("📦 Sistema de Pedidos - Auto Eliminacion con Registro");
        t.setPosition(10.f, 8.f);
        w.draw(t);
        };

    while (win.isOpen()) {
        Event ev;
        while (win.pollEvent(ev)) {
            if (ev.type == Event::Closed) win.close();
            if (ev.type == Event::MouseButtonPressed && ev.mouseButton.button == Mouse::Left) {
                Vector2f mp((float)ev.mouseButton.x, (float)ev.mouseButton.y);
                for (auto& b : botones) if (b.contains(mp) && b.onClick) b.onClick();
            }
        }
        actualizarYEliminarVencidos();
        win.clear(Color::White);
        drawBanner(win);
        for (auto& b : botones) b.draw(win);
        drawPedidos(win, font);
        win.display();
    }
    return 0;
}
