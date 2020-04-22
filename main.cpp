#include <iostream>
#include <cstring>
#include "DataBase.h"

const char *path = "/home/mikhail/Garage/C/lab_9/";

char *get_full_path(char *filename)
{
    char *ret = (char *)calloc(strlen(path) + strlen(filename), sizeof(char));
    strcat(ret, path);
    strcat(ret, filename);
    return ret;
}

void tutorial()
{
    printf("Please, choose one option of the following:\na - add a new entry\n"
           "d - delete entry from database\ng - get entries with matching field\n"
           "r - rewrite an entry\ns - sort database by field\np - print database\n"
           "h - help\nq - quit\n");
}

int main() {
    char key = 'h';
    table db;
    char *filename = "database.txt";
    // open_table(&db, get_full_path(filename));
    db = get_new_table();
    print_table(&db);
    // while(key != 'q')
    // {
    //     switch(key)
    //     {
    //         case 'h':
    //             tutorial();
    //             break;

    //         case 'a': //add
    //             break;

    //         case 'd': //delete
    //             break;

    //         case 'g': //get
    //             break;

    //         case 'r': //rewrite
    //             break;

    //         case 's': //sort
    //             break;

    //         case 'p': //print
    //             print_table(&db);
    //             break;
    //     }
    //     key = getchar();
    // }

    // table a;
    // open_table(&a, get_full_path(filename));
    // int fn = 11, ft = 9, rs = 9, ri = 9, nm = 11, sn = 11, pc = 15;
    // entry addition;
    // addition.flat_number = 1;
    // addition.flat_type = 1;
    // addition.rent_size = 228;
    // addition.rent_info = -1337;
    // addition.name = "jop03"; //(char *)calloc(32*sizeof(char), sizeof(char));
    // addition.surname = "jopov"; //(char *)calloc(32*sizeof(char), sizeof(char));
    // addition.patronymic = "jopovich"; //(char *)calloc(32*sizeof(char), sizeof(char));
    // add_entry(&a, addition);
    // // add_entry(&a, addition);
    // // add_entry(&a, addition);
    // table b = get_entries_with_flat_number(&a, 1);
    // print_table(&b);
    // printf("size:%d, capacity:%d\n", a.size, a.capacity);
    // sort_by_flat_number(&a);
    // // print_table(&a);
    // save_table(&a, get_full_path(filename));
    return 0;
}
