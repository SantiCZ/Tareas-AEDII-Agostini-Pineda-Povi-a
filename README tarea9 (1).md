# Minisistema Modular de Geometria Computacional

Este proyecto es un **minisistema modular en C++** que integra algoritmos de geometria computacional en un **contexto de logistica urbana**.  
La aplicacion permite modelar depositos, clientes y barrios, utilizando **algoritmos geometricos clasicos** y visualizacion dinamica con **SFML**.

---

## 🚀 Funcionalidades

- **Voronoi (fuerza bruta):** visualizacion de regiones de influencia de cada deposito.  
- **Punto en poligono:** determina si un cliente pertenece a un barrio dibujado por el usuario.  
- **Interseccion de segmentos:** deteccion de cruces entre tramos.  
- **Range Query:** filtra clientes dentro de un poligono.  
- **Nearest Neighbor:** conecta cada cliente con el deposito mas cercano.  
- **Visualizacion en tiempo real** con **SFML**.

---

## 📂 Estructura del proyecto

```
├── Geometria.h          # Definicion de Point, Polygon y utilidades basicas
├── PointInPolygon.h     # Algoritmo punto-en-poligono (ray casting)
├── RangeQuery.h         # Consulta por rango (clientes dentro de un poligono)
├── SegmentIntersect.h   # Algoritmo de interseccion de segmentos
├── NearestNeighbor.h    # Algoritmo de vecino mas cercano
├── Voronoi.h            # Construccian simple de Voronoi
└── main.cpp             # Programa principal con la interfaz grafica
```

---

## 🎮 Controles de la aplicacion

- **Click izquierdo:** agregar un deposito.  
- **Click derecho:** agregar un cliente.  
- **Tecla P:** comenzar/terminar el dibujo de un poligono (barrio).  
- **Tecla Enter:** cerrar el poligono del barrio.  
- **Tecla R:** resetear el sistema (borra todo).  
- **Cerrar ventana:** salir del programa.  

Visualizacion en pantalla:
- Los depositos se muestran en **rojo**.  
- Los clientes en **negro** (fuera del barrio) o **verde** (dentro del barrio).  
- Cada cliente se conecta al deposito mas cercano con una linea **magenta**.  
- El barrio se dibuja en **azul translucido**.  
- Las regiones de Voronoi se colorean automaticamente.  

---

## 🛠️ Requisitos

- **Windows 10/11**  
- **Visual Studio 2019/2022** con **Desktop development with C++**  
- **SFML 2.5+ (Visual C++ 64-bit)**  

📌 Descarga SFML desde la web oficial:  
[https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php)

---

## ⚙️ Configuracion en Visual Studio

1. **Crear un proyecto nuevo** en Visual Studio → C++ Console App.  
2. Copiar los archivos del proyecto en la carpeta `Source Files` / `Header Files`.  
3. **Configurar SFML**:  
   - Ir a **Project → Properties**  
   - En **C/C++ → General → Additional Include Directories**: agregar la carpeta `SFML/include`.  
   - En **Linker → General → Additional Library Directories**: agregar la carpeta `SFML/lib`.  
   - En **Linker → Input → Additional Dependencies** agregar:  
     ```
     sfml-graphics.lib
     sfml-window.lib
     sfml-system.lib
     ```
   - Repetir lo mismo para la configuracion **Debug** y **Release**.  

4. Copiar las **DLLs de SFML** (`.dll` de la carpeta `bin`) en la carpeta donde se genera tu `.exe` (por defecto `Debug/` o `Release/`).  

---

## ▶️ Ejecucion

- Compilar y ejecutar desde Visual Studio (`Ctrl+F5`).  
- Se abrira la ventana de 800x600 donde se pueden usar los controles descritos arriba.  

---

## 📌 Contexto de uso

Este sistema se penso en un **contexto de logistica urbana**:
- Los **depositos** representan centros de distribucion.  
- Los **clientes** son puntos de demanda.  
- El **barrio** es un poligono que delimita una zona de entrega.  
- El **Voronoi** representa las areas de influencia de cada deposito.  
