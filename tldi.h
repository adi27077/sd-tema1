/* MUSAT-BURCEA Adrian - 312CB */

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>


#ifndef _LISTA_DUBLU_INLANTUITA_
#define _LISTA_DUBLU_INLANTUITA_

typedef struct celula2
{
  void *info;              /* adresa element extern */
  struct celula2 *pre, *urm;  /* legaturi spre celulele vecine */
} TCelula2, *TLDI;

/*Functii LDI*/
TLDI InitLDI();
int InsDupa(TLDI a, void* x);

#endif
