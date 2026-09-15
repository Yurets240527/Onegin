#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 100;
char * Index(void * data, size_t el_size, size_t i);

int CompareIntUp(const void *adr_a, const void *adr_b);

int CompareDoubleUp(const void *adr_a, const void *adr_b);

void BubaSort(void *data, size_t len, size_t el_size, int(*Comparator) (const void *a, const void *b));

void Swap(void *value1, void *value2, size_t el_size);

void PrintString(char *str, const char *reason);

int main()
{
    //PrintString("yaitsa", "PO_ROFLU");

}



int CompareIntUp(const void *adr_a, const void *adr_b)
{
    const int a = *(const int *)adr_a;
    const int b = *(const int *)adr_b;

    return (a - b);
}

int CompareDoubleUp(const void *adr_a, const void *adr_b)
{
    const double a = *(const double *)adr_a;
    const double b = *(const double *)adr_b;

    return (a - b);
}

void BubaSort(void *data, size_t len, size_t el_size, int(*Comparator) (const void *a, const void *b))
{
    bool need_continue = false;

    for (size_t n = 0; n < len-1; n++)
    {

        for (size_t i = 0; i < len-1-n; i++)
        {
            need_continue = false;

            if (Comparator((Index(data, el_size, i)), (Index(data, el_size, i+1))) > 0)
            {
                Swap((Index(data, el_size, i)), (Index(data, el_size, i+1)), el_size);

                need_continue = true;
            }


        }

        if (!need_continue) break;
    }

}

void Swap(void *value1, void *value2, size_t el_size)
{
    void *temp = malloc(el_size);

    memcpy(temp, value2, el_size);

    memcpy(value2, value1, el_size);

    memcpy(value1, temp, el_size);

    free(temp);
}


void PrintString(char *str, const char *reason)
{
    printf("\n<%s>  len = %d, %s", str, strlen(str), reason);
}

char * Index(void * data, size_t el_size, size_t i)
{
    return (char *) data + el_size*i; 
}