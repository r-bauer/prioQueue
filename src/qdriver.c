/*
  Arquivo: QDRIVER.C
  Autor: Roberto Bauer
*/


#define __QDRIVER_C__

#include "main.h"

LOCAL BOOL RemoveQueue (SLIST *, SLIST *, void *);
LOCAL BOOL InsertQueue (SLIST *, SLIST *, void *);

LOCAL BOOL DataCopy (void *, void *);


#define QMAX        100         // numero maximo de elementos na fila


/*
    Funcao: 
              QDriver

    Proposito: 
              Exemplo de tratamento de fila de prioridade

    Parametros:
              int argc
              char *argv[]

    Retorno:
            EXIT_SUCCESS - exemplo executou corretamente
            EXIT_FAILURE - ocorreu um erro no controlador

    Descricao:
              Le um arquivo de dados consistindo em linhas de texto na 
              forma de X9Message.
              Onde    X = I p/ inserir na fila, R = remover, L = listar a fila
                      9 = prioridade do item da fila
                      Message = string a ser enfileirada
              Obs.: As acoes D e P naoh tem prioridade ou mensagem.
*/
int
QDriver (int argc, char *argv[])
{
  char cRecord[64];             // buffer do texto lido do arquivo
  int iCount;
  void *pTemp;                  // area temporaria de dados
  SLIST *slQueue;               // as duas filas
  SLIST *slFreeList;
  FILE *fin;                    // o arquivo de entrada

  if (argc != 2)
    {
      fprintf (stderr, "ERRO! Uso: prioqueue arquivo.ext\n");
      return (EXIT_FAILURE);
    }

  fin = fopen (argv[1], "rt");
  if (fin == NULL)
    {
      fprintf (stderr, "Naoh conseguiu abrir/localizar %s\n", argv[1]);
      return (EXIT_FAILURE);
    }

  // configura a estrutura de lista ligada para filas
  slQueue = CreateLList (CreateData1,
                         DeleteData1, DuplicatedNode, NodeDataCmp1);

  slFreeList = CreateLList (CreateData2,
                            DeleteData2, DuplicatedNode, NodeDataCmp2);

  if ((slQueue == NULL) || (slFreeList == NULL))
    {
      fprintf (stderr, "Erro criando a fila\n");
      return (EXIT_FAILURE);
    }

  // aloca a lista livre
  for (iCount = 0; iCount < QMAX; ++iCount)
    {
      if (!AddNodeAtHead (slFreeList, cRecord))
        {
          fprintf (stderr, "Naoh conseguiu criar a fila de %d\n", QMAX);
          return (EXIT_FAILURE);
        }
    }

  // comecah a processar o arquivo
  pTemp = CreateData2 (NULL);
  if (pTemp == NULL)
    {
      fprintf (stderr, "ERRO criando area de dados temporaria\n");
      return (EXIT_FAILURE);
    }

  while (fgets (cRecord, sizeof (cRecord) - 1, fin) != NULL)
    {
      if (strlen (cRecord) > 0)
        {
          cRecord[strlen (cRecord) - 1] = '\0'; // elimina o CR/LF
        }

      if (*cRecord == 'I')
        {
          ((pND) pTemp)->uiPriority = *(cRecord + 1) - '0';
          ((pND) pTemp)->pText = cRecord + 2;

          if (InsertQueue (slQueue, slFreeList, pTemp))
            {
              printf ("\tEnfilerou (%d %s)\n", ((pND) pTemp)->uiPriority,
                      ((pND) pTemp)->pText);
            }
          else
            {
              fprintf (stderr, "ERRO ao Enfilerar %d %s\n",
                       ((pND) pTemp)->uiPriority, ((pND) pTemp)->pText);
              return (EXIT_FAILURE);
            }
        }
      else if (*cRecord == 'R')
        {
          if (RemoveQueue (slQueue, slFreeList, pTemp))
            {
              printf ("\tDesenfilerou (%d %s)\n", ((pND) pTemp)->uiPriority,
                      ((pND) pTemp)->pText);
            }
          else
            {
              if (slQueue->uiCount == 0)
                {
                  fprintf (stderr, "ERRO não tem dados para apagar\n");
                }
              else
                {
                  fprintf (stderr, "ERRO ao Desenfilerar %d %s\n",
                           ((pND) slQueue->slkHead->pdata)->uiPriority,
                           ((pND) slQueue->slkHead->pdata)->pText);
                  return (EXIT_FAILURE);
                }
            }

        }
      else if (*cRecord == 'L')
        {
          if (slQueue->uiCount == 0)
            {
              printf ("\n--------------- Fila vazia ----------------\n\n");
            }
          else
            {
              SLINK slkCurr;
              printf ("\n----------- Fila ateh o momento -----------\n");
              for (slkCurr = slQueue->slkHead; slkCurr != NULL;
                   slkCurr = slkCurr->next)
                {
                  printf ("%d %s\n", ((pND) slkCurr->pdata)->uiPriority,
                          ((pND) slkCurr->pdata)->pText);
                }
              printf ("\n");
            }
        }
      else
        {
          fprintf (stderr, "Erro nos dados: %s\n", cRecord);
        }
    }

  fclose (fin);

  // esvazia a fila e libera toda a memoria
  while (RemoveQueue (slQueue, slFreeList, pTemp));
  DestroyLList (slFreeList);
  DestroyLList (slQueue);

  return (EXIT_SUCCESS);
}

/*
    Funcao: 
              InsertQueue

    Proposito: 
              Armazenar um dado baseado em sua prioridade

    Parametros:
              SLIST *slQueue  - lista ligada usada para fila de prioridade
              SLIST *slFree   - lista com as vagas livres a serem inseridas
              void *pNewEntry - a nova entrada a ser guardada na fila

    Retorno:
              TRUE  - dado armazenado
              FALSE - naoh foi possivel armazenar o dado

    Descricao:
              Enfileira os dados passados no inicio da lista livre, e depois
              adiciona o noh na lista baseada na prioridade.
*/
LOCAL BOOL
InsertQueue (SLIST * slQueue, SLIST * slFree, void *pNewEntry)
{
  SLINK slkCurr;
  SLINK slkNewNode;

  // existe algum noh livre na lista
  if (slFree->uiCount == 0)
    {
      fprintf (stderr, "Excedeu o tamanho da fila\n");
      return (FALSE);
    }

  // usa como novo noh, o inicio da lista livre
  slkNewNode = slFree->slkHead;

  if (DataCopy (slkNewNode->pdata, pNewEntry) == FALSE)
    {
      fprintf (stderr,
               "ERRO (InsertQueue), naoh conseguiu copiar os dados\n");
      return (FALSE);
    }

  // adiciona os dados numa lista vazia
  if (slQueue->uiCount == 0)
    {
      // encontrou uma vaga, corrige a lista livre
      // atualiza a o inicio da lista para o seguinte
      slFree->slkHead = slFree->slkHead->next;

      // inicia os ponteiros do novo noh com NULO por ser o primeira insercaoh
      slkNewNode->prior = NULL;
      slkNewNode->next = NULL;

      // indica que o novo noh faz parte da lista de prioridade
      slQueue->slkTail = slkNewNode;
      slQueue->slkHead = slkNewNode;

      slQueue->uiCount = 1;     // inicia o contador da lista de prioridade
      --slFree->uiCount;        // retira uma vaga da lista livre

      return (TRUE);
    }
  // jah existem dados na lista 
  else
    {
      //´percorre a lista para encontrar a posicaoh de insercaoh
      for (slkCurr = slQueue->slkHead;; slkCurr = slkCurr->next)
        {
          if ((slkCurr == NULL) ||      // se a posicaoh for no fim da fila
              (slQueue->fNodeDataCmp (pNewEntry, slkCurr->pdata) < 0))  // ou dentro dele
            {
              // encontrou uma vaga, corrige a lista livre
              // atualiza a o inicio da lista para o seguinte
              slFree->slkHead = slFree->slkHead->next;

              // eh o fim da lista ?
              if (slkCurr == NULL)
                {
                  slkNewNode->prior = slQueue->slkTail; // faz o novo noh apontar para o fim da fila
                  slkNewNode->next = NULL;      // naoh tem para quem apontar
                  slQueue->slkTail->next = slkNewNode;  // o seguinte do fim deve apontar para o novo noh
                  slQueue->slkTail = slkNewNode;        // indica o novo fim da lista
                }
              else
                {
                  if (slkCurr->prior == NULL)   // estah adicionando no inicio da lista ?
                    slQueue->slkHead = slkNewNode;      // atualiza o inicio da lista

                  slkNewNode->prior = slkCurr->prior;   // atualiza os ponteiro do novo noh
                  slkNewNode->next = slkCurr;   // colocando ele entre o atual e anterior

                  if (slkCurr->prior != NULL)   // naoh estah adicionando no inicio
                    slkCurr->prior->next = slkNewNode;  // ajusta o anterior p/ referenciar o novo noh
                  slkCurr->prior = slkNewNode;  // ajusta o atual p/ referenciar o novo noh
                }

              // atualiza a contagem das lista
              // adiciona um noh na lista de prioridade
              ++slQueue->uiCount;
              // subtrai um noh da lista de livres
              --slFree->uiCount;

              return (TRUE);
            }
          // enquanto naoh encontra a posicaoh,
          // mostra os resultados da pesquisa para insercaoh
          else
            {
              pND p1, p2;
              p1 = slkCurr->pdata;
              p2 = pNewEntry;

              //printf( "\tprocurou em (%d %s) para inserir (%d %s)\n",
              //        p1->uiPriority, p1->pText,
              //        p2->uiPriority, p2->pText);
            }
        }
    }
}

/*
    Funcao: 
              RemoveQueue

    Proposito: 
              Retira o dado de uma fila

    Parametros:
              SLIST *slQueue  - lista ligada usada para fila de prioridade
              SLIST *slFree   - lista com as vagas livres a serem inseridas
              void *pOurData  - o dado retirado da fila

    Retorno:
            TRUE  - recuperou a informacaoh da fila
            FALSE - Fila vazia ou erro ao copiar dado para buffer

    Descricao:
              Para desenfileirar, usa um ponteiro para o noh no inicio da 
              fila. Move esse noh inicial retirado da fila e o enfilera na
              lista de nohs livres.
              Obs.: Se naoh os dados naoh forem usados antes que se execute 
              uma nova operacao p/ enfileirar(InsertQueue), os dados seraoh
              perdidos, portanto copie se for nescessario.
*/
LOCAL BOOL
RemoveQueue (SLIST * slQueue, SLIST * slFree, void *pOurData)
{
  SLINK slkRemove;

  // existe algum dado p/ desinfilerar ?
  if (slQueue->uiCount == 0)
    {
      fprintf (stderr, "ERRO, tentou retirar de um fila vazia\n");
      return (FALSE);
    }

  // faz a copia dos dados desenfilerados
  if (DataCopy (pOurData, slQueue->slkHead->pdata) == FALSE)
    {
      fprintf (stderr,
               "ERRO (RemoveQueue), naoh conseguiu copiar os dados\n");
      return (FALSE);
    }

  // salva a posicaoh do dado a ser retirado da fila
  slkRemove = slQueue->slkHead;
  // remove o noh da fila de prioridade
  slQueue->slkHead = slQueue->slkHead->next;    // atualizando o inicio p/ o seguinte da lista

  // sendo diferente de nulo, ainda tem elemento armazenados na fila
  if (slQueue->slkHead != NULL)
    slQueue->slkHead->prior = NULL;     // eliminando a referencia de um previo
  // se o inicio for nulo, fila foi esvaziada, atualiza o fim 
  else
    slQueue->slkTail = NULL;    // naoh existe dados armazenados

  --slQueue->uiCount;           // ajusta a contagem de itens na fila


  // e adiciona no inicio da lista de livres
  slkRemove->prior = NULL;      // aponta o anterior p/ NULO
  slkRemove->next = slFree->slkHead;    // e o seguinte para o inicio da lista de livres
  slFree->slkHead = slkRemove;  // e atualiza o inicio da lista de nohs livres
  ++slFree->uiCount;            // avisa que tem + 1 noh livre

  return (TRUE);
}

// funcaoh para copia de dados
LOCAL BOOL
DataCopy (void *pDest, void *pSrc)
{
  pND nodeSrc, nodeDest;

  nodeSrc = pSrc;
  nodeDest = pDest;

  if ((nodeSrc == NULL) || (nodeDest == NULL))
    return FALSE;

  //printf("\tCopiando %d - %s \t", nodeSrc->uiPriority, nodeSrc->pText);
  nodeDest->uiPriority = nodeSrc->uiPriority;
  strncpy (nodeDest->pText, nodeSrc->pText, TEXT_SIZE);

  return TRUE;
}
