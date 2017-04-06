/*
  Arquivo: QAPP.C
  Autor: Roberto Bauer
  Observacoes:
              Dados especificos da aplicacaoh de filas
*/


#define __QAPP_C__

#include "main.h"


void *
CreateData1 (void *data)
{
  // os nohs vem de uma lista livre, 
  // portanto estah funcaoh nunca eh chamada.
  return (NULL);
}

BOOL
DeleteData1 (void *data)
{
  // os nohs vem e retornam de uma lista livre, 
  // e o tratamento para ponteiros duplicado eh 2, 
  // ou seja, deve adicionar dados duplicados,
  // portanto estah funcaoh nunca eh chamada.
  return TRUE;
}

int
NodeDataCmp1 (void *first, void *second)
{
  // compara somente a prioridade 
  return (((pND) first)->uiPriority - ((pND) second)->uiPriority);
}



//***************************************************************************//
//                  FUNCOES PARA A LISTA DE NOHS LIVRES                      //
//***************************************************************************//

// aonde data eh nivel de prioridade(int) e texto(string)
void *
CreateData2 (void *data)
{
  SNODEDATA *pNewData;

  // aloca sua estrutura de dados
  pNewData = (SNODEDATA *) malloc (sizeof (SNODEDATA));
  if (pNewData == NULL)
    return (NULL);

  // move os valores para a estrutura de dados
  // assinala-se prioridade 0
  pNewData->uiPriority = 0;
  // e aloca uma string de tamanho TEXT_SIZE + 1
  pNewData->pText = (char *) malloc (TEXT_SIZE + 1);
  *pNewData->pText = '\0';

  if (pNewData->pText == NULL)  // erro alocando a string
    {
      free (pNewData);
      return (NULL);
    }
  else
    {
      return (pNewData);        // retorna o endereco da estrutura
    }
}

BOOL
DeleteData2 (void *data)
{
  // neste caso SNODEDATA consiste em : um ponteiro e um inteiro.
  // Sendo nescessario liberar a string manualmente
  free (((pND) data)->pText);
  // e somente agora liberar a estrutura SNODEDATA1
  free (data);
  return TRUE;
}

// estah funcaoh nunca deve ser chamada
int
NodeDataCmp2 (void *first, void *second)
{
  return (0);
}


// codigo para filas
// rotina não utlilizada
int
DuplicatedNode (SLINK slkNewNode, SLINK slkListNode)
{
  return 2;
}
