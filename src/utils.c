#include "quickbill.h"

#include<string.h>

Product product[MAX_PRODUCTS];


/* Open a file inside the data/ directory */
FILE *open_data_file(const char *name, const char *mode) {
    char path[256];
    snprintf(path, sizeof(path), "data/%s", name);
    FILE *fp = fopen(path, mode);
    return fp;
}


int kb_getch(void) {
#ifdef _WIN32
    return _getch();
#else
    struct termios oldt, newt;
    int ch;
    if (tcgetattr(STDIN_FILENO, &oldt) == -1) return getchar();
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1) return getchar();
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}


void kb_gets(char *buf, size_t bufsz) {
    size_t idx = 0;
    int ch;
    while (1) {
        ch = kb_getch();
        if (ch == '\r' || ch == '\n') { // Enter
            putchar('\n');
            break;
        }
        else if (ch == 8 || ch == 127) { // Backspace
            if (idx > 0) {
                idx--;
                printf("\b \b");
            }
        }
        else if (isprint(ch) && idx < bufsz - 1) {
            buf[idx++] = (char)ch;
            putchar((char)ch);
        }
    }
    buf[idx] = '\0';
}


void trim(char *str) {
    char *start = str;
    char *end;

    // Trim leading spaces
    while (isspace((unsigned char)*start)) {
        start++;
    }

    // If all spaces
    if (*start == 0) {
        *str = 0;
        return;
    }

    // Trim trailing spaces
    end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) {
        end--;
    }

    // Write new null terminator
    *(end + 1) = 0;

    // Shift string to the front if needed
    if (start != str) {
        memmove(str, start, end - start + 2);
    }
}


int login(int *fla, int *id) {
    char username[50], password[50];
    char fileUser[50], filePass[50];
    int found = 0, index = 0;

    FILE *fp = fopen("data/password.txt", "r");
    if (!fp) {
        printf("\nError: password.txt not found.\n");
        return 0;
    }

    printf("\nEnter username: ");
    scanf("%49s", username);

    // Flush leftover newline
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}

    printf("Enter password: ");
    read_password_hidden(password, sizeof(password), 1); // hide with *

    trim(username);
    trim(password);

    while (fscanf(fp, "%49[^,],%49[^\n]\n", fileUser, filePass) == 2) {
        trim(fileUser);
        trim(filePass);

        if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
            if (*fla == 0 && strcmp(fileUser, "admin") == 0) {
                found = 1;
                break;
            }
            if (*fla == 1 && strcmp(fileUser, "admin") != 0) {
                found = 1;
                *id = index;
                break;
            }
        }
        index++;
    }

    fclose(fp);

    if (found) {
        printf("\nLogin successful.\n");
        return 1;
    } else {
        printf("\nInvalid username or password.\n");
        return 0;
    }
}


int read_password_hidden(char *out, size_t outsz, int echo_stars) {
    if (!out || outsz == 0) return 0;
    size_t i = 0;
    for (;;) {
        int c = kb_getch();
        if (c == '\r' || c == '\n') {
            if (echo_stars) putchar('\n');
            break;
        }
        if (c == 127 || c == 8) { /* backspace */
            if (i > 0) {
                i--;
                if (echo_stars) {
                    fputs("\b \b", stdout);
                    fflush(stdout);
                }
            }
            continue;
        }
        if (c == EOF) break;
        if (i + 1 < outsz) {
            out[i++] = (char)c;
            if (echo_stars) {
                putchar('*');
                fflush(stdout);
            }
        }
    }
    out[i] = '\0';
    return (int)i;
}


FILE *addto(void) {
    time_t T = time(NULL);
    struct tm *local = localtime(&T);
    char ch[32];
    /* MM_DD_YYYY.txt */
    snprintf(ch, sizeof(ch), "%02d_%02d_%04d.txt", local->tm_mon + 1, local->tm_mday, local->tm_year + 1900);
    return open_data_file(ch, "a");
}


void print_date(FILE *ptr, char ch) {
    time_t T = time(NULL);
    struct tm *local = localtime(&T);
    if (ptr) fprintf(ptr, "%s\n", ctime(&T));
    if (ch == 'n') {
        printf("Date:%d/%d/%d   Time:%02d:%02d:%02d",
               local->tm_mday, local->tm_mon + 1, local->tm_year + 1900,
               local->tm_hour, local->tm_min, local->tm_sec);
    }
}


void convt(char arr[30], int i, int l) {
    switch (l) {
        case 1: strncpy(product[i].p_Name, arr, sizeof(product[i].p_Name) - 1);
                product[i].p_Name[sizeof(product[i].p_Name)-1] = '\0';
                break;
        case 2: product[i].MRP = strtol(arr, NULL, 10); break;
        case 3: product[i].dis = strtol(arr, NULL, 10); break;
        case 4: product[i].GST = strtol(arr, NULL, 10); break;
        case 6: product[i].Netcost = strtol(arr, NULL, 10); break;
        default: break;
    }
}


void rfile(int cnt) {
    int flag = 0, l = 0, i = 0, t, extcnt = 0;
    char line[256];
    FILE *f = open_data_file("products.csv", "r");
    if (f == NULL) {
        perror("Unable to open product file");
        exit(1);
    }
    while (fgets(line, sizeof(line), f) && extcnt < cnt) {
        char *token = strtok(line, ",");
        if (!token) continue;
        t = atoi(token);
        for (i = 0; i < cnt; i++) {
            if (t == product[i].Code && t != 0) { l = 0; flag = 1; extcnt++; break; }
        }
        if (flag != 1) continue;
        while (token != NULL) {
            if (flag == 1) {
                if ((l > 0 && l < 5) || l == 6) convt(token, i, l);
                l++;
            }
            token = strtok(NULL, ",");
        }
        flag = 0;
    }
    fclose(f);
}


void b_Print(long cmno, int id, int *cnt) {
    int i, j;
    long qnt = *cnt, items, value = 0;
    const int width = 97; // total bill width for centering

    FILE *fptr = addto();
    if (fptr == NULL) {
        perror("Unable to open bill file");
        exit(1);
    }

    fprintf(fptr, "cmno-\n");
    fprintf(fptr, "%ld,%d,", cmno, id);

    // ---- HEADER ----
    printf("\n\n\n\n\n\n\n\n\n\n\n");
    printf("%*s\n", (width + (int)strlen("Connect Retails")) / 2, "Connect Retails");
    printf("%*s\n", (width + (int)strlen("- Quality Is Strength -")) / 2, "- Quality Is Strength -");
    printf("%*s\n", (width + (int)strlen("CIN No: XXXXXXXXXX")) / 2, "CIN No: XXXXXXXXXX");
    printf("%*s\n", (width + (int)strlen("VIP Road, Ongole, AP - 523001")) / 2, "VIP Road, Ongole, AP - 523001");
    printf("%*s\n", (width + (int)strlen("TEL: +91xxxxxx3407")) / 2, "TEL: +91xxxxxx3407");
    printf("%*s\n", (width + (int)strlen("GST No: XXXXXXXXXXXXXXX")) / 2, "GST No: XXXXXXXXXXXXXXX");
    printf("%*s\n", (width + (int)strlen("FSSAI License No: XXXXXXXXXXXXXX")) / 2, "FSSAI License No: XXXXXXXXXXXXXX");
    printf("%*s\n", (width + (int)strlen("TAX Invoice")) / 2, "TAX Invoice");
    printf("---------------------------------------------------------------------------------------------------------\n");

    print_date(fptr, 'n');

    char cmStaffLine[100];
    snprintf(cmStaffLine, sizeof(cmStaffLine), "CM No: %-5ld      Staff Code: %-5d", cmno, id);
    printf("%*s\n", (width + (int)strlen(cmStaffLine)) / 2, cmStaffLine);

    printf("---------------------------------------------------------------------------------------------------------\n");
    printf("%-6s %-20s %8s %12s %12s\n", "Code", "Name", "Qty", "Unit Rate", "Value");
    printf("---------------------------------------------------------------------------------------------------------\n");

    items = qnt;
    for (i = 0; i < *cnt; i++) {
        if (product[i].quantity > 1)
            qnt += product[i].quantity - 1;

        // Check for duplicates
        for (j = i - 1; j >= 0; j--) {
            if (product[i].Code == product[j].Code) {
                product[i].MRP = product[j].MRP;
                strncpy(product[i].p_Name, product[j].p_Name, sizeof(product[i].p_Name) - 1);
                product[i].p_Name[sizeof(product[i].p_Name) - 1] = '\0';
                product[i].dis = product[j].dis;
                product[i].Netcost = product[j].Netcost;
                product[i].GST = product[j].GST;
                items--;
                break;
            }
        }

        value += product[i].Netcost * product[i].quantity;

        printf("%-6d %-20s %8d %12ld.%02ld %12ld.%02ld\n",
               product[i].Code,
               product[i].p_Name,
               product[i].quantity,
               product[i].Netcost / 100, labs(product[i].Netcost % 100),
               (product[i].Netcost * product[i].quantity) / 100,
               labs((product[i].Netcost * product[i].quantity) % 100));
    }

    printf("---------------------------------------------------------------------------------------------------------\n");

    char roundOffLine[100];
    snprintf(roundOffLine, sizeof(roundOffLine), "SALE ROUND OFF AMT: (-) .%02ld", labs(value % 100));
    printf("%*s\n", (width + (int)strlen(roundOffLine)) / 2, roundOffLine);

    printf("---------------------------------------------------------------------------------------------------------\n");

    char totalsLine[100];
    snprintf(totalsLine, sizeof(totalsLine), "Items: %-3ld  Qty: %-3ld  Total: %ld.00",
             (long)items, qnt, value / 100);
    printf("%*s\n", (width + (int)strlen(totalsLine)) / 2, totalsLine);

    printf("---------------------------------------------------------------------------------------------------------\n");
    printf("%*s\n", (width + (int)strlen("Thank you! Visit Again")) / 2, "Thank you! Visit Again");
    printf("---------------------------------------------------------------------------------------------------------\n");

    // ---- SAVE BILL TO FILE ----
    for (i = 0; i < *cnt; i++) {
        fprintf(fptr, "%d-%d-%d-%ld.%02ld-%ld.%02ld-\n",
                i + 1,
                product[i].Code,
                product[i].quantity,
                product[i].Netcost / 100, labs(product[i].Netcost % 100),
                (product[i].Netcost * product[i].quantity) / 100,
                labs((product[i].Netcost * product[i].quantity) % 100));
    }
    fprintf(fptr, "paid amt %ld.00\n\n", value / 100);
    fclose(fptr);
}


void filewrite(int id, int *cnt) {
    long cmno = 0;
    FILE *ptr = open_data_file("cidupdate.txt", "r");

    if (ptr == NULL) {
        // File doesn't exist — start fresh
        cmno = 1000; // starting bill/customer ID
        ptr = open_data_file("cidupdate.txt", "w");
        if (!ptr) {
            perror("cidupdate.txt");
            exit(1);
        }
        fprintf(ptr, "%ld", cmno);
        fclose(ptr);
    } else {
        // File exists — read current number
        int ch = fgetc(ptr);
        while (ch != EOF && isdigit(ch)) {
            cmno = cmno * 10 + (ch - '0');
            ch = fgetc(ptr);
        }
        fclose(ptr);
    }

    // Increment and write back
    ptr = open_data_file("cidupdate.txt", "w");
    if (!ptr) {
        perror("cidupdate.txt");
        exit(1);
    }
    fprintf(ptr, "%ld", cmno + 1);
    fclose(ptr);

    // Continue with billing
    b_Print(cmno, id, cnt);
}

