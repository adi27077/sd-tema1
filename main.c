/* MUSAT-BURCEA Adrian - 312CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "thash.h"
#include "tldi.h"

#define LENGTH 30

typedef struct 
{
    char *key;
    char *value;
}TDNS;

//functia de calculare codhash
int codHash(void *elem, size_t M)
{
    TDNS *dns = (TDNS *)elem;
    int l = strlen(dns->key), i, s = 0;
    for(i = 0; i < l; i++)
        s += dns->key[i];
    return s % M;
}

//functia de comparatie a 2 elem in functie de key
int compDNS(void *elem1, void *elem2)
{
    TDNS *dns1 = (TDNS *)elem1;
    TDNS *dns2 = (TDNS *)elem2;
    return strcmp(dns1->key, dns2->key);
}

//functie pt afisarea valorii unui element
void AfiDNS_Value(void *elem)
{
    TDNS *dns = (TDNS *)elem;
    printf("%s ", dns->value);
}

//scrierea in fisier a valorii unui element
void PrintDNS_Value(void *elem, FILE *g)
{
    TDNS *dns = (TDNS *)elem;
    fprintf(g, "%s", dns->value);
}

//functia de eliberare element
void freeDNS(void *elem)
{
    TDNS *dns = (TDNS *)elem;
    free(dns->key);
    free(dns->value);
    free(dns);
}


int main(int argc, char *argv[])
{
    int index; //pt print_bucket
    size_t M = atoi(argv[1]);
    char *input_file = argv[2];
    char *output_file = argv[3];
    size_t n = 2;
    char *line = malloc(n * sizeof(char));
    if(line == NULL)
        return 1;
    char *operation  = malloc(LENGTH * sizeof(char));
    if(operation == NULL)
    {
        free(line);
        return 1;
    }
    char *key = malloc(LENGTH * sizeof(char));
    if(key == NULL)
    {
        free(line);
        free(operation);
        return 1;
    }
    char *value = malloc(LENGTH * sizeof(char));
    if(value == NULL)
    {
        free(line);
        free(operation);
        free(key);
        return 1;
    }
    FILE *f = fopen(input_file, "rt");
    FILE *g = fopen(output_file, "wt");
    TDNS *dns;
    TH *h = IniTH(M, codHash); //initializare tabela hash

    //citim fiecare linie din fisier
    while(getline(&line, &n, f) != -1)
    {
        if(strcmp(line, "\n") == 0) //verificare linie goala
            continue;
        
        sscanf(line, "%s", operation); //citire operatie

        dns = malloc(sizeof(TDNS));
        if(dns == NULL)
        {
            free(line);
            free(operation);
            free(key);
            free(value);
            return 1;
        }
        dns->key = malloc(LENGTH * sizeof(char));
        if(dns->key == NULL)
        {
            free(line);
            free(operation);
            free(key);
            free(value);
            free(dns);
            return 1;
        }
        dns->value = malloc(LENGTH * sizeof(char));
        if(dns->value == NULL)
        {
            free(line);
            free(operation);
            free(key);
            free(value);
            free(dns->key);
            free(dns);
            return 1;
        }

        //operatia put <key> <value>
        if(strcmp(operation, "put") == 0)
        {
            sscanf(line, "%*s %s %s", key, value);
            strcpy(dns->key, key);
            strcpy(dns->value, value);

            //inserare ordonata in tabela hash
            int rez = InsTH(h, dns, compDNS);
            if(!rez) //daca elementul nu a fost inserat, il eliberam
            {
                free(dns->key);
                free(dns->value);
                free(dns);
            }
            /*daca elementul e inserat, eliberarea lui se va face de catre
             DistrTH la sfarsit */
            continue;

        }

        //operatia print
        if(strcmp(operation, "print") == 0)
        {
            PrintTH(h, g, PrintDNS_Value);

            //pt afisare nu avem nevoie de aceste date, asa ca le eliberam
            free(dns->key);
            free(dns->value);
            free(dns);
            continue;

        }

        //operatia get <key>
        if(strcmp(operation, "get") == 0)
        {
            sscanf(line, "%*s %s", key);
            strcpy(dns->key, key);

            //verificare existenta key in tabela hash
            TDNS *rez = ExistaTH(h, dns, compDNS);
            free(dns->key);
            free(dns->value);
            free(dns);

            if(rez == NULL) //key nu exista in tabela hash
            {
                fprintf(g, "NULL\n");
                continue;
            }

            //afisare valoare corespunzatoare key
            PrintDNS_Value(rez, g);
            fprintf(g, "\n");
            
            continue;
            
        }

        //operatia remove <key>
        if(strcmp(operation, "remove") == 0)
        {
            sscanf(line, "%*s %s", key);
            strcpy(dns->key, key);

            //eliminare element corespunzator key
            ExtrTH(h, dns, compDNS, freeDNS);
            
            free(dns->key);
            free(dns->value);
            free(dns);
            continue;
        }

        //operatia find <key>
        if(strcmp(operation, "find") == 0)
        {
            sscanf(line, "%*s %s", key);
            strcpy(dns->key, key);

            //verificare existenta key in tabela hash
            TDNS *rez = ExistaTH(h, dns, compDNS);
            free(dns->key);
            free(dns->value);
            free(dns);
            if(rez == NULL)
                fprintf(g, "False\n");
            else
                fprintf(g, "True\n");
            continue;
        }

        //operatia print_bucket <index>
        if(strcmp(operation, "print_bucket") == 0)
        {
            sscanf(line, "%*s %d", &index);

            if(index >= h->M) //indexul nu poate fi > dimensiunea tabelei
            {
                free(dns->key);
                free(dns->value);
                free(dns);
                continue;
            }

            //afisare lista <index> din tabela hash
            PrintBucket(h, g, PrintDNS_Value, index);

            //pt afisare nu avem nevoie de aceste date, asa ca le eliberam
            free(dns->key);
            free(dns->value);
            free(dns);
            continue;

        }

        //daca operatia e invalida, eliberam memoria
        free(dns->key);
        free(dns->value);
        free(dns);
    }

    free(operation);
    free(key);
    free(value);
    free(line);
    DistrTH(&h, freeDNS);
    fclose(f);
    fclose(g);
    return 0;
}
