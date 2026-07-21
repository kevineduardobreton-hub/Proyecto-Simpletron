# Programas en C++ - Programación Avanzada  (Entrega Final)

¡Bienvenido a mi repositorio de Programación Avanzada! 

Esta es la **Entrega Final** y el proyecto integrador del curso. Este espacio está dedicado a recopilar las prácticas, algoritmos y proyectos desarrollados a lo largo de la materia. El objetivo principal ha sido aplicar estructuras de datos lógicas (como el uso de pilas) y resolver problemas complejos utilizando C++, culminando en la creación de un compilador y una máquina virtual.

---

## Proyectos Incluidos

Actualmente, el repositorio cuenta con los siguientes programas funcionales, los cuales forman parte de la evaluación final:

### 1. Compilador.cpp (Proyecto Integrador)
El proyecto cumbre del curso. Es un compilador completo que lee código fuente escrito en lenguaje "Simple" y lo traduce a Lenguaje de Máquina Simpletron (SML). Implementa análisis léxico, una tabla de símbolos, y convierte operaciones aritméticas usando evaluación posfija para generar las instrucciones finales.

### 2. Programa de simpletron.cpp
Una simulación en código de la arquitectura de la computadora virtual Simpletron. Actúa como la máquina que lee, carga en memoria y ejecuta las instrucciones en lenguaje máquina (SML) generadas por el compilador.

### 3. Conversion de expresiones a postfijo.cpp
Un programa interactivo que convierte expresiones algebraicas en formato infijo (humano) a formato posfijo (notación polaca inversa). Implementa de forma estricta las reglas de jerarquía de operadores, asociatividad y eliminación de signos de agrupación mediante estructuras de datos.

### 4. Evaluacion de expresiones postfijas.cpp
El complemento ideal de la conversión. Este programa utiliza estructuras lógicas para leer una expresión matemática en formato posfijo y evaluar su resultado exacto, sirviendo como base fundamental para la lógica del compilador.

### 5. Profundidad de anidamiento.cpp
Un validador algorítmico que utiliza **Pilas (Stacks)** para verificar la correcta distribución y balanceo de símbolos de agrupamiento `()`, `[]` y `{}` en expresiones matemáticas, asegurando que se respeten los niveles de apertura y cierre.

---

##  Tecnologías Utilizadas

* **Lenguaje:** C++
* **Estructuras de Datos:** Pilas de la biblioteca estándar (`std::stack`), Vectores (`std::vector`).
* **Librerías Clave:** `<iostream>`, `<fstream>`, `<string.h>`, `<stdlib.h>`.
* **Control de Versiones:** Git y GitHub.
