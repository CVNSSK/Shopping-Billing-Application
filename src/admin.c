#include "quickbill.h"

int ReviewBill() {
    int i, j;
    char ch[15];
    printf("Enter Date(DD) : ");
    scanf("%2s", ch + 3);
    printf("Enter Month : ");
    scanf("%2s", ch);
    printf("Enter Year : ");
    scanf("%4s", ch + 6);
    ch[2] = ch[5] = '_';
    printf("Enter the bill no : ");
    scanf("%d", &j);
    strcat(ch, ".txt");

    char path[256];
    snprintf(path, sizeof(path), "data/%s", ch);
    FILE *fptr = fopen(path, "r");
    if (fptr == NULL) {
        printf("\nInvalid Date");
        return 0;
    }

    char c;
    char str[31];
    printf("\n");
    while (1) {
        fscanf(fptr, "%[^\n]s", str);
        if (str[0] == 'c' && str[1] == 'm' && str[2] == 'n' && str[3] == 'o' && str[4] == '-') {
            fgetc(fptr);
            fscanf(fptr, "%d", &i);
            fscanf(fptr, "%[^\n]s", str);
            fgetc(fptr);
            fgetc(fptr);
            if (i == j || j == 0) {
                printf("%d", i);
                printf("%s\n", str);
                while (1) {
                    fscanf(fptr, "%[^\n]s", str);
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

        if (fgetc(fptr) == EOF)
            break;
    }
    fclose(fptr);
    return 0;
}

int ChangePassword(int k) {
    int count = 0, flag = 0;
    struct passwd {
        char user[10];
        char password[22];
    } n, file;
    char ch;
    printf("Enter User Id : ");
    scanf("%s", n.user);

    FILE *fptr = fopen("data/password.txt", "r");
    FILE *ptr = fopen("data/newpwd.txt", "w");
    if (fptr == NULL || ptr == NULL) {
        printf("\nUnable to open password file.\n");
        return 0;
    }

    while (fscanf(fptr, "%[^,][^\n]s", file.user) != EOF) {
        if (k == 0 || (k == 1 && strcmp(file.user, n.user) != 0)) {
            fputs(file.user, ptr);
            ch = fgetc(fptr);
            fputc(ch, ptr);
        }
        fscanf(fptr, "%s", file.password);
        if (k == 1 && strcmp(file.user, n.user) == 0)
            fgetc(fptr);
        if (k == 0) {
            if (strcmp(n.user, file.user) == 0 && flag == 0) {
                while (1) {
                    printf("\nEnter existing password : ");
                    scanf("%21s", n.password);
                    if (strcmp(n.password, file.password) == 0) {
                        printf("\nEnter new password(8-20 characters only) : ");
                        while (1) {
                            scanf("%21s", file.password);
                            if (strlen(file.password) <= 20) {
                                if (strcmp(n.password, file.password) != 0)
                                    break;
                                else
                                    printf("\nSimilar to old password\n");
                            } else
                                printf("\nPassword too long\n");
                        }
                        flag = 1;
                        break;
                    } else if (count <= 3) {
                        count++;
                        printf("\nWrong Password Entered.\n");
                    } else {
                        fclose(ptr);
                        fclose(fptr);
                        return 0;
                    }
                }
            }
        }
        if (k == 0 || (k == 1 && strcmp(file.user, n.user) != 0)) {
            fputs(file.password, ptr);
            ch = fgetc(fptr);
            fputc(ch, ptr);
        }
    }
    if (k == 0 && flag == 0) {
        printf("Id not found\n");
        fclose(ptr);
        fclose(fptr);
        return 0;
    }
    fclose(ptr);
    fclose(fptr);
    return 1;
}

void EditBiller() {
    char password[22];
    FILE *fptr = fopen("data/password.txt", "a");
    if (fptr == NULL) {
        printf("Unable to open file");
        exit(1);
    }
    printf("\nEnter new biller:(number)");
    scanf("%s", password);
    fputs(password, fptr);
    fputc(',', fptr);
    printf("Enter password:");
    scanf("%s", password);
    fputs(password, fptr);
    fputc('\n', fptr);
    fclose(fptr);
}
