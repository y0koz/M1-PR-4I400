#ifndef CLIENT_H_  
#define CLIENT_H_

#include <netdb.h>

typedef struct client {
	int socket;
	char ip[INET_ADDRSTRLEN + 1];

} client;

void client_init(client* client, int socket, char ip[INET_ADDRSTRLEN]);

void client_process_socket(client* client);

void client_destroy(client* client);

#endif 