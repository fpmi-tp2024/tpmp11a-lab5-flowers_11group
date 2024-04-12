#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include <locale.h>
void getInfoOnDate(sqlite3*, sqlite3_stmt*, char*, char*);
int callback(void*, int, char**, char**);
char* getDates();
int increaseFlowerCost(sqlite3*, sqlite3_stmt*, char*, int, double);
void createOrder(sqlite3*, sqlite3_stmt*, char*, int, char*, int, int*, int*, int);