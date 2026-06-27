#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Define la entidad base del inventario. Utiliza arreglos de caracteres estáticos 
// para el ID y el nombre, optimizando la memoria. Los campos de stock y precio 
// son enteros para facilitar las operaciones matemáticas posteriores en el sistema.

typedef struct {
    char id_producto[20];
    char nombre[50];
    int stock;
    int precio;
} Producto;

// Modela la orden de un cliente. Almacena el número de ticket secuencial, 
// el nombre del comprador y la clave (ID) del producto deseado. Esta estructura 
// será el contenido de cada nodo dentro de la fila de espera.

typedef struct {
    int ticket;
    char cliente[50];
    char id_producto[20];
} Pedido;

// Función de comparación requerida por el TDA Mapa. Recibe dos punteros genéricos, 
// los convierte a cadenas de texto y utiliza strcmp para verificar si los IDs 
// coinciden, permitiendo las búsquedas en la tabla hash.

int is_equal_str(void *key1, void *key2) {
    return strcmp((char *)key1, (char *)key2) == 0;
}

// Solicita los datos al usuario y reserva memoria dinámica (malloc) para un nuevo producto. 
// Antes de insertarlo, utiliza map_search para validar que el ID no exista previamente, 
// evitando duplicidad e inconsistencias en la base de datos.

void agregar_insumo(Map *inventario) {
    Producto *p = (Producto *)malloc(sizeof(Producto));

    printf("Ingrese ID del producto: ");
    scanf(" %19[^\n]", p->id_producto);

    if (map_search(inventario, p->id_producto) != NULL) {
        printf("Error: Producto ya registrado en el sistema\n");
        free(p);
        return;
    }

    printf("Ingrese nombre del producto: ");
    scanf(" %49[^\n]", p->nombre);

    printf("Ingrese cantidad: ");
    scanf("%d", &p->stock);

    printf("Ingrese precio: ");
    scanf("%d", &p->precio);

    map_insert(inventario, p->id_producto, p);
    printf("Insumo agregado con éxito\n");
}

// Busca el producto por su ID en el mapa. Si existe, solicita la variación 
// (positiva para agregar, negativa para mermas). Incluye una validación matemática 
// crucial que impide que el stock final quede en números negativos.

void actualizar_stock(Map *inventario) {
    char id[20];
    int variacion;

    printf("Ingrese ID del producto: ");
    scanf(" %19[^\n]", id);

    MapPair *pair = map_search(inventario, id);

    if (pair == NULL) {
        printf("Error: Operación inválida, stock insuficiente\n");
        return;
    }

    Producto *p = (Producto *)pair->value;

    printf("Ingrese cantidad a modificar: ");
    scanf("%d", &variacion);

    if (p->stock + variacion < 0) {
        printf("Error: Operación inválida, stock insuficiente\n");
    } else {
        p->stock += variacion;
        printf("Stock actualizado\n");
    }
}

// Realiza un recorrido secuencial del mapa utilizando map_first y map_next. 
// Formatea la salida en consola mediante tabulaciones para mostrar los datos 
// en una tabla ordenada y fácil de leer para el usuario.

void ver_inventario(Map *inventario) {
    limpiarPantalla();
    printf("%-20s | %-30s | %-10s | %-10s\n", "ID", "Nombre", "Stock", "Precio");
    printf("-------------------------------------------------------------------------------\n");

    MapPair *pair = map_first(inventario);
    while (pair != NULL) {
        Producto *p = (Producto *)pair->value;
        printf("%-20s | %-30s | %-10d | $%-9d\n", p->id_producto, p->nombre, p->stock, p->precio);
        pair = map_next(inventario);
    }
}

// Valida la existencia del producto y que su stock sea mayor a cero. Reserva memoria 
// para un nuevo pedido, le asigna un ticket y lo inserta al final de la lista 
// (list_pushBack) siguiendo la lógica FIFO. Inmediatamente descuenta el stock 
// para evitar sobreventas.

void ingresar_pedido(Map *inventario, List *fila_espera, int *ticket_counter) {
    char cliente[50];
    char id_prod[20];

    printf("Ingrese nombre del cliente: ");
    scanf(" %49[^\n]", cliente);

    printf("Ingrese ID del producto: ");
    scanf(" %19[^\n]", id_prod);

    MapPair *pair = map_search(inventario, id_prod);

    if (pair == NULL) {
        printf("Error: Producto agotado o código incorrecto.\n");
        return;
    }

    Producto *p = (Producto *)pair->value;

    if (p->stock <= 0) {
        printf("Error: Producto agotado o código incorrecto.\n");
        return;
    }

    Pedido *nuevo = (Pedido *)malloc(sizeof(Pedido));
    nuevo->ticket = (*ticket_counter)++;
    strcpy(nuevo->cliente, cliente);
    strcpy(nuevo->id_producto, id_prod);

    list_pushBack(fila_espera, nuevo);
    p->stock--;
    printf("Pedido registrado. Cliente agregado al final de la fila.\n");
}

// Extrae el primer elemento de la fila de espera utilizando list_popFront. 
// Utiliza el ID guardado en el pedido para buscar el nombre real del producto 
// en el mapa, avisa al usuario que está en preparación y libera la memoria del pedido.

void despachar_pedido(Map *inventario, List *fila_espera) {
    Pedido *p = (Pedido *)list_popFront(fila_espera);

    if (p == NULL) {
        printf("La fila de espera está vacía.\n");
        return;
    }

    MapPair *pair = map_search(inventario, p->id_producto);

    if (pair != NULL) {
        Producto *prod = (Producto *)pair->value;
        printf("Atendiendo a %s - Preparando %s. ¡Inventario descontado!\n", p->cliente, prod->nombre);
    }

    free(p);
}

// Recorre la lista enlazada desde el primer elemento (list_first) hasta el último, 
// mostrando el orden exacto de los clientes y sus respectivos tickets sin 
// alterar la estructura original de la cola.

void ver_fila(List *fila_espera) {
    limpiarPantalla();
    printf("%-10s | %-30s | %-20s\n", "Ticket", "Cliente", "ID Producto");
    printf("-----------------------------------------------------------------\n");

    Pedido *p = (Pedido *)list_first(fila_espera);
    while (p != NULL) {
        printf("%-10d | %-30s | %-20s\n", p->ticket, p->cliente, p->id_producto);
        p = (Pedido *)list_next(fila_espera);
    }
}

int main() {
    // Inicializa el TDA Mapa para gestionar el inventario de forma eficiente 
    // y el TDA Lista para controlar la fila de atención. Define un contador 
    // de tickets que incrementará con cada pedido nuevo.
    Map *inventario = map_create(is_equal_str);
    List *fila_espera = list_create();
    int ticket_counter = 1;
    char opcion_principal;
    char opcion_sub;

    // Implementa un bucle do-while que mantiene el programa en ejecución continua. 
    // Utiliza una estructura switch para derivar el flujo del programa hacia 
    // los submenús correspondientes de Inventario o de Caja.
    do {
        limpiarPantalla();
        puts("==================================================");
        puts("  SISTEMA DE GESTIÓN - CAFETERÍA DE ESPECIALIDAD  ");
        puts("==================================================");
        puts("1. Gestión de Inventario");
        puts("2. Caja y Pedidos");
        puts("3. Salir del Sistema");
        printf("Seleccione una opción: ");
        scanf(" %c", &opcion_principal);

        switch (opcion_principal) {
        case '1':
            do {
                limpiarPantalla();
                puts("--- Gestión de Inventario ---");
                puts("1. Agregar nuevo insumo");
                puts("2. Actualizar stock");
                puts("3. Ver inventario completo");
                puts("4. Volver al menú principal");
                printf("Seleccione una opción: ");
                scanf(" %c", &opcion_sub);

                if (opcion_sub == '1') {
                    agregar_insumo(inventario);
                } else if (opcion_sub == '2') {
                    actualizar_stock(inventario);
                } else if (opcion_sub == '3') {
                    ver_inventario(inventario);
                }

                if (opcion_sub != '4') {
                    presioneTeclaParaContinuar();
                }
            } while (opcion_sub != '4');
            break;

        case '2':
            do {
                limpiarPantalla();
                puts("--- Caja y Pedidos ---");
                puts("1. Ingresar nuevo pedido");
                puts("2. Despachar próximo pedido");
                puts("3. Ver fila de espera");
                puts("4. Volver al menú principal");
                printf("Seleccione una opción: ");
                scanf(" %c", &opcion_sub);

                if (opcion_sub == '1') {
                    ingresar_pedido(inventario, fila_espera, &ticket_counter);
                } else if (opcion_sub == '2') {
                    despachar_pedido(inventario, fila_espera);
                } else if (opcion_sub == '3') {
                    ver_fila(fila_espera);
                }

                if (opcion_sub != '4') {
                    presioneTeclaParaContinuar();
                }
            } while (opcion_sub != '4');
            break;

        case '3':
            puts("Saliendo del sistema...");
            break;

        default:
            puts("Opción no válida.");
            presioneTeclaParaContinuar();
        }
    } while (opcion_principal != '3');

    // Bucle fundamental para evitar fugas de memoria (memory leaks). Recorre 
    // cada par del mapa y utiliza la función free() para liberar la memoria 
    // dinámica de cada estructura Producto antes de destruir el mapa con map_clean.
    MapPair *pair = map_first(inventario);
    while (pair != NULL) {
        free(pair->value);
        pair = map_next(inventario);
    }
    map_clean(inventario);

    // Si quedan pedidos sin despachar al momento de cerrar el sistema, este ciclo 
    // recorre la lista enlazada y libera la memoria de cada estructura Pedido 
    // antes de vaciar la lista por completo con list_clean.
    Pedido *ped = (Pedido *)list_first(fila_espera);
    while (ped != NULL) {
        free(ped);
        ped = (Pedido *)list_next(fila_espera);
    }
    list_clean(fila_espera);

    return 0;
}
