/*
  Arquivo: QAPP.H
  Autor: Roberto Bauer
  Observacoes:
              Dados especificos da aplicacaoh de filas
*/

#ifndef __QAPP_H__
#define __QAPP_H__

#undef EXTERN
#ifdef __QAPP_C__
#define EXTERN
#else
#define EXTERN  extern
#endif

#define TEXT_SIZE       60      // tamanho maximo para alocacaoh do texto



typedef struct tagSNODEDATA
{
  char *pText;                  // ponteiro para um texto
  unsigned int uiPriority;      // o nivel de prioridade
} SNODEDATA;

// A fila e a a lista livre tem nohs identicos, usa a mesma estrutura
typedef SNODEDATA *pND;

EXTERN void *CreateData1 (void *);
EXTERN BOOL DeleteData1 (void *);
EXTERN int NodeDataCmp1 (void *, void *);


EXTERN void *CreateData2 (void *);
EXTERN BOOL DeleteData2 (void *);
EXTERN int NodeDataCmp2 (void *, void *);

EXTERN int DuplicatedNode (SLINK, SLINK);

#endif // #define __QAPP_H__
