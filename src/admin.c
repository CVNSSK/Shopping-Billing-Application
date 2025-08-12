#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "quickbill.h"


// Review bill from data/ folder
int ReviewBill() {
    int i, j;
    char ch[15];
    printf("Enter Date(DD) : ");
    kb_gets(ch + 3, 3);
    printf("Enter Month : ");
    kb_gets(ch, 3);
    printf("Enter Year : ");
    kb_gets(ch + 6, 5);
    ch[2] = ch[5] = '_';
    printf("Enter the bill no : ");
    char temp[10];
    kb_gets(temp, sizeof(temp));
    j = atoi(temp);
    strcat(ch, ".txt");

    FILE *fptr = open_data_file(ch, "r");  // read bill from data/
    if (fptr == NULL) {
        printf("\nInvalid Date");
        return 0;
    }

    char c;
    char str[256];
    printf("\n");
    while (1) {
        if (fscanf(fptr, "%[^\n]s", str) != 1) break;
        if (strncmp(str, "cmno-", 5) == 0) {
            fgetc(fptr);
            fscanf(fptr, "%d", &i);
            fscanf(fptr, "%[^\n]s", str);
            fgetc(fptr);
            fgetc(fptr);
            if (i == j || j == 0) {
                printf("%d", i);
                printf("%s\n", str);
                while (1) {
                    if (fscanf(fptr, "%[^\n]s", str) != 1) break;
                    printf("%s\n", str);
                    if (str[0] == 'p') {
                        fgetc(fptr);
                        fgetc(fptr);
                        c = fgetc(fptr);
                        if (j != 0 || c == EOF) {
                            fclose(fptr);
                            return j == 0 ? 1 : j;
                        }
                    }
                    c = fgetc(fptr);
                }
            }
        }
        if (fgetc(fptr) == EOF) break;
    }
    fclose(fptr);
    return 0;
}


// Change password or remove biller
int ChangePassword(int mode) {
    struct passwd {
        char user[32];
        char password[32];
    } n, file;

    printf("Enter User Id : ");
    kb_gets(n.user, sizeof(n.user));

    FILE *fptr = open_data_file("password.txt", "r");
    FILE *ptr  = open_data_file("newpwd.txt", "w");
    if (!fptr || !ptr) {
        printf("\nUnable to open password file.\n");
        if (fptr) fclose(fptr);
        if (ptr) fclose(ptr);
        return 0;
    }

    int found = 0;
    char line[128];

    while (fgets(line, sizeof(line), fptr)) {
        // Remove trailing CR/LF
        line[strcspn(line, "\r\n")] = 0;

        char *comma = strchr(line, ',');
        if (!comma) continue; // skip invalid lines
        *comma = '\0';

        strncpy(file.user, line, sizeof(file.user)-1);
        file.user[sizeof(file.user)-1] = '\0';
        strncpy(file.password, comma + 1, sizeof(file.password)-1);
        file.password[sizeof(file.password)-1] = '\0';

        if (mode == 1) {
            // REMOVE BILLER
            if (strcmp(file.user, n.user) == 0) {
                found = 1; // skip this user
                continue;
            }
            fprintf(ptr, "%s,%s\n", file.user, file.password);
        }
        else if (mode == 0) {
            // CHANGE PASSWORD
            if (strcmp(n.user, file.user) == 0 && !found) {
                int count = 0;
                while (1) {
                    char oldpwd[32];
                    printf("\nEnter existing password : ");
                    kb_gets(oldpwd, sizeof(oldpwd));
                    if (strcmp(oldpwd, file.password) == 0) {
                        char newpwd[32];
                        printf("\nEnter new password (8-20 characters only) : ");
                        while (1) {
                            kb_gets(newpwd, sizeof(newpwd));
                            if (strlen(newpwd) >= 8 && strlen(newpwd) <= 20) {
                                if (strcmp(newpwd, oldpwd) != 0) {
                                    strcpy(file.password, newpwd);
                                    break;
                                } else {
                                    printf("\nSimilar to old password\n");
                                }
                            } else {
                                printf("\nPassword length must be 8-20 characters : ");
                            }
                        }
                        found = 1;
                        break;
                    } else if (count < 3) {
                        count++;
                        printf("\nWrong Password Entered.\n");
                    } else {
                        fclose(ptr);
                        fclose(fptr);
                        return 0;
                    }
                }
            }
            fprintf(ptr, "%s,%s\n", file.user, file.password);
        }
    }

    fclose(ptr);
    fclose(fptr);

    if (!found) {
        printf("Id not found\n");
        return 0;
    }
    return 1;
}


// Add new biller
void EditBiller() {
    char username[32], password[32];
    FILE *fptr = open_data_file("password.txt", "a");
    if (fptr == NULL) {
        printf("Unable to open file");
        exit(1);
    }
    printf("\nEnter new biller username: ");
    kb_gets(username, sizeof(username));

    printf("Enter password: ");
    kb_gets(password, sizeof(password));

    fprintf(fptr, "%s,%s\n", username, password);
    fclose(fptr);
}
