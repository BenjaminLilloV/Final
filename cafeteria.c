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
