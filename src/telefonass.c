/*
 * telefonass.c
 *
 *  Created on: 15/02/2016
 *      Author: ernesto
 *
 *      http://www.spoj.com/problems/PHONELST/
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include <stddef.h>
#include <unistd.h>

#define MAX_CASOS_USO 40
#define MAX_TELEFONOS 10000
#define TAM_MAX_LINEA 10
#define CACA_X_VALOR_INVALIDO -1
#define TELEFONASS_LEE_CADENAS_LOG
/*
 #define CACA_LOG_HABILITADO
 */

#ifdef CACA_LOG_HABILITADO
#define caca_log_debug printf
#else
#define caca_log_debug(formato, args...) 0
#endif

/*
 #define assert_timeout(condition) if(!(condition)){printf("fuck\n");sleep(10);}
 #define assert_timeout(condition) assert(condition);
 */
#define assert_timeout(condition) 0

typedef unsigned int tipo_dato;

typedef long long bitch_vector;

typedef enum BOOLEANOS {
	falso = 0, verdadero
} bool;

typedef struct telefonoass_nudo_caracter {
	tipo_dato nodos_ocupados;
	tipo_dato letra;
	struct telefonoass_nudo_caracter *apuntadores[10];
} telefonoass_nudo_caracter;

telefonoass_nudo_caracter *pool_nudos = NULL;
int num_nodos_usados = 0;

void caca_comun_strreplace(char s[], char chr, char repl_chr) {
	int i = 0;
	while (s[i] != '\0') {
		if (s[i] == chr) {
			s[i] = repl_chr;
		}
		i++;
	}
}

static inline int lee_matrix_long_stdin(tipo_dato *matrix, int *num_filas,
		int *num_columnas, int num_max_filas, int num_max_columnas) {
	int indice_filas = 0;
	int indice_columnas = 0;
	long numero = 0;
	char *siguiente_cadena_numero = NULL;
	char *cadena_numero_actual = NULL;
	char *linea = NULL;

	linea = calloc(TAM_MAX_LINEA, sizeof(char));

	while (indice_filas < num_max_filas && fgets(linea, TAM_MAX_LINEA, stdin)) {
		indice_columnas = 0;
		cadena_numero_actual = linea;
		caca_log_debug("cadena leida numerica %s\n", linea);
		caca_comun_strreplace(linea, '\n', '\0');
		if (!strlen(linea)) {
			caca_log_debug("weird, linea vacia\n");
			continue;
		}
		for (siguiente_cadena_numero = linea;; siguiente_cadena_numero =
				cadena_numero_actual) {
			numero = strtol(siguiente_cadena_numero, &cadena_numero_actual, 10);
			if (cadena_numero_actual == siguiente_cadena_numero) {
				break;
			}
			*(matrix + indice_filas * num_max_columnas + indice_columnas) =
					numero;
			caca_log_debug("en col %d, fil %d, el valor %lu\n", indice_columnas,
					indice_filas, numero);
			indice_columnas++;
			caca_log_debug("las columnas son %d\n", indice_columnas);
		}
		if (num_columnas) {
			num_columnas[indice_filas] = indice_columnas;
		}
		indice_filas++;
		caca_log_debug("las filas son %d, con clos %d\n", indice_filas,
				indice_columnas);
	}

	*num_filas = indice_filas;
	free(linea);
	return 0;
}

static inline int caca_comun_lee_cadenas_stdin(char *matrix, int *num_filas,
		int num_cols, int num_max_filas) {
	int indice_filas = 0;
	char *linea = NULL;

	linea = matrix;

	while (indice_filas < num_max_filas && fgets(linea, num_cols + 1, stdin)) {
		caca_comun_strreplace(linea, '\n', '\0');
		if (strlen(linea) == 0 || *linea == '\n' || *linea == ' ') {
#ifdef TELEFONASS_LEE_CADENAS_LOG
			caca_log_debug("saltandose una linea\n");
#endif
			continue;
		}

#ifdef TELEFONASS_LEE_CADENAS_LOG
		caca_log_debug("la puta linea actual recibida es %s en offset %u\n",
				linea, indice_filas * num_cols);
#endif

		assert_timeout(strlen(linea) <= num_cols);
#ifdef TELEFONASS_LEE_CADENAS_LOG
		caca_log_debug("las filas son %d\n", indice_filas);
#endif

		indice_filas++;
		linea += num_cols;
		*linea = '\0';
		linea++;

	}

	*num_filas = indice_filas;
	return 0;
}

static inline char* caca_comun_imprime_lista_cadenas(char *cadenas,
		char *buffer, int num_filas, int num_cols) {
	int indice_filas = 0;
	char *apuntador_buffer = NULL;

	apuntador_buffer = buffer;

#if 0
	for (int i = 0; i < num_cols * num_filas; i++) {
		caca_log_debug("me lleva la mierda %u %c\n", i, cadenas[i]);
	}
#endif

	memset(buffer, 0, 100);
	for (indice_filas = 0; indice_filas < num_filas; indice_filas++) {
		caca_log_debug("copiando cadena %s de offset %u\n",
				cadenas + indice_filas * num_cols, indice_filas * num_cols);
		strcpy(apuntador_buffer, cadenas + indice_filas * num_cols);
		*(apuntador_buffer + strlen(cadenas + indice_filas * num_cols) - 0) =
				'\n';
		apuntador_buffer += strlen(cadenas + indice_filas * num_cols) + 1;
	}
	return buffer;
}

static inline telefonoass_nudo_caracter * telefonoass_nuevo_nudo(
		tipo_dato letra) {
	telefonoass_nudo_caracter *nuevo_nudo = NULL;
	nuevo_nudo = pool_nudos + num_nodos_usados;
	num_nodos_usados++;

	nuevo_nudo->letra = letra;

	return nuevo_nudo;
}

static inline void telefonoass_anade_nudo(telefonoass_nudo_caracter *nodo,
		tipo_dato letra, telefonoass_nudo_caracter *nodo_nuevo) {
	nodo->apuntadores[letra] = nodo_nuevo;
	nodo->nodos_ocupados++;
}

static inline bool telefonoass_core(char *telefonos, int num_telefonos,
		int tam_telefono) {
	telefonoass_nudo_caracter *raiz = NULL;

	raiz = telefonoass_nuevo_nudo(-1);

	for (int indice_tel = 0; indice_tel < num_telefonos; indice_tel++) {
		bool sequencia_nueva = falso;
		int tamano_tel_actual = 0;
		char *tel_actual = NULL;
		telefonoass_nudo_caracter *nodo_actual = NULL;

		tel_actual = telefonos + indice_tel * (tam_telefono + 1);

		caca_log_debug("procesando tel %s en offset %u\n", tel_actual,
				indice_tel * (tam_telefono + 1));

		tamano_tel_actual = strlen(tel_actual);

		nodo_actual = raiz;

		for (int indice_caracter = 0; indice_caracter < tamano_tel_actual;
				indice_caracter++) {
			char caracter_actual = 0;
			char valor_caracter_actual = 0;
			telefonoass_nudo_caracter *nodo_encontrado = NULL;
			telefonoass_nudo_caracter *nuevo_nudo = NULL;

			caracter_actual = *(tel_actual + indice_caracter);
			valor_caracter_actual = caracter_actual - '0';
			caca_log_debug("el caracter actual %c y sy valor %u en indice %u\n",
					caracter_actual, valor_caracter_actual, indice_caracter);

			if (sequencia_nueva) {
				nuevo_nudo = telefonoass_nuevo_nudo(valor_caracter_actual);
				telefonoass_anade_nudo(nodo_actual, valor_caracter_actual,
						nuevo_nudo);
				nodo_actual = nuevo_nudo;
				caca_log_debug("nodo en secuencia nueva %c\n", caracter_actual);
			} else {
				nodo_encontrado =
						nodo_actual->apuntadores[valor_caracter_actual];
				sequencia_nueva = !nodo_encontrado;
				if (nodo_encontrado) {
					if (!nodo_encontrado->nodos_ocupados
							&& indice_caracter < tamano_tel_actual - 1) {
						caca_log_debug(
								"el nodo encontrado es oja y el caracter del tel actual no es el ultimo\n");
						return falso;
					}
					nodo_actual = nodo_encontrado;
					caca_log_debug("el nodo encontrado %c\n", caracter_actual);
				} else {
					nuevo_nudo = telefonoass_nuevo_nudo(valor_caracter_actual);
					telefonoass_anade_nudo(nodo_actual, valor_caracter_actual,
							nuevo_nudo);
					nodo_actual = nuevo_nudo;
					caca_log_debug("primer nodo en secuencia nueva %c\n",
							caracter_actual);
				}
			}
		}
		if (!sequencia_nueva && nodo_actual->nodos_ocupados) {
			caca_log_debug(
					"no hubo seq nueva y el nodo donde se kedo no era el ultimo\n");
			return falso;
		}

	}
	return verdadero;
}

static inline void telefonoass_main() {
	int num_filas = 0;
	tipo_dato num_casos_uso = 0;
	tipo_dato num_telefonos = 0;
	tipo_dato contados_casos = 0;
	char *telefonos = NULL;
	int *matriz_nums = NULL;
	char buf[100] = { '\0' };

	matriz_nums = calloc(1, sizeof(int));
	assert_timeout(matriz_nums);

	lee_matrix_long_stdin(matriz_nums, &num_filas, NULL, 1, TAM_MAX_LINEA);
	num_casos_uso = *matriz_nums;
	caca_log_debug("el num de casos %u\n", num_casos_uso);

	pool_nudos = calloc(MAX_CASOS_USO * MAX_TELEFONOS * TAM_MAX_LINEA,
			sizeof(telefonoass_nudo_caracter));
	assert_timeout(pool_nudos);

	while (contados_casos < num_casos_uso) {
		bool consistente = falso;
		num_filas = 0;

		*matriz_nums = 0;
		lee_matrix_long_stdin(matriz_nums, &num_filas, NULL, 1, TAM_MAX_LINEA);
		num_telefonos = *matriz_nums;
		caca_log_debug("el numero de tels %u\n", num_telefonos);
		assert_timeout(num_telefonos && num_telefonos<=MAX_TELEFONOS);

		telefonos = calloc(MAX_TELEFONOS * (TAM_MAX_LINEA + 1), sizeof(char));
		assert_timeout(telefonos);

		caca_comun_lee_cadenas_stdin(telefonos, &num_filas, TAM_MAX_LINEA,
				num_telefonos);

		caca_log_debug("el num d files %u el de tels %u\n", num_filas,
				num_telefonos);
		assert_timeout(num_filas == num_telefonos);

#ifdef TELEFONASS_DUMPEA_TELS
		caca_log_debug("los telefonos son\n%s\n",
				caca_comun_imprime_lista_cadenas(telefonos, buf, num_telefonos,
						TAM_MAX_LINEA));
#endif

		consistente = telefonoass_core(telefonos, num_telefonos, TAM_MAX_LINEA);
		printf("%s\n", consistente ? "YES" : "NO");

		free(telefonos);
		telefonos = NULL;
		contados_casos++;
	}

	free(matriz_nums);
	free(pool_nudos);

}

int main(void) {
	telefonoass_main();
}
