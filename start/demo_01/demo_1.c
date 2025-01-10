#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#define MAX_FIELDS 10
#define MAX_FIELD_LEN 20
#define MAX_RECORDS 100
 
struct field 
{
    char name[MAX_FIELD_LEN];
    char type[MAX_FIELD_LEN];
};
 
struct record 
{
    int id;
    char fields[MAX_FIELDS][MAX_FIELD_LEN];
};
 
struct table 
{
    char name[MAX_FIELD_LEN];
    struct field fields[MAX_FIELDS];
    int num_fields;
    struct record records[MAX_RECORDS];
    int num_records;
};
 
void create_table(struct table *t) 
{
    /* initialize */
    memset(t, 0x00, sizeof(struct table));

    printf("Enter table name: ");
    scanf("%s", t->name);
    printf("Enter number of fields: ");
    scanf("%d", &t->num_fields);
    printf("Enter field names and types:\n");

    for (int i = 0; i < t->num_fields; i++) 
    {
        printf("Field %d name: ", i+1);
        scanf("%s", t->fields[i].name);
        printf("Field %d type: ", i+1);
        scanf("%s", t->fields[i].type);
    }
}
 
void insert_record(struct table *t) 
{
    if (t->num_records >= MAX_RECORDS) 
    {
        printf("Table is full\n");
        return;
    }

    t->records[t->num_records].id = t->num_records + 1;

    printf("Enter field values:\n");
    for (int i = 0; i < t->num_fields; i++) 
    {
        printf("%s: ", t->fields[i].name);
        scanf("%s", t->records[t->num_records].fields[i]);
    }
    t->num_records++;
}
 
void print_table(struct table *t) 
{
    printf("%s:\n\t", t->name);
    for (int i = 0; i < t->num_fields; i++) 
    {
        printf("%s\t", t->fields[i].name);
    }

    printf("\n");
    for (int i = 0; i < t->num_records; i++) 
    {
        printf("%d\t", t->records[i].id);
        for (int j = 0; j < t->num_fields; j++) 
        {
            printf("%s\t", t->records[i].fields[j]);
        }
        printf("\n");
    }
}
 
void update_record(struct table *t) 
{
    int id;

    printf("Enter record id: ");
    scanf("%d", &id);
    if (id < 1 || id > t->num_records) 
    {
        printf("Invalid id\n");
        return;
    }

    printf("Enter field values:\n");
    for (int i = 0; i < t->num_fields; i++) 
    {
        printf("%s: ", t->fields[i].name);
        scanf("%s", t->records[id-1].fields[i]);
    }
}
 
void delete_record(struct table *t) 
{
    int id;
    printf("Enter record id: ");
    scanf("%d", &id);
    if (id < 1 || id > t->num_records) 
    {
        printf("Invalid id\n");
        return;
    }

    for (int i = id-1; i < t->num_records-1; i++) 
    {
        t->records[i] = t->records[i+1];
    }
    t->num_records--;
}
 
int main() 
{
    struct table t;
    create_table(&t);

    while (1) 
    {
        printf("Enter command (insert/select/update/delete/exit): ");
        char command[MAX_FIELD_LEN];
        scanf("%s", command);
        if (strcmp(command, "insert") == 0) 
        {
            insert_record(&t);
        } else if (strcmp(command, "select") == 0) 
        {
            print_table(&t);
        } else if (strcmp(command, "update") == 0) 
        {
            update_record(&t);
        } else if (strcmp(command, "delete") == 0) 
        {
            delete_record(&t);
        } else if (strcmp(command, "exit") == 0) 
        {
            break;
        } else 
        {
            printf("Invalid command\n");
        }
    }
    return 0;
}
