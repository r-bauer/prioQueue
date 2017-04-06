/*
  Arquivo: MAIN.C                                                          
  Autor: Roberto Bauer                                                     
  Observacoes: Arquivo central do programa, funcaoh main()
*/

#define __MAIN_C__

#include "main.h"

int
main (int argc, char *argv[])
{
  int i;

  i = QDriver (argc, argv);

  return (i);
}
