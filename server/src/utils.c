#include"utils.h"
#include <commons/error.h>

t_log* logger;

int iniciar_servidor(void)
{

	int socket_servidor;
	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	int error = getaddrinfo(NULL, PUERTO, &hints, &servinfo);
	error_show(strerror(error)); 

	socket_servidor = socket(servinfo->ai_family,
                         servinfo->ai_socktype,
                         servinfo->ai_protocol);

	// Creamos el socket de escucha del servidor

	error = setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEPORT, &(int){1}, sizeof(int));
	error_show(strerror(error)); 

	// Asociamos el socket a un puerto

	error = bind(socket_servidor, servinfo->ai_addr, servinfo->ai_addrlen);
	error_show(strerror(error)); 

	

	// Escuchamos las conexiones entrantes

	error = listen(socket_servidor, SOMAXCONN);
	error_show(strerror(error)); 
	t_log *logger = log_create("tp0_server.log", "log", 1, LOG_LEVEL_INFO); 
	freeaddrinfo(servinfo);
	log_trace(logger, "Listo para escuchar a mi cliente");
	log_destroy(logger);  
	return socket_servidor;
}

int esperar_cliente(int socket_servidor)
{

	// Aceptamos un nuevo cliente
	int socket_cliente = accept(socket_servidor, NULL, NULL);
	t_log *logger = log_create("tp0_server.log", "log", 1, LOG_LEVEL_INFO);
	log_info(logger, "Se conecto un cliente!");
	log_destroy(logger);  
	return socket_cliente;
}

int recibir_operacion(int socket_cliente)
{
	int cod_op;
	if(recv(socket_cliente, &cod_op, sizeof(int), MSG_WAITALL) > 0)
		return cod_op;
	else
	{
		close(socket_cliente);
		return -1;
	}
}

void* recibir_buffer(int* size, int socket_cliente)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void recibir_mensaje(int socket_cliente)
{
	int size;
	char* buffer = recibir_buffer(&size, socket_cliente);
	log_info(logger, "Me llego el mensaje %s", buffer);
	free(buffer);
}

t_list* recibir_paquete(int socket_cliente)
{
	int size;
	int desplazamiento = 0;
	void * buffer;
	t_list* valores = list_create();
	int tamanio;

	buffer = recibir_buffer(&size, socket_cliente);
	while(desplazamiento < size)
	{
		memcpy(&tamanio, buffer + desplazamiento, sizeof(int));
		desplazamiento+=sizeof(int);
		char* valor = malloc(tamanio);
		memcpy(valor, buffer+desplazamiento, tamanio);
		desplazamiento+=tamanio;
		list_add(valores, valor);
	}
	free(buffer);
	return valores;
}
