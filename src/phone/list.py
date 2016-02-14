import logging
import fileinput

#nivel_log = logging.DEBUG
nivel_log = logging.ERROR
logger_cagada = None
raiz_trie = None
valor_caracter_zero = ord('0')


class nudo():
    def __init__(self, letra):
        self.nudos_ocupados = 0
        self.letra = letra
        self.apuntadores = [None for _ in range(10)]
    
    def anadir_nudo(self, valor_letra, nodo):
        self.apuntadores[valor_letra] = nodo
        self.nudos_ocupados += 1

@profile
def phone_ass_core(lista_telefonos):
    raiz_trie = nudo(-1)
    
    logger_cagada.debug("la lista a procesar %s" % lista_telefonos)
    
    for telefono in lista_telefonos:
        tamano_telefono = 0
        nueva_secuencia = False
        nodo_actual = None
        
        nodo_actual = raiz_trie
        logger_cagada.debug("verificando tel %s" % telefono)
        
        tamano_telefono = len(telefono.strip())
        for idx, digito in enumerate(telefono.strip()):
            nodo_encontrado = None
            valor_digito = 0
            
            valor_digito = ord(digito) - valor_caracter_zero
            logger_cagada.debug("verificando digito %s de valor %d" % (digito, valor_digito))
            
            if(nueva_secuencia):
                nuevo_nodo = nudo(valor_digito)
                nodo_actual.anadir_nudo(valor_digito, nuevo_nodo)
                nodo_actual = nuevo_nodo
                logger_cagada.debug("creando nueva seq")
            else:
                nodo_encontrado = nodo_actual.apuntadores[valor_digito]
                nueva_secuencia = not nodo_encontrado
                if(nodo_encontrado):
                    if(not nodo_encontrado.nudos_ocupados):
                        if(idx < tamano_telefono - 1):
                            logger_cagada.debug("se encontro prefijo al ir añadiendo %s, indice actual %d, tam cadena siendo añadida %d" % (digito, idx, tamano_telefono - 1))
                            return False
                        else:
                            logger_cagada.debug("time time se encontro una cadena ya añadida, pero es valido")
                    else:
                        logger_cagada.debug("la cadena encontrada no c a terminado")
                    
                    nodo_actual = nodo_encontrado
                else:
                    nuevo_nodo = nudo(valor_digito)
                    nodo_actual.anadir_nudo(valor_digito, nuevo_nodo)
                    nodo_actual = nuevo_nodo
                    logger_cagada.debug("a partir del digito %s es una cadena nueva" % digito)
        
        logger_cagada.debug("se termino de procesar mamada")
        if(not nueva_secuencia and nodo_encontrado.nudos_ocupados):
            logger_cagada.debug("se anadio seq prefix, asi que invalido")
            return False
        
    return True
        
@profile
def telefonass_main():
    res = False
    idx_linea = 0
    numero_casos = 0
    num_telefonos = 0
    lineas = []
    
    logging.basicConfig(level=nivel_log)
    logger_cagada = logging.getLogger("asa")
    logger_cagada.setLevel(nivel_log)
    
    lineas = list(fileinput.input())
    
    numero_casos = int(lineas[0])
    logger_cagada.debug("num de casos %d" % numero_casos)
    
    idx_linea = 1
    for caso_actual in range(numero_casos):
        lista_telefonos = []
        
        logger_cagada.debug("indice caso %d, indice idx %d" % (caso_actual, idx_linea))
        logger_cagada.debug("numero en cadena %s" % lineas[idx_linea])
        num_telefonos = int(lineas[idx_linea])
        
        logger_cagada.debug("numero de tels %s en caso %d" % (num_telefonos, caso_actual))
        idx_linea += 1
        for linea_act in lineas[idx_linea:idx_linea + num_telefonos ]:
            lista_telefonos.append(linea_act)

        lista_telefonos.sort()
        logger_cagada.debug("la lista de tesl %s" % lista_telefonos)
        res = phone_ass_core(lista_telefonos)
        if(res):
            print("YES")
        else:
            print("NO")
        
        idx_linea += num_telefonos
        
if __name__ == '__main__':
    telefonass_main()
