/* MUSAT-BURCEA Adrian - 312CB */

#include "thash.h"

//initializare tabela hash
TH* IniTH(size_t M, TFHash fh)
{
    //alocare tabela hash
    TH* h = (TH *) calloc(sizeof(TH), 1);
    if (!h) 
    {
        printf("Eroare alocare hash\n");
        return NULL;
    }
    //alocare vector de pointeri la TLDI
    h->v = (TLDI *) calloc(M, sizeof(TLDI));
    if(!h->v) {
        printf("Eroare alocare vector de pointeri TLDI in hash\n");
        free(h);
        return NULL;
    }

    h->M = M;
    h->fh = fh;
    return h;
}

//eliberare tabela hash
void DistrTH(TH** ah, TF fe)
{
    TLDI * p, el, aux;

    //parcurgere cu pointeri
    for (p = (*ah)->v; p < (*ah)->v + (*ah)->M; p++) 
    {
        el = *p;
        if(el == NULL) //daca lista acestui hash e nula, nu avem ce elibera
            continue;
        do 
        {
            //eliberam fiecare celula din lista
            aux = el;
            el = el->urm;
            fe(aux->info);
            free(aux);
        }while(el != *p);
    }
    free((*ah)->v); //eliberare vector de pointeri
    free(*ah); //eliberare tabela
    *ah = NULL;
}

//afisare tabela in fisier
void PrintTH(TH* ah, FILE *g, TAFile afiEl)
{
    TLDI p, el;
    int i;
    for(i = 0; i < ah->M; i++) 
    {
        p = ah->v[i];
        if(p) //daca avem lista la acest hash, o afisam
        {
            fprintf(g ,"%d: ", i);
            el = p;
            do
            {
                afiEl(el->info, g);
                fprintf(g, " ");
                el = el->urm;
            }while (el != p); 
            fprintf(g, "\n");
        }
    }
}

//afisare bucket
void PrintBucket(TH* ah, FILE *g, TAFile afiEl, int n)
{
    TLDI p, el; 
    p = ah->v[n];
    if(p) //daca bucketul nu este vid, il afisam
    {
        el = p;
        do
        {
            afiEl(el->info, g);
            fprintf(g, " ");
            el = el->urm;
        }while (el != p); 
        fprintf(g, "\n");
    }
    else //bucket vid
        fprintf(g, "VIDA\n");

}

//inserare ordonata in tabela hash
int InsTH(TH* a, void* ae, TFCmp f)
{
    int cod = a->fh(ae, a->M), rez;
    TLDI el;

    //daca lista acestui hash nu exista o initializam cu primul element
    if(a->v[cod] == NULL)
    {
        a->v[cod] = InitLDI();
        el = a->v[cod];
        el->info = ae;
        return 1;
    }
    //daca elementul exista deja, nu il adaugam
    el = a->v[cod];
    do
    {
        if (f(el->info, ae) == 0)
            return 0;
        el = el->urm;
    }while (el != a->v[cod]);
    

    //inserare ordonata
    
    el = a->v[cod];
    if(el == el->urm) //cazul in care in lista este un singur element
    {
        rez = InsDupa(el, ae);
        if(f(ae, el->info) <= 0) /*daca elementul este mai mic decat inceputul
                                listei, mutam inceputul pe acest element*/
            a->v[cod] = el->pre;
        return rez;
    }
    if(f(ae, el->info) <= 0) /*daca elementul e mai mic decat primul din lista
                            il punem inainte si mutam inceputul listei pe el*/
    {
        rez = InsDupa(el->pre, ae);
        a->v[cod] = el->pre;
        return rez;
    }

    /*daca elementul este mai mare decat primul, parcurgem lista pana
    il inseram in locul potrivit*/
    do
    {
        if(f(ae, el->info) >= 0 && f(ae, el->urm->info) <= 0)
        {
            rez = InsDupa(el, ae);
            return rez;
        }
        el = el->urm;
    }while(el != a->v[cod]);
    
    //daca elementul este cel mai mare il inseram la sfarsit
    el = a->v[cod];
    rez = InsDupa(el->pre, ae);
    return rez;
    
}

//cautare element in tabela hash
void *ExistaTH(TH *h, void *ae, TFCmp fcmp)
{
    int cod = h->fh(ae, h->M);
    TLDI el;
    el = h->v[cod];
    if(el == NULL) //lista asociata hashului nu exista
        return NULL;
    do
    {
        if(fcmp(ae, el->info) == 0) //am gasit elementul
            return el->info;
        el = el->urm;
    }while(el != h->v[cod]);
    //nu s-a gasit elementul
    return NULL;
}

//eliminare din tabela hash
int ExtrTH(TH *h, void *ae, TFCmp fcmp, TF fe)
{
    int cod = h->fh(ae, h->M);
    TLDI el = h->v[cod], aux;
    if(el == NULL) //lista asociata hashului nu exista
        return 0;

    //cazul in care lista are un singur element si acesta trb eliminat
    if(el == el->urm && fcmp(ae, el->info) == 0)
    {
        fe(el->info);
        free(el);
        h->v[cod] = NULL; //lista devine vida
        return 1;
    }
    do
    //parcurgerea listei
    {
        if(fcmp(ae, el->info) == 0) //gasire element
        {
            aux = el;
            if(el == h->v[cod]) //cazul eliminarii primului element
            {
                el->pre->urm = el->urm;
                el->urm->pre = el->pre;
                h->v[cod] = el->urm; //actualizare inceput lista
            }
            else
            {
                el->pre->urm = el->urm;
                el->urm->pre = el->pre;
            }
            fe(aux->info);
            free(aux);
            return 1;
        }
        el = el->urm;
    }while(el != h->v[cod]);
    return 0; //element negasit
}
