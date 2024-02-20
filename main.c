#include "camada_topologica.h"
#include "interface_utilizador.h"

Node* node = NULL; 

void print_help() {
    printf("Comandos disponíveis:\n");
    printf("  join (j) ring id - Entrada de um nó com identificador id no anel ring.\n");
    printf("  leave (l) - Saída do nó do anel.\n");
    printf("  exit (x) - Fecho da aplicação.\n");
    // Adicionar mais comandos aqui
}

int main(int argc, char *argv[]) {
    if(argc != 5) {
        printf("Uso: %s IP TCP regIP regUDP\n", argv[0]);
        return 1;
    }

    char* IP = argv[1];
    char* TCP = argv[2];

    printf("Bem-vindo ao programa! Digite 'help' para ver os comandos disponíveis.\n");

    char command[256];
    int ring, id;

    while (1) {
        printf("Insira um comando: ");
        fgets(command, 256, stdin);

        if (strncmp(command, "help", 4) == 0) {
            print_help();
        } else if (strncmp(command, "j", 1) == 0) {
            sscanf(command, "j %03d %02d", &ring, &id);
            node = join(ring, id, IP, TCP); 
        } else if (strncmp(command, "l", 1) == 0) {
            sscanf(command, "l %d", &ring);
            leave(node, ring); 
        }
        else if (strncmp(command, "x", 1) == 0) {
            printf("A fechar a aplicação...\n");
            return 0; // Termina o programa
        }   
        // Adicionar mais comandos aqui
    }

    return 0;
}
