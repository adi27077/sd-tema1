/* MUSAT-BURCEA Adrian - 312CB */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "tldi.h"

#ifndef _TAB_HASH_
#define _TAB_HASH_

typedef int (*TFElem)(void*);     /* functie prelucrare element */
typedef int (*TFCmp)(void*, void*); /* functie de comparare doua elemente */
typedef void (*TF)(void*);     /* functie afisare/eliberare un element */
typedef void (*TAFile)(void*, FILE *); /*functie afisare element in fisier*/
typedef int (*TFHash)(void*, size_t M); /*functie hash*/

typedef struct
{
    size_t M;
    TFHash fh;
    TLDI *v;
} TH;

/* functii tabela hash */
TH* IniTH(size_t M, TFHash fh);
void DistrTH(TH**aa, TF fe);
void PrintTH(TH* ah, FILE *g, TAFile afiEl);
void *ExistaTH(TH*a, void*ae, TFCmp f);
int InsTH(TH*a, void*ae, TFCmp f);
int ExtrTH(TH*a, void*ae, TFCmp f, TF fe);
void PrintBucket(TH* ah, FILE *g, TAFile afiEl, int n);

#endif