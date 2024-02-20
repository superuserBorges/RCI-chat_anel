// interface_utilizador.c
#include "interface_utilizador.h"
#include "camada_topologica.h"
#include <stdio.h>

Node* join(int ring, int id, char* IP, char* TCP) {
    // Cria um novo nó
    Node* node = createNode(id, IP, TCP);

    // Regista o nó
    char user_input[1024];
    sprintf(user_input, "REG %03d %02d %s %s", ring, id, IP, TCP);
    registerNode(node, ring, IP, TCP, user_input);

    return node;
}

void leave(Node* node, int ring) {
    // Desregistra o nó
    char user_input[1024];
    sprintf(user_input, "UNREG %03d %02d", ring, node->id);
    unregisterNode(node, user_input);
}


// Implementar as outras funções aqui
