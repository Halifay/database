#include <iostream>
#include <cstring>
#include "DataBase.h"

const char *path = "/home/mikhail/Garage/C/lab_9/";

char *get_full_path(char *filename)
{
    char *ret = (char *)calloc(strlen(path) + strlen(filename) + 1, sizeof(char));
    strcat(ret, path);
    strcat(ret, filename);
    return ret;
}

void table_menu(table *db)
{
    char *key = (char *)calloc(str_max_size, sizeof(char *));
    key[0] = 'h';
    char tutorial[] = "Please, choose one option of the following:\na - add a new entry\n"
                      "c - make a new column for all entries\nd - delete entry from database\n"
                      "g - get entries with matching field\nr - rewrite an entry\n"
                      "s - sort database by field\np - print database\n"
                      "w - write database down to file\nh - help\nq - quit\n";
    while (key[0] != 'q') {
        switch (key[0]) {
            case 'h': // help
            {
                printf("%s", tutorial);
                break;
            }

            case 'a': // add a new entry
            {
                add_new_entry(db);
                printf("Entry added successfully\n");
                break;
            }

            case 'c': // create a column in database
            {
                add_new_column(db);
                printf("New column created successfully\n");
                break;
            }

            case 'd': // delete an entry
            {
                printf("Enter values for entry to delete\n");
                entry slated = make_new_entry(db);
                int pos = find_entry(db, slated);
                if(pos == db->size)
                    printf("There is no such entry\n");
                else
                {
                    delete_entry(db, slated);
                    printf("Entry has been deleted successfully\n");
                }
                break;
            }

            case 'g': // get entries
            {

                table findings = get_entries(db);
                print_table(&findings);
                break;
            }

            case 'r': // rewrite entry
            {
                break;
            }

            case 's': // sort table
            {
                break;
            }

            case 'w': // write table down to file
            {
                printf("Enter name of file to save database: ");
                char *name = (char *)calloc(str_max_size, sizeof(char *));
                scanf("%s", name);
                char* fullname = get_full_path(name);
                save_table(db, fullname);
                printf("Table has been saved in %s", fullname);
                break;
            }

            case 'p': // print
            {
                print_table(db);
                break;
            }

            default:
            {
                printf("No such option '%c'\n", key[0]);
                break;
            }
        }

        scanf("%s", key);
    }
}

void main_menu()
{
    char *key = (char *)calloc(str_max_size, sizeof(char *));
    key[0] = 'h';
    printf("Hey, bro, nice dick. What do you want?\n");
    char tutorial[] = "o = open existing file as database\nc - create a new database\n"
                      "h - get help\nq - quit program\n";
    table db;

    while(key[0] != 'q') {
        switch (key[0]){
            case 'h': //help
            {
                printf("%s", tutorial);
                break;
            }

            case 'o': //open a file
            {
                printf("Type name of file to open: ");
                char *filename = (char *)calloc(str_max_size, sizeof(char *));
                scanf("%s", filename);
                open_table(&db, get_full_path(filename));
                table_menu(&db);
                break;
            }

            case 'c': // create a new database
            {
                db = get_new_table();
                table_menu(&db);
                break;
            }

            default:
            {
                printf("No such option '%c'\n", key[0]);
            }
        }
        scanf("%s", key);
        // key = getchar();
    }
}

int main() {
    main_menu();
    return 0;
}
