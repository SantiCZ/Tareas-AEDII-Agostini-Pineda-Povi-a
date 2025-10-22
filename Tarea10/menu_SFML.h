#ifndef MENUSFML_H
#define MENUSFML_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//clase para usar el menu, se podia hacer con funciones? si, pero elegimos otro enfoque
class MenuSFML {
public:
    MenuSFML(int width, int height, const std::string& title);
    void run();
private:
    sf::RenderWindow window;
    sf::Font font;
    int selectedItemIndex;
    string currentRole;
    bool inSubMenu = false;
    void handleEvents();
    void update();
    void render();
    void showMainMenu();
    void showMenuEmpleado();
    void showMenuCliente();
    void transitionEffect();
    void createButton(sf::Text& text, sf::RectangleShape& button, const string& label, float posY, float width = 600, float height = 40);
    int detectMouseClick(const vector<sf::RectangleShape>& botones, sf::Event& event);
};
// ============================================================
// Constructor
// ============================================================
MenuSFML::MenuSFML(int width, int height, const std::string& title)
    : window(sf::VideoMode(width, height), title), selectedItemIndex(0) {
    if (!font.loadFromFile("Fonts/arial.ttf")) {
        cerr << "Error al cargar fuente arial.ttf" << endl;
        exit(1);
    }
    window.setFramerateLimit(60);
}
// ============================================================
// Transicion para que sea mas fluido
// ============================================================
void MenuSFML::transitionEffect() {
    sf::RectangleShape fade(sf::Vector2f(window.getSize().x, window.getSize().y));
    fade.setFillColor(sf::Color(0, 0, 0, 255));
    for (int alpha = 255; alpha >= 0; alpha -= 10) {
        fade.setFillColor(sf::Color(0, 0, 0, alpha));
        window.clear(sf::Color::Black);
        window.draw(fade);
        window.display();
        sf::sleep(sf::milliseconds(5));
    }
}
// ============================================================
// Crear boton interactivo
// ============================================================
void MenuSFML::createButton(sf::Text& text, sf::RectangleShape& button, const string& label, float posY, float width, float height) {
    button.setSize(sf::Vector2f(width, height));
    button.setFillColor(sf::Color(50, 100, 200)); // azul
    button.setOutlineColor(sf::Color::White);
    button.setOutlineThickness(2);
    button.setPosition((window.getSize().x - width) / 2.f, posY);
    text.setFont(font);
    text.setString(label);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    float textX = button.getPosition().x + (width - text.getLocalBounds().width) / 2.f;
    float textY = button.getPosition().y + (height - text.getLocalBounds().height) / 2.f - 5;
    text.setPosition(textX, textY);
}
// ============================================================
// Detectar click sobre botones
// ============================================================
int MenuSFML::detectMouseClick(const vector<sf::RectangleShape>& botones, sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
        for (size_t i = 0; i < botones.size(); ++i)
            if (botones[i].getGlobalBounds().contains(mousePos))
                return i;
    }
    return -1;
}
// ============================================================
// Bucle principal
// ============================================================
void MenuSFML::run() {
    while (window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}
// ============================================================
// Manejo de eventos
// ============================================================
void MenuSFML::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            window.close();
        if (!inSubMenu && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Num1) {
                transitionEffect();
                currentRole = "Empleado";
                inSubMenu = true;
                showMenuEmpleado();
                transitionEffect();
                inSubMenu = false;
            }
            if (event.key.code == sf::Keyboard::Num2) {
                transitionEffect();
                currentRole = "Cliente";
                inSubMenu = true;
                showMenuCliente();
                transitionEffect();
                inSubMenu = false;
            }
            if (event.key.code == sf::Keyboard::Num4)
                window.close();
        }
    }
}
// ============================================================
// Actualizacion
// ============================================================
void MenuSFML::update() {
    if (inSubMenu) return;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
        switch (selectedItemIndex) {
        case 0:
            transitionEffect();
            currentRole = "Empleado";
            inSubMenu = true;
            showMenuEmpleado();
            transitionEffect();
            inSubMenu = false;
            break;
        case 1:
            transitionEffect();
            currentRole = "Cliente";
            inSubMenu = true;
            showMenuCliente();
            transitionEffect();
            inSubMenu = false;
            break;
        case 2:
            cout << "Ejecutando interfaz avanzada..." << endl;
            break;
        case 3:
            window.close();
            break;
        }
    }
}
// ============================================================
// Renderizado menu principal
// ============================================================
void MenuSFML::render() {
    window.clear(sf::Color::Black);
    sf::Text title;
    title.setFont(font);
    title.setString("SISTEMA DE GESTION MINIMARKET");
    title.setCharacterSize(28);
    title.setFillColor(sf::Color::Cyan);
    title.setStyle(sf::Text::Bold);
    title.setPosition(160, 40);
    window.draw(title);
    vector<sf::Text> options(4);
    vector<sf::RectangleShape> botones(4);
    createButton(options[0], botones[0], "1. Menu Empleado", 150);
    createButton(options[1], botones[1], "2. Menu Cliente", 200);
    createButton(options[2], botones[2], "3. Interfaz Avanzada", 250);
    createButton(options[3], botones[3], "4. Salir", 300);
    for (int i = 0; i < options.size(); ++i) {
        window.draw(botones[i]);
        window.draw(options[i]);
    }
    window.display();
}
// ============================================================
// Submenu Empleado con botones 
// ============================================================
void MenuSFML::showMenuEmpleado() {
    vector<string> etiquetas = {
        "1. Buscar producto (Fuerza Bruta)",
        "2. Buscar producto (KMP)",
        "3. Buscar producto (Boyer-Moore)",
        "4. Cifrar texto (Cesar)",
        "5. Descifrar texto (Cesar)",
        "6. Cifrar texto (Afin)",
        "7. Descifrar texto (Afin)",
        "8. Cifrar texto (Vigenere)",
        "9. Descifrar texto (Vigenere)",
        "10. Agregar producto al archivo",
        "11. Comprimir archivo (Huffman)",
        "12. Descomprimir archivo (Huffman)",
        "13. Volver al menu principal"
    };
    vector<sf::Text> textos(etiquetas.size());
    vector<sf::RectangleShape> botones(etiquetas.size());
    while (window.isOpen()) {
        window.clear(sf::Color(0, 0, 128));
        sf::Text titulo;
        titulo.setFont(font);
        titulo.setString("MENU EMPLEADO");
        titulo.setCharacterSize(26);
        titulo.setFillColor(sf::Color::White);
        titulo.setPosition(240, 20);
        window.draw(titulo);
        float posY = 80;
        float separacion = 35; // reduccion para que entren todos los botones
        for (size_t i = 0; i < etiquetas.size(); ++i) {
            createButton(textos[i], botones[i], etiquetas[i], posY + i * separacion, 700, 32);
            window.draw(botones[i]);
            window.draw(textos[i]);
        }
        window.display();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            int opcion = detectMouseClick(botones, event);
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num0 || event.key.code == sf::Keyboard::Escape)
                    return;
            }
            if (opcion != -1) {
                cout << "Empleado selecciono: " << etiquetas[opcion] << endl;
                if (opcion == etiquetas.size() - 1) return; // volver
            }
        }
    }
}
// ============================================================
// Submenu Cliente con botones
// ============================================================
void MenuSFML::showMenuCliente() {
    vector<string> etiquetas = {
        "1. Comprimir archivo (Huffman)",
        "2. Descomprimir archivo (Huffman)",
        "3. Comprimir archivo (LZW)",
        "4. Descomprimir archivo (LZW)",
        "5. Volver al menu principal"
    };
    vector<sf::Text> textos(etiquetas.size());
    vector<sf::RectangleShape> botones(etiquetas.size());
    while (window.isOpen()) {
        window.clear(sf::Color(0, 0, 128));
        sf::Text titulo;
        titulo.setFont(font);
        titulo.setString("MENU CLIENTE");
        titulo.setCharacterSize(26);
        titulo.setFillColor(sf::Color::White);
        titulo.setPosition(240, 20);
        window.draw(titulo);
        float posY = 80;
        for (size_t i = 0; i < etiquetas.size(); ++i) {
            createButton(textos[i], botones[i], etiquetas[i], posY + i * 60, 600, 40);
            window.draw(botones[i]);
            window.draw(textos[i]);
        }
        window.display();
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            int opcion = detectMouseClick(botones, event);
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num0 || event.key.code == sf::Keyboard::Escape)
                    return;
            }
            if (opcion != -1) {
                cout << "Cliente selecciono: " << etiquetas[opcion] << endl;
                if (opcion == etiquetas.size() - 1) return; // volver
            }
        }
    }
}
#endif