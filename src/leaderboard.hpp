#include "mysql.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LEADERBOARD_HPP
#define LEADERBOARD_HPP

using namespace std;
MYSQL mysql;

void exiterr(int exitcode)
{
    fprintf(stderr, "%s\n", mysql_error(&mysql));
    exit(exitcode);
}

void resultSaving(int Score)
{
    char buf[255];
    string Name, request;
    stringstream strScore;
    strScore << Score;
    cout << "Введите ваше имя: ";
    do {
        cin >> Name;
    } while (Name.size() > 15);
    if (!mysql_init(&mysql))
        exiterr(7);
    if (!(mysql_real_connect(
                &mysql,
                "localhost",
                "nikich",
                "6cRaP9_YE",
                "snake",
                3306,
                NULL,
                0)))
        exiterr(1);
    if (mysql_select_db(&mysql, "snake"))
        exiterr(2);
    request = "INSERT INTO leaderboards(name, score) VALUES ('" + Name + "', "
            + strScore.str() + ");";
    strcpy(buf, request.c_str());
    if (mysql_query(&mysql, buf))
        exiterr(3);
    mysql_close(&mysql);
}

#endif
