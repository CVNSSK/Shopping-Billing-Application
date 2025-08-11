#ifndef QUICKBILL_H
#define QUICKBILL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifdef _WIN32
    #include <conio.h>
#else
    char getch();
#endif

#define MAX_PRODUCTS 100

struct product {
    int Code;
    int quantity;
    char p_Name[20];
    long int MRP;
    long int dis;
    long int GST;
    long int Netcost;
    long int Taxable;
};

extern struct product product[MAX_PRODUCTS];

// Admin
int ReviewBill();
int ChangePassword(int n);
void EditBiller();

// Biller
int NewBill();
int ReturnProduct();

// Utils
int login(int* fla, int* id);
void rfile(int cnt);
void b_Print(long int cmno, int id, int *cnt);
void print_date(FILE *ptr, char ch);
void convt(char arr[30], int i, int l);
void filewrite(int id, int *cnt);
FILE* addto();

#endif
