# Sistema de Gestión Operativa para Cafetería de Especialidad ☕

## Descripción:
Este proyecto es una aplicación de consola en C diseñada para optimizar y sincronizar el punto de venta (caja) con el control de insumos (bodega) en una cafetería. El sistema evita la venta de productos agotados y garantiza un orden de atención justo. Para lograr un rendimiento óptimo de complejidad temporal promedio de *O(1)*, el programa implementa un modelo basado en Tipos de Datos Abstractos (TDAs): utiliza un Mapa (Tabla Hash) para búsquedas ultrarrápidas en el inventario y una Lista Enlazada Simple estructurada como Cola para modelar la política FIFO (First In, First Out) de la fila de atención.

## Instalación, compilación y ejecución:
### Por Replit:

1. Ingresa a [replit.com](https://replit.com) e inicia sesión, o crea una cuenta gratuita.
2. En tu página principal, haz clic en **"+ Create Repl"**.
3. Selecciona la opción **"Upload folder"** o **"Import ZIP"** y sube el archivo `.zip` del proyecto.
4. Replit detectará automáticamente que es un proyecto en C  y preparará el entorno de ejecución.

### Por GitHub:

Recomendamos usar Repl.it para una rápida configuración y ejecución:

1. Ingresa a [replit.com](https://replit.com) e inicia sesión, o crea una cuenta gratuita.
2. En tu página principal, selecciona **"New Repl"** y elige **"Import from GitHub"**.
3. Pega la URL del repositorio.
4. Replit clonará el repositorio y preparará el entorno de ejecución.
  
### Para ejecutar el programa
Abra la terminal (Shell) en Replit y ejecute:
````
gcc tdas/*.c cafeteria.c -Wno-unused-result -o cafeteria
````

Y luego ejecutar:
````
./cafeteria
````

## Funcionalidades

### Funcionando correctamente:
El programa gestiona operativamente una cafetería basándose en las interacciones del usuario mediante consola y permite utilizar los siguientes módulos:

`1) Gestión de Inventario: ` Utiliza el TDA Mapa para gestionar los productos. Permite registrar nuevos insumos, actualizar el stock de ítems existentes mediante su identificador único y desplegar una tabla formateada con el inventario completo.

`2) Caja y Pedidos: `  Utiliza el TDA Cola para gestionar a los clientes. Permite registrar un nuevo pedido (validando en tiempo real contra el Hash Map que exista stock disponible), despachar el próximo pedido en la fila (descontando el insumo automáticamente de la memoria) y visualizar el orden actual de espera.

`3) Salir del Sistema: ` Finaliza la ejecución del programa y ejecuta una rutina completa de liberación de memoria dinámica para asegurar que no queden procesos huérfanos.


### Problemas conocidos:
El programa resuelve correctamente la sincronización y despacha los pedidos operando in-place. Sin embargo, al ser una aplicación estructurada íntegramente en memoria volátil (RAM), carece de persistencia de datos. Al seleccionar la opción "Salir del Sistema", todo el inventario registrado y los pedidos en fila se destruyen tras liberar la memoria, por lo que el sistema arranca desde cero en cada nueva ejecución.

### A mejorar:

1. Implementar persistencia de datos utilizando operaciones de entrada/salida (I/O) en C para leer y escribir el inventario en un archivo .csv al iniciar y cerrar el programa.
2. Añadir un módulo de reporte de ventas que acumule el total de ingresos generados por los pedidos despachados antes de cerrar la caja.


## Ejemplo de uso e interacción:
Al compilar y ejecutar, el sistema mostrará el menú principal:

```
==================================================
  SISTEMA DE GESTIÓN - CAFETERÍA DE ESPECIALIDAD
==================================================
1. Gestión de Inventario
2. Caja y Pedidos
3. Salir del Sistema
Seleccione una opción: 1
```


`1) Gestión de Inventario > 1. Agregar nuevo insumo:`

```
Plaintext
--- Gestión de Inventario ---
1. Agregar nuevo insumo
2. Actualizar stock
3. Ver inventario completo
4. Volver al menú principal
Seleccione una opción: 1

Ingrese ID del producto: CAF01
Ingrese nombre del producto: Espresso Doble
Ingrese cantidad: 50
Ingrese precio: 2500
Insumo agregado con éxito
```
`2) Caja y Pedidos > 1. Ingresar nuevo pedido:`

```
--- Caja y Pedidos ---
1. Ingresar nuevo pedido
2. Despachar próximo pedido
3. Ver fila de espera
4. Volver al menú principal
Seleccione una opción: 1

Ingrese nombre del cliente: Juan Perez
Ingrese ID del producto: CAF01
Pedido registrado. Cliente agregado al final de la fila.
```
`2) Caja y Pedidos > 2. Despachar próximo pedido:`

```
--- Caja y Pedidos ---
...
Seleccione una opción: 2

Atendiendo a Juan Perez - Preparando Espresso Doble. ¡Inventario descontado!
```

`4) Salir:`

```
Ingrese su opción: 4

(El programa finaliza su ejecución)
```


## Contribuciones

El desarrollo, documentación y defensa de este proyecto fue realizado de manera equitativa por el equipo . Las responsabilidades se distribuyeron de la siguiente forma:

**Benjamín Lillo (Coordinador y Responsable de Integración)**
- Código: Desarrolló el módulo del TDA Cola en C y lideró la integración final del main.c.
- Informe: Redactó la planificación, la justificación de TDAs y conclusiones.
- Presentación: Responsable de la introducción oral y explicación del problema.
  
**Lucas Moncada (Comunicador)**
- Código: Diseñó e implementó desde cero el TDA Mapa (Tabla Hash), incluyendo la función de dispersión y la lógica de resolución de colisiones.  
- Informe: Estructuró el documento principal y mantuvo el README en GitHub.
- Presentación: Construyó las diapositivas y explicó la arquitectura en la defensa.

**Matías Bórquez (Gestor de Calidad)**
- Código: Estructuró las entidades (structs) y ejecutó el testing de validación para rastrear y neutralizar las fugas de memoria (memory leaks).
- Informe: Documentó las funcionalidades paso a paso y el análisis de complejidad.
- Presentación: Preparó el material de apoyo visual y expuso los algoritmos bajo el capó.
