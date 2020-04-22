//
// Created by mikhail on 18.04.2020.
//
#ifndef LAB_9_DATABASE_H
#define LAB_9_DATABASE_H

#include <assert.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>

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

//to make new columns in a table (for construction of a new table)
table make_new_column(table *db, char *title, char *type, void* value)
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
    for(int i = 0; i < db->size; i++)
    {
        if(db->arg_amt == 1)
            db->list[i].data = (void**)calloc(db->arg_amt, sizeof(void **));
        else
            db->list[i].data = (void**)realloc(db->list[i].data, db->arg_amt*sizeof(void **));
        db->list[i].data[db->arg_amt - 1] = value;
    }
}

void open_table(table *db, char *filename)
{
    FILE *file = fopen(filename, "r");
    fscanf(file, "%d ", &(db->size));
    fscanf(file, "%d ", &(db->arg_amt));
    db->arg_types = (char **)calloc(db->arg_amt, sizeof(char **));
    db->titles = (char **)calloc(db->arg_amt, sizeof(char **));
    for(int i = 0; i < db->arg_amt; i++)
    {
        fscanf(file, "%s %s ", db->titles[i], db->arg_types[i]);
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
                db->list[i].data[j] = (void *) calloc(32, sizeof(char));
                // I wonder if this trick would work
                // But maybe it is even unnecessary
                fscanf(file, "%s ", (char *) (db->list[i].data[j]));
            }
            if(is_int(db, j))
            {
                //this too
                fscanf(file, "%d ", (int *)(db->list[i].data[j]));
            }
        }
    }
}

entry get(table *db, int number)
{
    assert(db->size > number);
    assert(number >= 0);
    return (db->list[number]);
}

bool equal(table *db, entry first, entry second)
{
    bool ans = true;
    for(int i = 0; i < db->arg_amt; i++)
    {
        if(is_str(db, i))
            ans &= (strcmp((char *)(first.data[i]), (char *)(second.data[i])) == 0);
        if(is_int(db, i))
            ans &= (*(int *)(first.data[i]) == *(int *)(second.data[i]));
    }
    return ans;
}

int greater(const void *elem1, const void *elem2)
{
    //CHECk IF THERE BRACKETS ARE NEEDED
    //ps seems it works without additional brackets, but need to be careful.
    //pps yeah it works because dereference has less sequence than cast
    int f = *(int *)elem1;
    int s = *(int *)elem2;
    return(f > s);
}

int int_comparison(const void *elem1, const void *elem2)
{
    entry f = *((entry *)elem1);
    entry s = *((entry *)elem2);
    return greater(f.comp_field, s.comp_field);
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
    assert(1 == 1);
}

void save_table(table *db, char *filename)
{
    FILE *file = fopen(filename, "w");
    fprintf(file, "%d", db->size);
    fprintf(file, "%d ", db->arg_amt);
    for(int i = 0; i < db->arg_amt; i++)
    {
        fprintf(file, "%s %s ", db->titles[i], db->arg_types[i]);
    }
    // fprintf(file, "\n");
    for(int i = 0; i < db->size; i++)
    {
        fprintf(file, "\n");
        for(int j = 0; j < db->arg_amt; j++)
        {
            fprintf(file, "%s ", db->list[i].data[j]);
        }
    }
}

// need to change this method for more safety
void add_entry(table *db, entry new_entry)
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
    db->list[db->size-1] = new_entry;
}

int find_entry(table *db, entry one)
{
    int number = db->size;
    for(int i = 0; i < db->size; i++)
    {
        if(equal(db, one, db->list[i]))
        {
            number = i;
            break;
        }
    }
    return number;
}

void delete_entry(table *db, entry one)
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
            if(strcmp((char *)(db->list[i].data[type]), (char *)field))
            {
                // values in entry copied by reference!!!
                // need to write copy function to copy by value!!!
                add_entry(&result, db->list[i]);
            }
        }
        if(is_int(db, type))
        {
            if(*(int *)(db->list[i].data[type]) == *(int *)(field))
            {
                add_entry(&result, db->list[i]);
            }
        }
    }
}


void substitute_entry(table *db, entry before, entry after)
{
    int pos = find_entry(db, before);
    if(pos < db->size)
        db->list[pos] = after;
}

int get_int_width(int field)
{
    int res = 0;
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
    int number_width = (1<db->size?db->size:1);
    int *widths = (int *)calloc(db->arg_amt, sizeof(int *));

    //counting all widths for fields in table format
    for(int i = 0; i < db->arg_amt; i++)
    {
        widths[i] = get_str_width(db->titles[i]);
        for(int j = 0; j < db->size; j++)
        {
            if(is_str(db, i))
            {
                int width = get_str_width((char *)db->list[j].data[i]);
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
        printf("||%*s|", number_width, "№");
        entry ent = db->list[i];
        for(int j = 0; j < db->arg_amt; j++)//for each argument in entry
        {
            if(is_str(db, j))
            {
                printf("|%*s|", widths[j], (char *)db->list[i].data[j]);
            }
            if(is_int(db, j))
            {
                printf("|%*d|", widths[j], *(int *)db->list[i].data[j]);
            }
        }
        printf("|\n");
    }
}

#endif //LAB_9_DATABASE_H
