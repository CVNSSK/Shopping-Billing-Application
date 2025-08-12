#include "quickbill.h"


int NewBill(int staff_id) {
    int cnt = 0, d;
    char codeInput[32];

    while (1) {
        if (cnt >= MAX_PRODUCTS) {
            puts("Reached product limit.");
            break;
        }

        printf("\nEnter product code (x to menu): ");
        if (scanf("%31s", codeInput) != 1) {
            puts("Input error.");
            return 0;
        }

        if (codeInput[0] == 'x' || codeInput[0] == 'X') {
            char choice;

            // Clear any leftover newline before reading a single key
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}

            printf("\nPress\n\t1 to add more\n\t2 to delete an item\n\t0 to print bill\n\tx to exit\n");
            choice = kb_getch();

            if (choice == '1') {
                continue; // back to adding
            } else if (choice == '2') {
                if (cnt > 0) {
                    printf("\nCurrent items:\n");
                    for (int i = 0; i < cnt; i++)
                        printf("%d - Code:%d Qty:%d\n", i + 1, product[i].Code, product[i].quantity);

                    printf("Select item number to delete: ");
                    if (scanf("%d", &d) != 1) {
                        puts("Invalid input.");
                        // flush line
                        while ((c = getchar()) != '\n' && c != EOF) {}
                        continue;
                    }
                    if (d >= 1 && d <= cnt) {
                        for (int i = d - 1; i < cnt - 1; i++)
                            product[i] = product[i + 1];
                        cnt--;
                        printf("Item removed.\n");
                    } else {
                        printf("Invalid selection.\n");
                    }
                } else {
                    printf("No items to remove.\n");
                }
            } else if (choice == '0') {
                break; // finish & print
            } else if (choice == 'x' || choice == 'X') {
                return 0; // exit without printing
            } else {
                printf("Wrong choice entered.\n");
            }
            continue;
        }

        // Add product
        char *endp = NULL;
        long code = strtol(codeInput, &endp, 10);
        if (endp == codeInput || *endp != '\0' || code <= 0 || code > INT_MAX) {
            printf("Invalid product code.\n");
            continue;
        }

        product[cnt].Code = (int)code;

        printf("Enter the quantity: ");
        if (scanf("%d", &product[cnt].quantity) != 1 || product[cnt].quantity <= 0) {
            puts("Invalid quantity. Try again.");
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }
        cnt++;
    }

    if (cnt > 0) {
        rfile(cnt);
        filewrite(staff_id, &cnt); // use the logged-in staff id
        return 1;
    } else {
        printf("No items in bill.\n");
        return 0;
    }
}


int ReturnProduct(void) {
    int l = ReviewBill();
    if (l != 0) {
        int n = l, m;
        char str[64];
        FILE *fptr = open_data_file("return.txt", "a+"); // always in data/
        if (!fptr) { perror("return.txt"); return 0; }
        rewind(fptr);
        while (fscanf(fptr, "%d", &m) == 1) {
            if (fscanf(fptr, "%63[^\n]s", str) != 1) break;
            if (n == m) {
                printf("\nReturn not possible.\n");
                fclose(fptr);
                return 0;
            }
            int c = fgetc(fptr);
            if (c == EOF) break;
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
                printf("\nPress Enter to return another product.\n");
                printf("\nPress any other key to continue.\n");
            } while (kb_getch() == '\r' || kb_getch() == '\n');
            printf("\nEnter mobile number.");
            scanf("%63s", str);
            fprintf(fptr, "%s,", str);
            fputc(',', fptr);
            print_date(fptr, 'y');
            fclose(fptr);
            return 1;
        }
        fclose(fptr);
    }
    return 0;
}
