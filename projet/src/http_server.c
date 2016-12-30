/* includes yet to be added */

#include "http_server.h"
#include "logger.h"
#include "client.h"

static int init_server(int port, int maxclient) {
	struct sockaddr_in sin;
	int sock;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return errno;
	}

	memset((char *)&sin, 0, sizeof(sin));
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(port);
	sin.sin_family = AF_INET;

	if(bind(sock, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
		perror("bind");
		return errno;
	}

	listen(sock, maxclient);

	return sock;
}

http_server_init(http_server* server, int numPort, int nbMaxClient, int antiDOS) {
	
	server->log = (logger *) malloc(sizeof(logger));
	logger_init(server->log);
	server->numPort = numPort;
	server->nbMaxClient = nbMaxClient;
	server->antiDOS = antiDOS;
	server->nbClient = 0;
	pthread_mutex_init(&server->mutex_nbClient, NULL);
	pthread_cond_init(&server->cond_maxClient, NULL);

#ifdef DEBUG
	puts("HTTP Server initialized");
#endif

}

http_server_destroy(http_server* server) {
	
	logger_destroy(server->log);
	free(server->log);
	pthread_mutex_destroy(&server->mutex_nbClient);
	pthread_cond_destroy(&server->cond_maxClient);

#ifdef DEBUG
	puts("HTTP Server destroyed");
#endif
}

http_server_run_loop(http_server* server) {

	struct sockaddr_in exp;
	int socket_server, socket_client;
	socklen_t fromlen;
	client* client;

	if ((socket_server = init_server(server->numPort, server->nbMaxClient)) < 0) {
#ifdef DEBUG
		puts("error in init_server");
#endif
		exit(EXIT_FAILURE);
	}

#ifdef DEBUG
		puts("Server running ... Starting to accept clients");
#endif

	while(1) {

		if((socket_client = accept(socket_server, (struct sockaddr *)&exp, &fromlen)) < 0) {
			perror("error accept client");
			/* Error: Do what? */
    	}

#ifdef DEBUG
		puts("New client connected!");
#endif    	

    	/* TODO: if IP address exp is blacklisted -> do nothing */

    	/* TODO: accept client
			malloc new struct client
			init client
			pthread_create with client struct as argument
    	*/

    	if (pthread_mutex_lock(&server->mutex_nbClient) < 0) {
			perror("lock mutex nbClient");
			/* Error: Do what? */
		}

		server->nbClient++;

		while(server->nbClient == server->nbMaxClient) {
#ifdef DEBUG
			puts("Too much clients, server starts sleeping while no client disconnects...");
#endif
			pthread_cond_wait(&server->cond_maxClient, &server->mutex_nbClient);
			/* signaled by client threads */
		}

		if (pthread_mutex_unlock(&log->mutex) < 0) {
			perror("unlock mutex logger");
			/* Error: Do what? */
		}
	}


}