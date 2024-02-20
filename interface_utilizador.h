#ifndef INTERFACE_UTILIZADOR_H
#define INTERFACE_UTILIZADOR_H

#include "camada_topologica.h"

Node* join(int ring, int id, char* IP, char* TCP);
void direct_join(int id, int succid, char* succIP, int succTCP);
void chord();
void remove_chord();
void show_topology();
void show_routing(int dest);
void show_path(int dest);
void show_forwarding();
void message(int dest, char* message);
void leave(Node* node, int ring);
void exit();

#endif // INTERFACE_UTILIZADOR_H
