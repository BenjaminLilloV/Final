#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char id_producto[20];
    char nombre[50];
    int stock;
    int precio;
} Producto;

typedef struct {
    int ticket;
    char cliente[50];
    char id_producto[20];
} Pedido;

int is_equal_str(void *key1, void *key2) {
    return strcmp((char *)key1, (char *)key2) == 0;
}

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
    printf("Pedido registrado. Cliente agregado al final de la fila.\n");
}

void despachar_pedido(Map *inventario, List *fila_espera) {
    Pedido *p = (Pedido *)list_popFront(fila_espera);

    if (p == NULL) {
        printf("La fila de espera está vacía.\n");
        return;
    }

    MapPair *pair = map_search(inventario, p->id_producto);

    if (pair != NULL) {
        Producto *prod = (Producto *)pair->value;
        prod->stock--;
        printf("Atendiendo a %s - Preparando %s. ¡Inventario descontado!\n", p->cliente, prod->nombre);
    }

    free(p);
}

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
