// conteo de frecuencia de proveedor
// struct que tiene los detalles de proveedor

// no se utilizó IA en este código jejeje

// https://parzibyte.me/blog/posts/contar-frecuencia-de-palabras-en-c/
typedef struct prov
{
  char nombre[30];
  int frecuencia;
}NOMBRE_PROVEEDOR;

typedef struct nodito
{
  NOMBRE_PROVEEDOR nombre_prov;
  struct nodito *sig;
}NODITO;

NODITO* head_lista = NULL;

// func crear nuevo nodo
NODITO* crear_nodo(const char string_prov[])
{
  NODITO *nuevo = (NODITO*)malloc(sizeof(NODITO));
  if(!nuevo)
  {
    Serial.println("ERROR EN ASIGNACIÓN DE MEMORIA PARA LA LISTA DE PROVEEDORES");
    return NULL;
  }
  strncpy(nuevo->nombre_prov.nombre, string_prov, sizeof(nuevo->nombre_prov.nombre) - 1);
  nuevo->nombre_prov.nombre[sizeof(nuevo->nombre_prov.nombre) - 1] = '\0';
  nuevo->nombre_prov.frecuencia = 1;
  //strcpy(nuevo->nombre_prov.nombre, string_prov);
  nuevo->sig = NULL;
  return nuevo;
}

void insertar(char* string_prov)
{
  NODITO* nuevo_nodito = crear_nodo(string_prov);
  // si no hay nada en la lista
  if(head_lista == NULL)
  {
    head_lista = nuevo_nodito;
    return;
  }

  // si hay algo, insertar al final jeje
  NODITO *tmp = head_lista;
  while(tmp->sig != NULL) tmp = tmp->sig;
  tmp->sig = nuevo_nodito;
}

void agregar_palabra(char* nombre)
{
  if(head_lista == NULL)
  {
    head_lista = crear_nodo(nombre);
    return;
  }

  NODITO* tmp = head_lista;
  NODITO* ultimo = NULL;

  while(tmp != NULL)
  {
    // comprobar si la palabra ya estaba
    // si es 0, entonces si está jsjs
    if(!strcasecmp(tmp->nombre_prov.nombre, nombre))
    {
      tmp->nombre_prov.frecuencia++;
      return;
    }
    ultimo = tmp;
    tmp = tmp->sig;

    if(ultimo != NULL)
    {
      ultimo->sig = crear_nodo(nombre);
    }
  }

  // si no se encuentra nada, agregar una nueva
  NOMBRE_PROVEEDOR detalle_nombre;
  strcpy(detalle_nombre.nombre, nombre);
  detalle_nombre.frecuencia = 1;
  insertar(nombre);
}

// liberar memoria
void liberar_lista()
{
  NODITO* actual = head_lista;
  NODITO* siguiente = NULL;

  while(actual != NULL)
  {
    siguiente = actual->sig; // guardar referencia antes de liberar
    free(actual);            // libearar
    actual = siguiente;
  }

  head_lista = NULL;
}

// checa la red que predomina más y de ahí saca un ssid para engañar visualmente
const char* obtener_predominante()
{
  if(head_lista == NULL)
  {
    return "Totalplay_B1U3_2.4"; // determinado si no hay mas redes jeje
  }

  NODITO *tmp = head_lista;
  NODITO *mayor = head_lista;

  while(tmp != NULL)
  {
    if(tmp->nombre_prov.frecuencia > mayor->nombre_prov.frecuencia)
    {
      mayor = tmp;
    }
    tmp = tmp->sig;
  }
  return mayor->nombre_prov.nombre;
}