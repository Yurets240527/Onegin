#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINES 100
#define MAXLEN 100000

char * Index(void * data, size_t el_size, size_t i);

int CompareStrUp(const void *adr_a, const void *adr_b);

int CompareDoubleUp(const void *adr_a, const void *adr_b);

void BubaSort(void *data, size_t len, size_t el_size, int(*Comparator) (const void *a, const void *b));

void Swap(void *value1, void *value2, size_t el_size);

void PrintString(char *str, const char *reason);

int ReadFromFile(char * filename, char * index[]);

int main()
{
    //PrintString("yaitsa", "PO_ROFLU");

    char * index[MAXLINES] = {};

    int num_of_strings = ReadFromFile("Onegin.txt", index);

    //printf("%s\n", index[0]);

    //PrintString(index[0], "POROFLU");

    BubaSort(index, num_of_strings, sizeof(index[0]), CompareStrUp);

    printf("%s", index[0]);
    

}



int CompareStrUp(const void *adr_a, const void *adr_b)
{
    const char* a = *(const char **)adr_a;
    const char* b = *(const char **)adr_b;

    return strcmp(a,b);
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

int ReadFromFile(char * filename, char * index[])
{
    FILE *fp = fopen(filename, "r");

    char buffer[MAXLEN] = "";
    int i = 0;
    while (fgets(buffer, MAXLINES, fp))
    {
        //buffer[strcspn(buffer, "\n")] = '\0';
        index[i] = strdup(buffer);
        i++;
    }

    fclose(fp);
    return i;
}