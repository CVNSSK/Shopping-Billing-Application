#include "quickbill.h"

#ifdef _WIN32
    // Windows will use conio.h's getch()
#else
    #include <termios.h>
    #include <unistd.h>
    char getch() {
        struct termios oldattr, newattr;
        char ch;
        tcgetattr(STDIN_FILENO, &oldattr);
        newattr = oldattr;
        newattr.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
        return ch;
    }
#endif

struct product product[MAX_PRODUCTS];

FILE* addto() {
    time_t T;
    struct tm *local;
    time(&T);

    local = localtime(&T);
    char ch[14];
    ch[0] = (local->tm_mon + 1) / 10 + '0';
    ch[1] = (local->tm_mon + 1) % 10 + '0';
    ch[2] = '_';
    ch[3] = local->tm_mday / 10 + '0';
    ch[4] = local->tm_mday % 10 + '0';
    ch[5] = '_';
    ch[6] = (local->tm_year + 1900) / 1000 + '0';
    ch[7] = ((local->tm_year + 1900) / 100) % 10 + '0';
    ch[8] = ((local->tm_year + 1900) / 10) % 10 + '0';
    ch[9] = (local->tm_year + 1900) % 10 + '0';
    ch[10] = '\0';
    strcat(ch, ".txt");

    char path[256];
    snprintf(path, sizeof(path), "data/%s", ch);
    return fopen(path, "a");
}

void b_Print(long int cmno, int id, int *cnt) {
    int i, j;
    long int qnt = *cnt, items, value = 0;

    FILE *fptr = addto();
    if (fptr == NULL) {
        perror("Unable to open file");
        exit(1);
    }
    fprintf(fptr, "cmno-\n");
    fprintf(fptr, "%ld,%d,", cmno, id);

    printf("\n\n\n\n\n\n\n\n\n\n\nConnect\n");
    printf("Quality Is Strength\nConnect Retails\nCIN No xxxxxxxxxx\nNBS Ongole\n");
    printf("D NO xx/x/xxxx QIS COLLEGE,\nVengumukkapalem AP\nTEL:+91xxxxxx3407\nGST No:xxxxxxxxxxxxx\nFSSAI Lisence No:xxxxxxxxxxxx\nTAX Invoice\n");
    printf("---------------------------------------------------------------------------------------------------------\n");
    print_date(fptr, 'n');

    printf("\nCM NO:%ld\t", cmno);
    printf("\tStaff Code:%d", id);
    printf("\n---------------------------------------------------------------------------------------------------------\n");
    printf("Code:\tName:\t\t\tQuantity\t UnitRate\tValue");
    printf("\n---------------------------------------------------------------------------------------------------------");
    items = qnt;
    for (i = 0; i < *cnt; i++) {
        if (product[i].quantity > 1)
            qnt = qnt + product[i].quantity - 1;
        for (j = i - 1; j >= 0; j--)
            if (product[i].Code == product[j].Code) {
                product[i].MRP = product[j].MRP;
                strcpy(product[i].p_Name, product[j].p_Name);
                product[i].dis = product[j].dis;
                product[i].Netcost = product[j].Netcost;
                product[i].GST = product[j].GST;
                items = items - 1;
                break;
            }
        value += product[i].Netcost * product[i].quantity;
        printf("\n%d\t%10s\t\t\t%d\t\t%d.%d\t\t%d.%d",
               product[i].Code,
               product[i].p_Name,
               product[i].quantity,
               product[i].Netcost / 100,
               product[i].Netcost % 100,
               (product[i].Netcost * product[i].quantity) / 100,
               (product[i].Netcost * product[i].quantity) % 100);
    }
    printf("\nSALE ROUND OFF AMT:\t\t(-)    .%d", value % 100);
    printf("\n---------------------------------------------------------------------------------------------------------\n");
    printf("Items:%d\t\tqty:%ld\t\t\t%ld.00\n", items, qnt, value / 100);
    printf("\n---------------------------------------------------------------------------------------------------------\n");
    printf("Thankyou");
    for (i = 0; i < *cnt; i++) {
        fprintf(fptr, "%d-%d-%d-%ld.%ld-%ld.%ld-\n", i + 1, product[i].Code, product[i].quantity,
                product[i].Netcost / 100, product[i].Netcost % 100,
                (product[i].Netcost * product[i].quantity) / 100,
                (product[i].Netcost * product[i].quantity) % 100);
    }
    fprintf(fptr, "paid amt %ld.00\n\n", value / 100);
    fclose(fptr);
}

void print_date(FILE *ptr, char ch) {
    time_t T;
    struct tm *local;
    time(&T);
    local = localtime(&T);
    fprintf(ptr, "%s\n", ctime(&T));
    if (ch == 'n')
        printf("Date:%d/%d/%d   Time:%d:%d:%d",
               local->tm_mday, local->tm_mon + 1, local->tm_year + 1900,
               local->tm_hour, local->tm_min, local->tm_sec);
}

int login(int* fla, int* id) {
    struct passwd {
        int id;
        char password[22];
    } inp, file;

    int i = 0;

    printf("\nStaff ID:");
    scanf("%d", &inp.id);
    *id = inp.id;

    FILE *fptr = fopen("data/password.txt", "r");
    if (fptr == NULL) {
        perror("Unable to open password file.");
        exit(1);
    }

    char ch = '0';
    fscanf(fptr, "%s", file.password);
    while (1) {
        fscanf(fptr, "%d", &file.id);
        ch = fgetc(fptr);
        fscanf(fptr, "%s", file.password);
        ch = fgetc(fptr);
        if (ch == EOF || file.id == inp.id)
            break;
    }
    if (file.id != inp.id) {
        fclose(fptr);
        printf("id not found");
        return 0;
    }
    fclose(fptr);

    if (*fla < 3) {
        printf("Enter password:");
        for (i = 0;; i++) {
            inp.password[i] = getch();
            if (inp.password[i] != (char)13)
                printf("*");
            else
                break;
        }
        inp.password[i] = '\0';
        if (strcmp(inp.password, file.password) == 0)
            return 1;
        else {
            (*fla)++;
            printf("\nInvalid password");
        }
    } else
        printf("\nWrong password Entered too many times.");
    return 0;
}

void rfile(int cnt) {
    int flag = 0, l = 0, i = 0, t, extcnt = 0;
    char line[200];
    FILE *f = fopen("data/mycssv.csv", "r");
    if (f == NULL) {
        perror("Unable to open bill file.");
        exit(1);
    }
    while (fgets(line, sizeof(line), f) && extcnt < cnt) {
        char *token;
        token = strtok(line, ",");
        t = atoi(token);
        for (i = 0; i < cnt; i++) {
            if (t == product[i].Code && t != 0) {
                l = 0;
                flag = 1;
                extcnt++;
                break;
            }
        }
        if (flag != 1)
            continue;
        while (token != NULL) {
            if (flag == 1) {
                if ((l > 0 && l < 5) || l == 6)
                    convt(token, i, l);
                l++;
            }
            token = strtok(NULL, ",");
        }
        flag = 0;
    }
    fclose(f);
}

void convt(char arr[30], int i, int l) {
    switch (l) {
        case 1: strcpy(product[i].p_Name, arr); break;
        case 2: product[i].MRP = atoi(arr); break;
        case 3: product[i].dis = atoi(arr); break;
        case 4: product[i].GST = atoi(arr); break;
        case 6: product[i].Netcost = atoi(arr); break;
    }
}

void filewrite(int id, int *cnt) {
    FILE *ptr = fopen("data/cidupdate.txt", "r");
    if (ptr == NULL) {
        printf("Cannot open target file\n");
        exit(1);
    }
    long int cmno = 0;
    char ch = '0';
    ch = fgetc(ptr);
    while (ch != EOF) {
        cmno = cmno * 10 + ch - '0';
        ch = fgetc(ptr);
    }
    fclose(ptr);
    ptr = fopen("data/cidupdate.txt", "w");
    fprintf(ptr, "%ld", cmno + 1);
    fclose(ptr);
    b_Print(cmno, id, cnt);
}
