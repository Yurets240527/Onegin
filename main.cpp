#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>


char * Index(void * data, size_t el_size, size_t i);

int CompareStrUp(const void *adr_a, const void *adr_b);

int CompareStrDown(const void *adr_a, const void *adr_b);

void BubaSort(void *data, size_t len, size_t el_size, int(*Comparator) (const void *a, const void *b));

void Swap(void *value1, void *value2, size_t el_size);

void PrintString(char *str, const char *reason);

int WriteToFile(const char * filename, char * index[]);

int Strcomp(const char *s1, const char *s2);

int StrcompReverse(const char *s1, const char *s2);

char * Strdup(const char *s);

int ReadFile(struct File file, char buffer[]);

int FullIndex(char *index[], char buffer[], size_t size);

int WriteFromBuffer(const char *filename, char buffer[], size_t size);

int CountSymbol(char *buffer, char sym, size_t size);

int SizeOfFile(struct File file);

struct File
{
    char *filename;
    int file_descriptor;
    struct stat st;
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Enter the name of input and output files\n");
        return 1;
    }

    struct File input_file = {};

    input_file.filename = argv[1];

    int max_size = SizeOfFile(input_file);

    char *buffer = (char *) calloc(max_size, sizeof(char));

    int buffer_size = ReadFile(input_file, buffer);

    int num_of_lines = CountSymbol(buffer, '\n', buffer_size) + 1;

    printf("%d\n", buffer_size);
    
    char **index = (char**) calloc(num_of_lines, sizeof(char*));

    int num_of_strings = FullIndex(index, buffer, buffer_size);

    BubaSort(index, num_of_strings, sizeof(index[0]), CompareStrUp);

    WriteToFile(argv[2], index);

    qsort(index, num_of_strings, sizeof(index[0]), CompareStrDown);

    WriteToFile(argv[2], index);

    WriteFromBuffer(argv[2], buffer, buffer_size);

    free(buffer);

}

int Strcomp(const char *s1, const char *s2)
{
    int i = 0;
    int j = 0;

     while (s1[i] != '\0' || s2[j] != '\0')
    {
        while (s1[i] != '\0' && !isalpha(s1[i])) i++;
        while (s2[j] != '\0' && !isalpha(s2[j])) j++;

        if (s1[i] == '\0' && s2[j] == '\0') return 0;

        if (s1[i] == '\0') return -1;
        if (s2[j] == '\0') return 1;

        int c1 = tolower(s1[i]);
        int c2 = tolower(s2[j]);

        if (c1 != c2) return c1 - c2;

        i++;
        j++;
    }

    return 0;

}

int StrcompReverse(const char *s1, const char *s2)
{
    int i = strlen(s1)-1;
    int j = strlen(s2)-1;

    while (i > 0 && j > 0)
    {
        while(!isalpha(s1[i]) && i > 0) i--;
        while(!isalpha(s2[j]) && j > 0) j--;

        if (i <= 0 || j <= 0) break;

        if (tolower(s1[i]) != tolower(s2[j]))
            return s1[i] - s2[j];

        i--;
        j--;

    }


    return s1[i] - s2[j];

}

int CompareStrUp(const void *adr_a, const void *adr_b)
{
    const char* a = *(const char **)adr_a;
    const char* b = *(const char **)adr_b;

    return Strcomp(a,b);
}

int CompareStrDown(const void *adr_a, const void *adr_b)
{
    const char* a = *(const char **)adr_a;
    const char* b = *(const char **)adr_b;

    return StrcompReverse(a,b);
}


void BubaSort(void *data, size_t len, size_t el_size, int(*Comparator) (const void *a, const void *b))
{
    bool need_continue = false;

    for (size_t n = 0; n < len-1; n++)
    {
        need_continue = false;

        for (size_t i = 0; i < len-1-n; i++)
        {

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

int WriteToFile(const char * filename, char * index[])
{
    FILE *fp = fopen(filename, "a");

    for(int i = 0; i<5; i++) fprintf(fp, ". . . . . . . . . . . . . . . . . . .\n");

    int i = 0;
    while (index[i])
    {
        fprintf(fp, "%s\n", index[i]);
        i++;
    }
    fclose(fp);
    return i;
}

char * Strdup(const char *s) {
    char *p;
    p = (char *) calloc(strlen(s) + 1, sizeof(char));
    if (p != NULL) {
        strcpy(p, s);
    }
    return p;
}

int ReadFile(struct File file, char buffer[])
{
    stat(file.filename, &file.st);

    file.file_descriptor = open(file.filename, O_RDONLY);

    size_t max_size = file.st.st_size;

    if (file.file_descriptor == -1) return -1;

    size_t real_buffer_size = read(file.file_descriptor, buffer, max_size);

    close(file.file_descriptor);

    buffer[real_buffer_size] = '\0';

    return real_buffer_size;
}

int FullIndex(char *index[], char buffer[], size_t size)
{
    index[0] = buffer;
    
    int current_index = 1;
    int i = 0;
    int j = 0;
    while (i < size)
    {
        if (buffer[i] == '\0') printf("!\n");
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';

            if (buffer[i+1] != '\n' && buffer[i+1] != '\0')
            {
                j = i + 1;
                while (buffer[j] == ' ') j++;
                index[current_index] = &(buffer[j]);
                current_index++;
            }
        }
        i++;
    }

    index[current_index] = NULL;
    return current_index;
}

int WriteFromBuffer(const char *filename, char buffer[], size_t size)
{
    FILE *fp = fopen(filename, "a");

    printf("burmalda\n");

    for(int i = 0; i<5; i++) fprintf(fp, ". . . . . . . . . . . . . . . . . . .\n");

    for(int i = 0; i < size; i++)
    {
        if (buffer[i]) putc(buffer[i], fp);

        else putc('\n', fp);
    }

    fclose(fp);
}

int CountSymbol(char *buffer, char sym, size_t size)
{
    int count = 0;
    for (int i = 0; i < size; i++)
        if (buffer[i] == sym) count++;

    return count;
}

int SizeOfFile(struct File file)
{
    stat(file.filename, &file.st);

    file.file_descriptor = open(file.filename, O_RDONLY);

    size_t max_size = file.st.st_size;

    if (file.file_descriptor == -1) return -1;
    
    return max_size;
}