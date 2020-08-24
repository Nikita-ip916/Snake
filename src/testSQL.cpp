#include "mysql.h"
#include <stdio.h>
#include <stdlib.h>

MYSQL mysql;
MYSQL_RES* res;
MYSQL_ROW row;

void exiterr(int exitcode)
{
    fprintf(stderr, "%s\n", mysql_error(&mysql));
    exit(exitcode);
}

int main()
{
    uint i = 0;

    if (!(mysql_real_connect(
                &mysql,
                "localhost",
                "stud",
                "sergeichnik",
                "snake",
                3306,
                NULL,
                0)))
        exiterr(1);
    if (mysql_select_db(&mysql, "snake"))
        exiterr(2);
    if (mysql_query(&mysql, "SELECT name, birth FROM test_table"))
        exiterr(3);
    if (!(res = mysql_store_result(&mysql)))
        exiterr(4);
    while ((row = mysql_fetch_row(res))) {
        for (i = 0; i < mysql_num_fields(res); i++)
            printf("%s\n", row[i]);
    }
    if (!mysql_eof(res))
        exiterr(5);
    mysql_free_result(res);
    mysql_close(&mysql);
}
