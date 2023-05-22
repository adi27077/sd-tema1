/* MUSAT-BURCEA Adrian - 312CB */

#include "tldi.h"

/* inserare dupa celula cu adresa a*/
int InsDupa(TLDI a, void* x)
{
	TLDI aux = (TLDI)malloc(sizeof(TCelula2));
	if(!aux) 
		return 0;
	aux->info = x;
	aux->pre = a;
	aux->urm = a->urm;
	a->urm->pre = aux;
	a->urm = aux;
    return 1;
}

//initializare lista dublu inlantuita circulara generica
TLDI InitLDI()
{
	TLDI aux = (TLDI)malloc(sizeof(TCelula2));
    if (!aux) 
		return NULL;

	aux->pre = aux->urm = aux;
    aux->info = NULL;

	return aux;     
}
