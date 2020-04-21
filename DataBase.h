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
    void **data;
    // int flat_number;
    // int flat_type;
    // int rent_size;
    // int rent_info;
    // char *name;
    // char *surname;
    // char *patronymic;
} entry;

typedef struct table_ {
    int arg_amt = 0;
    char **art_types;
    int size = 0;
    int capacity = 0;
    entry *list = NULL;
} table;

void open_table(table *db, char *filename)
{
    FILE *file = fopen(filename, "r");
    fscanf(file, "%d", &(db->size));
    printf("%d\n", db->size);
    db->capacity = db->size;
    db->list = (entry *)calloc(db->capacity, sizeof(entry));
    for(int i = 0; i< db->size; i++)
    {
        fscanf(file, "%d ", &(db->list[i].flat_number));
        fscanf(file, "%d ", &(db->list[i].flat_type));
        fscanf(file, "%d ", &(db->list[i].rent_size));
        fscanf(file, "%d ", &(db->list[i].rent_info));
        db->list[i].name = (char *)calloc(32*sizeof(char), sizeof(char));
        fscanf(file, "%s ", (db->list[i].name));
        db->list[i].surname = (char *)calloc(32*sizeof(char), sizeof(char));
        fscanf(file, "%s ", (db->list[i].surname));
        db->list[i].patronymic = (char *)calloc(32*sizeof(char), sizeof(char));
        fscanf(file, "%s", (db->list[i].patronymic));
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
    ans &= first.flat_number == second.flat_number;
    ans &= first.flat_type == second.flat_type;
    ans &= first.rent_size == second.rent_size;
    ans &= first.rent_info == second.rent_info;
    ans &= first.name == second.name;
    ans &= first.surname == second.surname;
    ans &= first.patronymic == second.patronymic;
    return ans;
}

int greater(const void *elem1, const void *elem2)
{
    //CHECk IF THERE BRACKETS ARE NEEDED
    //p.s. seems it works without additional brackets, but need to be careful.
    int f = *(int *)elem1;
    int s = *(int *)elem2;
    return(f > s);
}


int comp_fn(const void *elem1, const void *elem2)
{
    entry f = *((entry *)elem1);
    entry s = *((entry *)elem2);
    return greater(&(f.flat_number),&(s.flat_number));
}


int comp_ft(const void *elem1, const void *elem2)
{
    entry f = *((entry *)elem1);
    entry s = *((entry *)elem2);
    return greater(&(f.flat_number),&(s.flat_number));
}


int comp_rs(const void *elem1, const void *elem2)
{
    entry f = *((entry *)elem1);
    entry s = *((entry *)elem2);
    return greater(&(f.flat_number),&(s.flat_number));
}


int comp_ri(const void *elem1, const void *elem2)
{
    entry f = *((entry *)elem1);
    entry s = *((entry *)elem2);
    return greater(&(f.flat_number),&(s.flat_number));
}

int comp_name(const void *elem1, const void *elem2)
{
    entry f = *((entry *)elem1);
    f = (*((entry *)elem1));
    entry s = *((entry *)elem2);
    return strcmp(f.name, s.name);
}

int comp_surname(const void *elem1, const void *elem2)
{
    entry f = *((entry *)elem1);
    f = (*((entry *)elem1));
    entry s = *((entry *)elem2);
    return strcmp(f.surname, s.surname);
}

int comp_patronymic(const void *elem1, const void *elem2)
{
    entry f = *((entry *)elem1);
    f = (*((entry *)elem1));
    entry s = *((entry *)elem2);
    return strcmp(f.patronymic, s.patronymic);
}

void sort_by_flat_number(table *db)
{
    qsort(db->list, db->size, sizeof(entry), comp_fn);
}

void sort_by_flat_type(table *db)
{
    qsort(db->list, db->size, sizeof(entry), comp_ft);
}

void sort_by_name(table *db)
{
    qsort(db->list, db->size, sizeof(entry), comp_name);
}

void sort_by_rent_size(table *db)
{
    qsort(db->list, db->size, sizeof(entry), comp_rs);
}

void sort_by_rent_info(table *db)
{
    qsort(db->list, db->size, sizeof(entry), comp_ri);
}

void sort_by_surname(table *db)
{
    qsort(db->list, db->size, sizeof(entry), comp_surname);
}

void sort_by_patronymic(table *db)
{
    qsort(db->list, db->size, sizeof(entry), comp_surname);
}

void save_table(table *db, char *filename)
{
    FILE *file = fopen(filename, "w");
    fprintf(file, "%d", db->size);
    for(int i = 0; i < db->size; i++)
    {
        fprintf(file, "\n");
        fprintf(file, "%d ", db->list[i].flat_number);
        fprintf(file, "%d ", db->list[i].flat_type);
        fprintf(file, "%d ", db->list[i].rent_size);
        fprintf(file, "%d ", db->list[i].rent_info);
        fprintf(file, "%s ", db->list[i].name);
        fprintf(file, "%s ", db->list[i].surname);
        fprintf(file, "%s", db->list[i].patronymic);
    }
}

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
        if(equal(one, db->list[i]))
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

table get_entries_with_flat_number(table *db, int field)
{
    table result;
    result.size = 0;
    result.capacity = 10;
    result.list = (entry *)calloc(result.capacity, sizeof(entry));
    for(int i = 0; i < db->size; i++)
    {
        if(field == db->list[i].flat_number)
            add_entry(&result, db->list[i]);
    }
    return result;
}

table get_entries_with_flat_type(table *db, int field)
{
    table result;
    result.size = 0;
    result.capacity = 10;
    result.list = (entry *)calloc(result.capacity, sizeof(entry));
    for(int i = 0; i < db->size; i++)
    {
        if(field == db->list[i].flat_type)
            add_entry(&result, db->list[i]);
    }
    return result;
}


table get_entries_with_rent_size(table *db, int field)
{
    table result;
    result.size = 0;
    result.capacity = 10;
    result.list = (entry *)calloc(result.capacity, sizeof(entry));
    for(int i = 0; i < db->size; i++)
    {
        if(field == db->list[i].rent_size)
            add_entry(&result, db->list[i]);
    }
    return result;
}

table get_entries_with_rent_info(table *db, int field)
{
    table result;
    result.size = 0;
    result.capacity = 10;
    result.list = (entry *)calloc(result.capacity, sizeof(entry));
    for(int i = 0; i < db->size; i++)
    {
        if(field == db->list[i].rent_info)
            add_entry(&result, db->list[i]);
    }
    return result;
}

table get_entries_with_name(table *db, char *field)
{
    table result;
    result.size = 0;
    result.capacity = 10;
    result.list = (entry *)calloc(result.capacity, sizeof(entry));
    for(int i = 0; i < db->size; i++)
    {
        if(strcmp(field, db->list[i].name) == 0)
            add_entry(&result, db->list[i]);
    }
    return result;
}

table get_entries_with_surname(table *db, char *field)
{
    table result;
    result.size = 0;
    result.capacity = 10;
    result.list = (entry *)calloc(result.capacity, sizeof(entry));
    for(int i = 0; i < db->size; i++)
    {
        if(field == db->list[i].surname)
            add_entry(&result, db->list[i]);
    }
    return result;
}

table get_entries_with_patronymic(table *db, char *field)
{
    table result;
    result.size = 0;
    result.capacity = 10;
    result.list = (entry *)calloc(result.capacity, sizeof(entry));
    for(int i = 0; i < db->size; i++)
    {
        if(field == db->list[i].patronymic)
            add_entry(&result, db->list[i]);
    }
    return result;
}

void substitute_entry(table *db, entry before, entry after)
{
    int pos = find_entry(db, before);
    if(pos < db->size)
        db->list[pos] = after;
}

void print_table(table *db)
{
    int number = 3, fn = 11, ft = 9, rs = 9, ri = 9, nm = 11, sn = 11, pc = 15;
    printf("||  №||%*s||%*s||%*s||%*s||%*s||%*s||%*s||\n", fn, "flat number", ft, "flat type",
            rs, "rent size", ri, "rent info", nm, "name", sn, "surname", pc, "patronymic");
    for(int i = 0; i < db->size; i++)
    {
        entry ent = db->list[i];
        printf("||%*d||%*d||%*d||%*d||%*d||%*s||%*s||%*s||\n", number, i, fn, ent.flat_number,
               ft, ent.flat_type, rs, ent.rent_size, ri, ent.rent_info, nm, ent.name, sn,
               ent.surname, pc, ent.patronymic);
    }
}

#endif //LAB_9_DATABASE_H
