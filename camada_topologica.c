#include "camada_topologica.h"

Node* createNode(int id, char* ip, char* tcp) {
    Node* node = (Node*) malloc(sizeof(Node));
    node->id = id;
    strcpy(node->ip, ip);
    strcpy(node->tcp, tcp);
    node->sucessor = node; // O nó é seu próprio sucessor, criando um anel com apenas um nó
    return node;
}

// Compara o id com os ids na lista de nós
int isIdInList(int id, char* nodes_list) {
    char id_str[3];
    sprintf(id_str, "%02d", id); // Converte o id para uma string

    // Verifica se o id_str está na nodes_list
    if (strstr(nodes_list, id_str) != NULL) {
        return 1; // Retorna verdadeiro se o id_str estiver na nodes_list
    } else {
        return 0; // Retorna falso se o id_str não estiver na nodes_list
    }
}

// Retorna um identificador único que não esteja na lista de nós
int getUniqueIdentifier(char* nodes_list) {
    int id;
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    do {
        id = rand() % 100; // Gera um número aleatório entre 00 e 99
    } while (isIdInList(id, nodes_list)); // Continua se o id já estiver na lista

    return id;
}

void registerNode(Node* node, int ring, char* IP, char* TCP, char* user_input) {
    int fd,errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints, *res;
    struct sockaddr_in addr;
    char buffer[128];

    fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
    if (fd==-1) /*error*/ exit (1);

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET; //IPv4
    hints.ai_socktype=SOCK_DGRAM;  //UDP socket
    errcode=getaddrinfo (SERVER_IP, PORT, &hints, &res);
    if(errcode!=0) /*error*/ exit(1);

    // Envia a mensagem de registro
    char message[1024];
    sprintf(message, "%s", user_input);

    // Imprime a mensagem que será enviada
    printf("Sending message: %s\n", message);
    fflush(stdout); // Força a liberação do fluxo de saída padrão

    n=sendto(fd, message, strlen(message), 0, res->ai_addr, res->ai_addrlen);
    if (n==-1) /*error*/ exit(1);

    // Recebe a resposta
    addrlen=sizeof(addr);
    n=recvfrom(fd,buffer,128,0,(struct sockaddr*) &addr,&addrlen);
    if(n==-1) /*error*/ exit(1);

    // Verifica se o identificador não está disponível
    if (strncmp(buffer, "ERROR - node id not available", 29) == 0) {
        // Se o identificador não estiver disponível, pede a lista de nós
        char nodes_command[1024];
        sprintf(nodes_command, "NODES %03d", ring);
        sendto(fd, nodes_command, strlen(nodes_command), 0, res->ai_addr, res->ai_addrlen);

        // Recebe a lista de nós
        char nodes_list[1024];
        recvfrom(fd, nodes_list, 1024, 0, (struct sockaddr*) &addr, &addrlen);

        // Escolhe um novo identificador que não esteja na lista
        int new_id = getUniqueIdentifier(nodes_list);
        printf("O identificador fornecido já estava em uso. Um novo identificador único, %02d, foi escolhido.\n", new_id);
        node->id = new_id;

        // Tenta registrar novamente com o novo identificador
        sprintf(user_input, "REG %03d %02d %s %s", ring, new_id, IP, TCP);
        registerNode(node, ring, IP, TCP, user_input);
    } else {
        // Escreve no ecra a resposta do servidor
        write(1, "Resposta do servidor: ", 22); write(1,buffer,n); write(1, "\n", 1);
    }


    freeaddrinfo(res);
    close(fd);
}

void unregisterNode(Node* node, char* user_input) {
    int fd,errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints, *res;
    struct sockaddr_in addr;
    char buffer[128];

    fd=socket(AF_INET,SOCK_DGRAM,0); //UDP socket
    if (fd==-1) /*error*/ exit (1);

    memset(&hints,0,sizeof hints);
    hints.ai_family=AF_INET; //IPv4
    hints.ai_socktype=SOCK_DGRAM;  //UDP socket
    errcode=getaddrinfo (SERVER_IP, PORT, &hints, &res);
    if(errcode!=0) /*error*/ exit(1);

    // Envia a mensagem de desregisto
    char message[1024];
    sprintf(message, "%s", user_input);

    // Imprime a mensagem que será enviada
    printf("Sending message: %s\n", message);
    fflush(stdout); // Força a liberação do fluxo de saída padrão

    n=sendto(fd, message, strlen(message), 0, res->ai_addr, res->ai_addrlen);
    if (n==-1) /*error*/ exit(1);

    // Recebe a resposta
    addrlen=sizeof(addr);
    n=recvfrom(fd,buffer,128,0,(struct sockaddr*) &addr,&addrlen);
    if(n==-1) /*error*/ exit(1);

    // Escreve no ecra a resposta do servidor
    write(1, "Resposta do servidor: ", 22); write(1,buffer,n); write(1, "\n", 1);

    freeaddrinfo(res); //libertar a memoria alocada
    close(fd); //fechar o socket
}
