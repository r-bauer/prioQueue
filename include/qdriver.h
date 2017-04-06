/*
  Arquivo: QDRIVER.H
  Autor: Roberto Bauer
  Observacoes:
              Le textos de um arquivo especifico da linha de comando
              e o coloca em duas listas ligadas. Entaoh executa uma
              variedade de atividades de lista ligada, imprimindo o
              resultado a cada passo.
*/

#ifndef __QDRIVER_H__
#define __QDRIVER_H__

#undef EXTERN
#ifdef __QDRIVER_C__
#define EXTERN
#else
#define EXTERN  extern
#endif

EXTERN int QDriver (int argc, char *argv[]);


#endif // #define __QDRIVER_H__
