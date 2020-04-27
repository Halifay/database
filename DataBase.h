//
// Created by mikhail on 18.04.2020.
//
#ifndef LAB_9_DATABASE_H
#define LAB_9_DATABASE_H

#include <assert.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>

int const str_max_size = 32;

typedef struct entry_ {
    void **data = NULL;
    void *comp_field = NULL;
} entry;

typedef struct table_ {
    int arg_amt = 0;
    char **arg_types = NULL;
    char **titles = NULL;
    int size = 0;
    int capacity = 0;
    entry *list = NULL;
} table;

void destroy_table(table *);
void destroy_entry(table *, entry *);
entry copy_entry(table *, entry *);
void print_table(table *);
table get_new_table();
int is_str(table *, int);
int is_int(table *, int);
char *make_str_copy(char *);
void add_column(table *, char *, char *, void *);
void add_new_column(table *);
void add_entry(table *, entry *);
entry make_new_entry(table *);
void add_new_entry(table *);
void open_table(table *, char *);
entry get(table *, int);
bool equal(table *, entry *, entry *);
int int_comparison(const void *, const void *);
int str_comparison(const void *, const void *);
void sort_by_field(table *, int);
void save_table(table *, char *);
int find_entry(table *, entry *);
void delete_entry(table *, entry *);
table get_entries_by_field(table *, void *, int);
table get_entries(table *);
void substitute_entry(table *, entry *, entry *);
int get_int_width(int);
int get_str_width(char *);
int get_width(table *, int, int);


void destroy_table(table *db)
{

}

//poryadok eto otstoy
void destroy_entry(table *db, entry *slated)
{

}

entry copy_entry(table *db, entry *one)
{
    entry result;
    result.data = (void **)calloc(db->arg_amt, sizeof(void *));
    for(int j = 0; j < db->arg_amt; j++)
    {
        if(is_str(db, j))
        {
            char * new_string = make_str_copy((char *)((*one).data[j]));
            result.data[j] = new_string;
        }
        if(is_int(db, j))
        {
            result.data[j] = (void *)calloc(1, sizeof(int));
            ((int *)result.data[j])[0] = ((int *)(*one).data[j])[0];
        }
    }
    return result;
}


table get_new_table()
{
    table ret;
    ret.arg_amt = 0;
    ret.size = 0;
    ret.capacity = 0;

    return ret;
}

//functions for checking type of field under given number in arg_types array
int is_str(table *db, int field)
{
    return strcmp(db->arg_types[field], "str") == 0;
}

int is_int(table *db, int field)
{
    return strcmp(db->arg_types[field], "int") == 0;
}

char *make_str_copy(char *origin)
{
    char *result = (char *)calloc(strlen(origin), sizeof(char *));
    for(int i = 0; i < strlen(origin); i++)
    {
        result[i] = origin[i];
    }

    return result;
}

//to make new columns in a table (for construction of a new table)
void add_column(table *db, char *title, char *type, void* value)
{
    db->arg_amt++;
    if(db ->arg_amt == 1)
    {
        db->titles = (char **)calloc(db->arg_amt, sizeof(char **));
        db->arg_types = (char **)calloc(db->arg_amt, sizeof(char **));
    }
    else
    {
        db->titles = (char **)realloc(db->titles, db->arg_amt*sizeof(char **));
        db->arg_types = (char **)realloc(db->arg_types, db->arg_amt*sizeof(char **));
    }
    db->titles[db->arg_amt - 1] = title;
    db->arg_types[db->arg_amt - 1] = type;

    // assigning default value to existing entries
    for(int i = 0; i < db->size; i++)
    {
        if(db->arg_amt == 1)
            db->list[i].data = (void**)calloc(db->arg_amt, sizeof(void **));
        else
            db->list[i].data = (void**)realloc(db->list[i].data, db->arg_amt*sizeof(void **));
        db->list[i].data[db->arg_amt - 1] = value;
    }
}

void add_new_column(table *db)
{
    char *title, *type;
    title = (char *)calloc(32, sizeof(char *));
    void *value;
    printf("Enter title for a new column: ");
    scanf("\n%[^\n]s", title);
    const char *typelist[] = {"int", "str"};
    printf("Enter number of corresponding type\n");
    int *typen = (int *)malloc(sizeof(int));
    while(true)
    {
        for (int i = 0; i < 2; i++)
        {
            printf("%s - %d\n", typelist[i], i);
        }
        scanf("%d", typen);
        if(*typen >= 0 && *typen < 2)
            break;
        printf("Incorrect number, please try again: ");
    }
    type = (char *)typelist[*typen];
    printf("Enter default value for this field in all existing entries: ");
    if(*typen == 0)
    {
        int *input = (int *)malloc(sizeof(int));
        scanf("%d", input);
        value = (void *)input;
    }
    if(*typen == 1)
    {
        char *input = (char *)calloc(str_max_size, sizeof(char *));
        scanf("%s", input);
        value = (void *)input;
    }
    add_column(db, title, type, value);
    printf("New column has been successfully added to database\n");
}

// need to change this method for more safety
void add_entry(table *db, entry *new_entry)
{
    if(db->size == 0)
    {
        db->capacity = 10;
        db->list = (entry *)calloc((db->size)*sizeof(entry), sizeof(entry));
    }
    db->size++;
    if(db->size > db->capacity)
    {
        db->capacity*=2;
        db->list = (entry *)realloc(db->list, db->capacity*sizeof(entry));
    }
    db->list[db->size-1] = copy_entry(db, new_entry);
}

entry make_new_entry(table *db)
{
    entry new_entry;
    new_entry.data = (void **)calloc(db->arg_amt, sizeof(void *));
    for(int i = 0; i < db->arg_amt; i++)
    {
        printf("Type %s (%s value) for a new entry: ", db->titles[i], db->arg_types[i]);
        if(is_str(db, i))
        {
            new_entry.data[i] = calloc(str_max_size, sizeof(char));
            scanf("%s", (char *)(new_entry.data[i]));
        }
        if(is_int(db, i))
        {
            int *to_read = (int *)malloc(sizeof(int));
            scanf("%d", to_read);
            new_entry.data[i] = (void *)(to_read);
        }
    }
    return new_entry;
}

void add_new_entry(table *db)
{
    entry new_entry = make_new_entry(db);
    add_entry(db, &new_entry);
}

void open_table(table *db, char *filename)
{
    FILE *file = fopen(filename, "r");
    fscanf(file, "%d ", &(db->arg_amt));
    fscanf(file, "%d\n", &(db->size));
    db->arg_types = (char **)calloc(db->arg_amt, sizeof(char *));
    db->titles = (char **)calloc(db->arg_amt, sizeof(char *));
    for(int i = 0; i < db->arg_amt; i++)
    {
        db->arg_types[i] = (char *)calloc(str_max_size, sizeof(char));
        db->titles[i] = (char *)calloc(str_max_size, sizeof(char));
        fscanf(file, "%s %[^\n]s\n", db->arg_types[i], db->titles[i]);
        // printf("%d:%s\n", i, db->arg_types[i]);
    }
    db->capacity = db->size;
    db->list = (entry *)calloc(db->capacity, sizeof(entry));
    for(int i = 0; i < db->size; i++)
    {
        db->list[i].data = (void **)calloc(db->arg_amt, sizeof(void *));
        for(int j = 0; j < db->arg_amt; j++)
        {
            if(is_str(db, j))
            {
                // I wonder if this trick would work
                // But maybe it is even unnecessary
                db->list[i].data[j] = calloc(str_max_size, sizeof(char));
                fscanf(file, "%s ", (char *) (db->list[i].data[j]));
            }
            if(is_int(db, j))
            {
                //this too
                db->list[i].data[j] = malloc(sizeof(int));
                fscanf(file, "%d ", (int *)(db->list[i].data[j]));
            }
        }
    }
    fclose(file);
}

entry get(table *db, int number)
{
    assert(db->size > number);
    assert(number >= 0);
    return (db->list[number]);
}

bool equal(table *db, entry *first, entry *second)
{
    bool ans = true;
    for(int i = 0; i < db->arg_amt; i++)
    {
        if(is_str(db, i))
            ans &= (strcmp((char *)(first->data[i]), (char *)(second->data[i])) == 0);
        if(is_int(db, i))
            ans &= (*(int *)(first->data[i]) == *(int *)(second->data[i]));
    }
    return ans;
}

int int_comparison(const void *elem1, const void *elem2)
{
    entry f = *((entry *)elem1);
    entry s = *((entry *)elem2);
    return (*(int *)elem1 > *(int *)elem2);
}

int str_comparison(const void *elem1, const void *elem2)
{
    entry f = *((entry *)elem1);
    entry s = *((entry *)elem2);
    return strcmp((char *)f.comp_field, (char *)s.comp_field);
}

void sort_by_field(table *db, int field)
{
    for(int i = 0; i < db->size; i++)
    {
        db->list[i].comp_field = (void *)(db->list[i].data[field]);
    }
    if(is_str(db, field))
    {
        qsort(db->list, db->size, sizeof(entry), str_comparison);
        return;
    }
    if(is_int(db, field))
    {
        qsort(db->list, db->size, sizeof(entry), int_comparison);
        return;
    }
    //It must end before
    assert(false);
}

void save_table(table *db, char *filename)
{
    FILE *file = fopen(filename, "w");
    fprintf(file, "%d ", db->arg_amt);
    fprintf(file, "%d\n", db->size);
    for(int i = 0; i < db->arg_amt; i++)
    {
        fprintf(file, "%s %s\n", db->arg_types[i], db->titles[i]);
    }

    for(int i = 0; i < db->size; i++)
    {
        fprintf(file, "\n");
        for(int j = 0; j < db->arg_amt; j++)
        {
            if(is_str(db, j))
            {
                fprintf(file, "%s ", (char *)db->list[i].data[j]);
            }
            if(is_int(db, j))
            {
                fprintf(file, "%d ", *(int *)db->list[i].data[j]);
            }
        }
    }
    fclose(file);
}


int find_entry(table *db, entry *one)
{
    int number = db->size;
    for(int i = 0; i < db->size; i++)
    {
        if(equal(db, one, &(db->list[i])))
        {
            number = i;
            break;
        }
    }
    return number;
}

void delete_entry(table *db, entry *one)
{
    int start = find_entry(db, one);
    if(start < db->size)
        db->size--;

    for(int i = start; i < db->size; i++)
    {
        db->list[i] = db->list[i+1];
    }
}

table get_entries_by_field(table *db, void* field, int type)
{
    table result = get_new_table();
    result.arg_amt = db->arg_amt;
    result.arg_types = db->arg_types;
    result.titles = db->titles;
    for(int i = 0; i < db->size; i ++)
    {
        if(is_str(db, type))
        {
            if(strcmp((char *)(db->list[i].data[type]), (char *)field) == 0)
            {
                add_entry(&result, &db->list[i]);
            }
        }
        if(is_int(db, type))
        {
            if(*(int *)(db->list[i].data[type]) == *(int *)(field))
            {
                entry *to_add = (entry *)calloc(1, sizeof(entry));
                *to_add = copy_entry(db, &db->list[i]);
                add_entry(&result, to_add);
            }
        }
    }

    return result;
}

table get_entries(table *db)
{
    int type;
    void *value;
    printf("Enter number of corresponding column\n");
    for(int i = 0; i < db->arg_amt; i++)
    {
        printf("%s - %d\n", db->titles[i], i);
    }
    scanf("%d", &type);
    while(type < 0 || type >= db->arg_amt)
    {
        printf("Invalid option '%d'. Please, try again: ", type);
        char checker = 'q';
        while(checker != '\n')
        {
            checker = getchar();
        }
        scanf("%d", &type);
    }

    printf("Enter value of field (%s value): ", db->arg_types[type]);
    if(is_str(db, type));
    {
        value = calloc(str_max_size, sizeof(char));
        scanf("%s", (char *)value);
    }
    if(is_int(db, type))
    {
        value = malloc(sizeof(int));
        scanf("%d", (int *)value);
    }
    return get_entries_by_field(db, value, type);
}

void substitute_entry(table *db, entry *before, entry *after)
{
    int pos = find_entry(db, before);
    if(pos < db->size)
        db->list[pos] = *after;
}

int get_int_width(int field)
{
    int res = 0;
    if(field < 0)
    {
        field *= -1;
        res++;
    }

    while(field > 0)
    {
        res++;
        field /= 10;
    }
    if(res == 0)
        res++;
    return res;
}

int get_str_width(char * field)
{
    return strlen(field);
}

int get_width(table *db, int entrynum, int field)
{
    if(is_str(db, field))
    {
        return get_str_width((char *)(db->list[entrynum].data[field]));
    }
    if(is_int(db, field))
    {
        return get_int_width(*(int *)(db->list[entrynum].data[field]));
    }
}

void print_table(table *db)
{
    int number_width = (1<db->size?get_int_width(db->size):1);
    int *widths = NULL;
    //there is problem  with zero memory allocation
    widths = (int *)calloc(db->arg_amt, sizeof(int));

    //counting all widths for fields in table format
    for(int i = 0; i < db->arg_amt; i++)
    {
        widths[i] = get_str_width(db->titles[i]);
        for(int j = 0; j < db->size; j++)
        {
            if(is_str(db, i))
            {
                int width = get_str_width((char *)(db->list[j].data[i]));
                if(widths[i] < width)
                    widths[i] = width;
            }
            if(is_int(db, i))
            {
                int width = get_int_width(*(int *)db->list[j].data[i]);
                if(widths[i] < width)
                    widths[i] = width;
            }
        }
    }

    //printing titles in table format to terminal
    printf("||%*s|", number_width, "№");
    for(int i = 0; i < db->arg_amt; i++)
    {
            printf("|%*s|", widths[i], (char *)db->titles[i]);
    }
    printf("|\n");

    //printing entries by values in table format
    for(int i = 0; i < db->size; i++)//for each entry
    {
        printf("||%*d|", number_width, i + 1);
        entry ent = db->list[i];
        for(int j = 0; j < db->arg_amt; j++)//for each argument in entry
        {
            if(is_str(db, j))
            {
                printf("|%*s|", widths[j], (char *)db->list[i].data[j]);
            }
            if(is_int(db, j))
            {
                printf("|%*d|", widths[j], *((int *)(db->list[i].data[j])));
            }
        }
        printf("|\n");
    }
}

#endif //LAB_9_DATABASE_H
