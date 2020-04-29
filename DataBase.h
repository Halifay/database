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


//two functions for freeing memory from entries and databases
void destroy_table(table *db)
{
    for(int i = 0; i < db->size; i++)
    {
        destroy_entry(db, &db->list[i]);
    }
    // if(db->list != NULL)
    //     free(db->list);

    for(int i = 0; i < db->arg_amt; i++)
    {
        free(db->titles[i]);
        free(db->arg_types[i]);
    }
    if(db->arg_types != NULL)
    {
        free(db->titles);
        free(db->arg_types);
    }
}

//sends entry with given number to a better world
void delete_entry(table *db, entry *one)
{
    //why did the entry cross the road?
     int start = find_entry(db, one);
     if(start < db->size)
     {
         destroy_entry(db, &db->list[find_entry(db, one)]);
         db->size--;
     }

     for(int i = start; i < db->size; i++)
     {
         db->list[i] = db->list[i+1];
     }
}

//poryadok eto otstoy
void destroy_entry(table *db, entry *slated)
{
    for(int i = 0; i < db->arg_amt; i++)
    {
        // free(slated->data[i]);
    }
    if(slated->comp_field != NULL)
        free(slated->comp_field);
    if(slated->comp_field != NULL)
        free(slated->data);
}

entry copy_entry(table *db, entry *one)
{
    entry result;
    result.data = (void **)calloc(db->arg_amt, sizeof(void *));
    for(int j = 0; j < db->arg_amt; j++)
    {
        if(is_str(db, j))
        {
            result.data[j] = make_str_copy((char *)(one->data[j]));
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
    char *result = (char *)calloc(str_max_size, sizeof(char));
    for(int i = 0; i < strlen(origin) + 1; i++)
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
    db->titles[db->arg_amt - 1] = make_str_copy(title);
    db->arg_types[db->arg_amt - 1] = make_str_copy(type);

    // assigning default value to existing entries
    for(int i = 0; i < db->size; i++)
    {
        if(db->arg_amt == 1)
            db->list[i].data = (void**)calloc(db->arg_amt, sizeof(void **));
        else
            db->list[i].data = (void**)realloc(db->list[i].data, db->arg_amt*sizeof(void **));
        if(is_str(db, db->arg_amt - 1))
        {
            // db->list[i].data[db->arg_amt - 1]
            db->list[i].data[db->arg_amt - 1] = make_str_copy((char *)value);
        }
        if(is_int(db, db->arg_amt - 1))
        {
            db->list[i].data[db->arg_amt - 1] = malloc(sizeof(int));
            ((int *)db->list[i].data[db->arg_amt - 1])[0] = *(int *)value;
        }
    }
}

//interface for making new columns
void add_new_column(table *db)
{
    char *title, *type;
    title = (char *)calloc(str_max_size, sizeof(char *));
    void *value;
    printf("Enter title for a new column: ");
    scanf("\n%[^\n]s", title);
    const char *typelist[] = {"int", "str"};
    printf("Enter number of corresponding type\n");
    for (int i = 0; i < 2; i++)
    {
        printf("%s - %d\n", typelist[i], i);
    }
    int typen;
    while(true)
    {

        scanf("%d", &typen);
        if(typen >= 0 && typen < 2)
            break;
        printf("Incorrect number, please try again: ");
        char cleaner = 'q';
        while(cleaner != '\n')
            scanf("%c", &cleaner);
    }
    type = (char *)typelist[typen];
    printf("Enter default value for this field: ");
    if(typen == 0)
    {
        value = malloc(sizeof(int));
        scanf("%d", (int *)value);
        // value = (void *)input;
    }
    if(typen == 1)
    {
        // char *input = (char *)calloc(str_max_size, sizeof(char *));
        value = calloc(str_max_size, sizeof(char));
        scanf("%s", (char *)value);
        // value = (void *)input;
    }
    add_column(db, title, type, value);
    printf("New column has been successfully added to database\n");
    free(title);
    // free(type); // it points on element of static array
    free(value);
}

//maximum safe method to add an existing entry
void add_entry(table *db, entry *new_entry)
{
    if(db->size == 0)
    {
        db->capacity = 10;
        db->list = (entry *)calloc((db->capacity), sizeof(entry));
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
            new_entry.data[i] = malloc(sizeof(int));
            scanf("%d", (int *)new_entry.data[i]);
            // new_entry.data[i] = (void *)(to_read);
        }
    }
    return new_entry;
}

void add_new_entry(table *db)
{
    entry new_entry = make_new_entry(db);
    add_entry(db, &new_entry);
    destroy_entry(db, &new_entry);
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
                // "One week later"
                // I mastered pointers like a hell and I know that this shit works
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

// the most useless function here
entry get(table *db, int number)
{
    assert(db->size > number);
    assert(number >= 0);
    return (copy_entry(db, &db->list[number]));
}

// compares two entries and if they are equal returns true otherwise false
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

// integer comparator for qsort
int int_comparison(const void *elem1, const void *elem2)
{
    entry f = *((entry *)elem1);
    entry s = *((entry *)elem2);
    return (*(int *)elem1 > *(int *)elem2);
}

// char * comparator for qsort
int str_comparison(const void *elem1, const void *elem2)
{
    entry f = *((entry *)elem1);
    entry s = *((entry *)elem2);
    return strcmp((char *)f.comp_field, (char *)s.comp_field);
}

//sorts by field under given number in table->titles
void sort_by_field(table *db, int field)
{
    for(int i = 0; i < db->size; i++)
    {
        //comparison field is the field used by qsort comparators above
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
    //first line is amount of arguments and number of entries
    FILE *file = fopen(filename, "w");
    fprintf(file, "%d ", db->arg_amt);
    fprintf(file, "%d\n", db->size);

    //next lines contain type and title for each argument field
    for(int i = 0; i < db->arg_amt; i++)
    {
        fprintf(file, "%s %s\n", db->arg_types[i], db->titles[i]);
    }

    //next lines contain values for each field in each entry separated by whitespaces
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

//returns position of first occurrence of equal entry
// and returns database size if there is no equal entries in given database
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

// returns database containing entries with desired value in chosen field only
table get_entries_by_field(table *db, void* field, int type)
{
    table result = get_new_table();
    result.arg_amt = db->arg_amt;
    result.arg_types = (char **)calloc(db->arg_amt, sizeof(char *));
    result.titles = (char **)calloc(db->arg_amt, sizeof(char *));
    for(int i = 0; i < db->arg_amt; i++)
    {
        result.arg_types[i] = make_str_copy(db->arg_types[i]);
        result.titles[i] = make_str_copy(db->titles[i]);
    }

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
                // entry *to_add = (entry *)calloc(1, sizeof(entry));
                // *to_add = copy_entry(db, &db->list[i]);
                add_entry(&result, &db->list[i]);
            }
        }
    }

    return result;
}

//interface for searching entries with desired values
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
    table result = get_entries_by_field(db, value, type);
    free(value);
    return result;
}

//trying to find a given entry in database and rewrite it
void substitute_entry(table *db, entry *before, entry *after)
{
    int pos = find_entry(db, before);
    delete_entry(db, &db->list[pos]);
    if(pos < db->size)
        db->list[pos] = copy_entry(db, after);
}

//determines minimum width for integer fields to print database as spreadsheet
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

//function for pretty print of database on the screen
void print_table(table *db)
{
    int number_width = (1<db->size?get_int_width(db->size):1);
    int *widths = NULL;
    widths = (int *)calloc(db->arg_amt, sizeof(int));

    for(int i = 0; i < db->arg_amt; i++)
    {
        printf("%s, ", db->arg_types[i]);
    }
    printf("\n");
    //counting all widths for fields in table format
    for(int i = 0; i < db->arg_amt; i++) // for each argument
    {
        widths[i] = get_str_width(db->titles[i]);
        for(int j = 0; j < db->size; j++) // in each entry
        {
            if(is_str(db, i))
            {
                if(db->list[j].data[i] == NULL)
                {
                    printf("Alert!");
                }
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
//I hope you are ok after reading so much shitty code
//take care

#endif //LAB_9_DATABASE_H
