#include "client.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	
	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	config = config_create("cliente.config");
	valor = config_get_string_value(config, "CLAVE"); 
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config
	logger = log_create("tp0.log", "log", 1, LOG_LEVEL_INFO); 
	log_info(logger, valor); 
	


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);
	
	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	int bytes = send(conexion, &valor, sizeof(valor), 0);
	if(bytes == -1) {
		log_info(logger, "fallo el envio");
	} else {
		log_info(logger, "se ha enviado el mensaje"); 
	}

	// Armamos y enviamos el paquete
	paquete(conexion);
	

	
	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	log_info(logger, leido); 
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	if (strcmp(leido, "\0")) {
		log_info(logger, leido); 
	}
	free(leido); 


	// ¡No te olvides de liberar las lineas antes de regresar!

}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	if (strcmp(leido, "\0")) {
		int bytes = send(conexion, &leido, sizeof(leido), 0);
		if(bytes == -1) {
			printf("fallo el envio del paquete"); 
		}
	}
	
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido); 
	free(paquete); 
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	config_destroy(config); 
	log_destroy(logger); 
	close(conexion); 
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
