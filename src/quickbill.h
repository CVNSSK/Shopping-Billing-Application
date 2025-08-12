#ifndef QUICKBILL_H
#define QUICKBILL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#ifdef _WIN32
  #include <windows.h>
  #include <conio.h> /* used only if available; kb_getch abstracts it */
#else
  #include <unistd.h>
  #include <termios.h>
#endif

#define MAX_PRODUCTS 100


typedef struct {
    int Code;
    int quantity;
    char p_Name[20];
    long MRP;
    long dis;
    long GST;
    long Netcost;
    long Taxable;
} Product;


extern Product product[MAX_PRODUCTS];


/* cross-platform single-char input without enter */
int kb_getch(void);


/* corss-platform multi-char input wihtout enter */
void kb_gets(char *buf, size_t bufsz);


/* optional masked password input */
int read_password_hidden(char *out, size_t outsz, int echo_stars);


/* open a file from the data/ directory */
FILE *open_data_file(const char *name, const char *mode);


/* utils */
FILE *addto(void);
void print_date(FILE *ptr, char ch);
void convt(char arr[30], int i, int l);
void rfile(int cnt);
void filewrite(int id, int *cnt);
void b_Print(long cmno, int id, int *cnt);


/* admin */
int login(int *fla, int *id);
int ChangePassword(int mode);  /* mode 0 = change; mode 1 = remove biller by id */
void EditBiller(void);
int ReviewBill(void);


/* biller */
int NewBill(int id);
int ReturnProduct(void);


#endif
