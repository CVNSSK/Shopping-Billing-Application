#include "quickbill.h"

int ReturnProduct() {
    int l = ReviewBill();
    if (l != 0) {
        int n = l, m;
        char str[30];
        FILE *fptr = fopen("data/Return.txt", "a+");
        while (1) {
            fscanf(fptr, "%d", &m);
            fscanf(fptr, "%[^\n]s", str);
            if (n == m) {
                printf("\nReturn not possible.");
                fclose(fptr);
                return 0;
            }
            if (fgetc(fptr) == EOF)
                break;
        }
        printf("\nEnter old bill no.");
        scanf("%d", &n);
        if (l == n) {
            fprintf(fptr, "%d", n);
            fputc(',', fptr);
            do {
                printf("\nEnter the product id.");
                scanf("%d", &n);
                fprintf(fptr, "%d", n);
                fputc(',', fptr);
                printf("\nPress Enter to return another product.");
                printf("\nPress any other key to continue.\n");
            } while (getch() == (char)13);
            printf("\nEnter mobile number.");
            scanf("%s", str);
            fprintf(fptr, "%s,", str);
            fputc(',', fptr);
            print_date(fptr, 'y');
            fclose(fptr);
            return 1;
        }
    }
    return 0;
}

int NewBill() {
    int cnt, id, fla = 0, d;
    char ch = '1';
    do {
        if (login(&fla, &id)) {
            cnt = 0;
            do {
                if (ch == '1') {
                    printf("\nEnter products data and Press any other key to add next product.\nPress x to exit\n");
                    do {
                        printf("\nEnter product code:");
                        scanf("%d", &product[cnt].Code);

                        printf("Enter the quantity:");
                        scanf("%d", &product[cnt].quantity);
                        cnt++;
                        ch = getch();
                    } while (ch != 'x' && ch != 'X');
                }
                printf("Press\n\t 1 to add\n\t 2 to delete\n\t 0 to printbill\n\t x to exit");
                ch = getch();
                if (ch == '1')
                    continue;
                else if (ch == '2') {
                    while (1) {
                        if (cnt > 0) {
                            for (int i = 0; i < cnt; i++)
                                printf("\n%d-%d", i + 1, product[i].Code);
                            printf("\nSelect the item to delete.");
                            scanf("%d", &d);
                            while (d <= cnt) {
                                product[d - 1].Code = product[d].Code;
                                product[d - 1].quantity = product[d].quantity;
                                d += 1;
                            }
                            cnt -= 1;
                            while (1) {
                                printf("Press\n 1 to delete another item\n 2 to goto previous menu.\n");
                                ch = getch();
                                if (ch == '1' || ch == '2')
                                    break;
                            }
                            if (ch == '2')
                                break;
                        } else
                            break;
                    }
                } else if (ch == '0')
                    break;
                else if (ch == 'x')
                    return 0;
                else
                    printf("\nwrong choice entered");
            } while (cnt < MAX_PRODUCTS && ch != '0');
            rfile(cnt);
            filewrite(id, &cnt);
            return 1;
        } else
            printf("\nPress Enter to Retry.\nPress any other key to exit.");
    } while (getch() == (char)13);
    return 0;
}
