#include "client.h"

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

logger = log_create("tp0.log", "tp0", true, LOG_LEVEL_INFO);

if(logger==NULL){
	printf("Error al crear el logger.\n");
	return 1;
}
log_info(logger, "Hola! Soy un log");

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"


	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	config = config_create("cliente.config");

	if (config == NULL) {
        printf("Error al cargar el archivo de configuración.\n");
        return 1;
    }
	valor= config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	log_info(logger,valor);

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje

	enviar_mensaje(valor,conexion);

	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
	return 0;
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

    while(strcmp(leido,"")!=0){

    log_info(logger,leido);
	free(leido);
	leido=readline("> ");

}
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío


	// ¡No te olvides de liberar las lineas antes de regresar!
free(leido);
}

void paquete(int conexion)
{
    // Crear un paquete
    t_paquete* paquete = crear_paquete();
    if (paquete == NULL) {
        fprintf(stderr, "Error al crear el paquete\n");
        return;
    }

    // Inicializar el buffer para el mensaje
    char* mensaje = NULL;
    size_t mensaje_size = 0;
    size_t buffer_size = 1024; // Tamaño inicial del buffer
    mensaje = malloc(buffer_size);
    if (mensaje == NULL) {
        perror("Error al asignar memoria para el mensaje");
        eliminar_paquete(paquete);
        return;
    }
    mensaje[0] = '\0'; // Inicializar el buffer con una cadena vacía

    // Leer y concatenar líneas hasta encontrar una línea vacía
    while (1) {
        char* leido = readline("> ");
        if (leido == NULL || strcmp(leido, "") == 0) {
            free(leido);
            break;
        }

        // Verificar si el buffer necesita ser ampliado
        size_t leido_size = strlen(leido);
        if (mensaje_size + leido_size >= buffer_size) {
            buffer_size = mensaje_size + leido_size + 1;
            char* temp = realloc(mensaje, buffer_size);
            if (temp == NULL) {
                perror("Error al reasignar memoria para el mensaje");
                free(leido);
                free(mensaje);
                eliminar_paquete(paquete);
                return;
            }
            mensaje = temp;
        }

        // Concatenar la línea leída al buffer
        strcat(mensaje, leido);
        mensaje_size += leido_size;

        // Liberar la memoria de la línea leída
        free(leido);
    }

    // Agregar el buffer concatenado al paquete
    agregar_a_paquete(paquete, mensaje, mensaje_size);

    // Enviar el paquete
   enviar_paquete(paquete,conexion);

    // Liberar recursos
    free(mensaje);
    eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	liberar_conexion(conexion);
	config_destroy(config);
	log_destroy(logger);
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
}
