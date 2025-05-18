#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

/*Cree la función int is_valid(Node * n), para validar si un estado/nodo es válido 
(cumple con las restricciones del problema). Debe validar que:

No se repitan números en las filas
No se repitan números en las columnas
No se repitan números en las submatrices de 3x3
Si el estado es válido la función retorna 1, si no lo es retorna 0
*/
int is_valid(Node* n){
  int seen[10];

  //VALIDAR FILAS
  for (int i = 0 ; i < 9; i++) {
    memset(seen, 0, sizeof(seen));
    for (int j = 0 ; j < 9 ; j++) {
      int num = n->sudo[i][j];
      if (num == 0) continue;
      if (seen[num]) return 0;
      seen[num] = 1;
    }
  }
  //VALIDAR COLUMNAS
  for (int j = 0 ; j < 9; j++) {
    memset(seen, 0, sizeof(seen));
    for (int i = 0 ; i < 9 ; i++) {
      int num = n->sudo[i][j];
      if (num == 0) continue;
      if (seen[num]) return 0;
      seen[num] = 1;
    }
  }
  //VALIDAR SUBMATRICES 3X3
  for(int k = 0 ; k < 9 ; k++){
    memset(seen, 0, sizeof(seen));
    for(int p = 0; p < 9; p++){
      int i = 3*(k/3) + (p/3);
      int j = 3*(k%3) + (p%3);
      int num = n->sudo[i][j];
      if(num == 0) continue;
      if(seen[num]) return 0;
      seen[num] = 1;
    }
  }
  return 1;
}

/*
Cree una función que a partir de un nodo genere una lista con los nodos adyacentes:

List* get_adj_nodes(Node* n){
   List* list=createList();
    
   //obtenga los nodos adyacentes a n
   //y agréguelos a la lista
   
   return list;

Modifique la función get_adj_nodes para que sólo los nodos válidos sean retornados 
(use la función is_valid).
}
*/
List* get_adj_nodes(Node* n){
  List* list=createList();

  for (int i = 0 ; i < 9 ; i++) 
    for (int j = 0 ; j < 9 ; j++) 
       if (n->sudo[i][j] == 0) {
        for (int k = 1 ; k <= 9 ; k++) {
          Node* nuevoNodo = copy(n);
          nuevoNodo->sudo[i][j] = k;
          if (is_valid(nuevoNodo))
            pushBack(list, nuevoNodo);
          else 
            free(nuevoNodo);
        }
        return list;
       }
  return list;
}

/*
Implemente la función int is_final(Node * n). Esta función retorna 1 si el nodo
corresponde a un nodo final (es decir, todos los valores de la matriz son distintos
 a 0) y 0 en caso contrario.*/
int is_final(Node* n){
  for (int i = 0 ; i < 9 ; i++) 
    for (int j = 0 ; j < 9 ; j++)
      if (n->sudo[i][j] == 0) 
        return 0;
  return 1;
}

/*
Implemente la función Node* DFS(Node* n, int* cont). Esta función realiza una búsqueda
en profundidad a partir del nodo n. El algoritmo es el siguiente:
1.- Cree un stack S (pila) e inserte el nodo.
2.- Mientras el stack S no se encuentre vacío:
a) Saque y elimine el primer nodo de S.
b) Verifique si corresponde a un estado final, si es así retorne el nodo.
c) Obtenga la lista de nodos adyacentes al nodo.
d) Agregue los nodos de la lista (uno por uno) al stack S.
e) Libere la memoria usada por el nodo.
3.- Si terminó de recorre el grafo sin encontrar una solución, retorne NULL.
Almacene en la variable cont, la cantidad de iteraciones que realiza el algoritmo.
*/
Node* DFS(Node* initial, int* cont){
  Stack* S = createStack();
  push(S, initial);

  while (!is_empty(S)) {
    Node* n = top(S);
    pop(S);
    (*cont)++;
    
    if(is_final(n)) 
      return n;
    
    List* adj = get_adj_nodes(n);
    Node* adjNodos = first(adj);
    while (adjNodos) {
      push(S, adjNodos);
      adjNodos = next(adj);
    }
    free(n);
  }
  return NULL;
}

//Se pone el main para ejecutar el código
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}
