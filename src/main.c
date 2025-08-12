#include "quickbill.h"


int main(void) {
    int ch;
    int fla = 0; // 0 → admin, 1 → user
    int id = 0;  // stores logged-in user ID

    do {
        printf("\n\t\tWelcome to Connect.\n");
        printf("\t\tPress A: for Admin login.\n");
        printf("\t\tPress U: for User login.\n");
        printf("\t\tPress any other key to exit\n");

        ch = kb_getch();

        switch (ch) {

            case 'A':
            case 'a': {
                fla = 0; // admin
                if (!login(&fla, &id)) { // ask for admin credentials
                    printf("\nInvalid admin credentials.\n");
                    break; // go back to main menu
                }

                do {
                    printf("\n\n\t\tPress\n");
                    printf("\t\t1. Review Bills\n");
                    printf("\t\t2. Change Admin Password\n");
                    printf("\t\t3. Edit Biller\n");
                    printf("\t\t4. Previous menu\n");
                    printf("\t\t0. Exit\n");

                    ch = kb_getch();

                    switch (ch) {
                        case '1': {
                            if (ReviewBill() == 0)
                                printf("\nBill not found\n");
                            else
                                printf("\nBill found.\n");
                            break;
                        }
                        case '2': {
                            if (ChangePassword(0) == 1) {
                                remove("data/password.txt");
                                rename("data/newpwd.txt", "data/password.txt");
                                printf("Password changed successfully.\n");
                            } else {
                                printf("Password not changed.\n");
                            }
                            break;
                        }
                        case '3': {
                            do {
                                printf("\nPress\n\t1 to add biller.\n\t2 to remove biller\n\t3 Previous Menu.\n");
                                ch = kb_getch();
                                if (ch == '1') {
                                    EditBiller();
                                    printf("\nBiller added successfully\n");
                                }
                                else if (ch == '2') {
                                    if (ChangePassword(1)) {
                                        remove("data/password.txt");
                                        rename("data/newpwd.txt", "data/password.txt");
                                        printf("\nBiller removed successfully\n");
                                    } else {
                                        remove("data/newpwd.txt");
                                        printf("\nBiller not removed\n");
                                    }
                                } else if (ch != '3') {
                                    printf("\nWrong choice entered.\nRe-enter your choice.\n");
                                }
                            } while (ch != '3');
                            break;
                        }
                        case '4':
                            break;
                        case '0':
                            return 0;
                        default:
                            printf("\nWrong Choice Entered.\nRe-enter Your Choice.\n");
                    }
                } while (ch != '4');
                break;
            }

            case 'U':
            case 'u': {
                fla = 1; // user
                if (!login(&fla, &id)) { // ask for user credentials
                    printf("\nInvalid user credentials.\n");
                    break; // back to main menu
                }

                do {
                    printf("\n\n\t\tPress\n");
                    printf("\t\t1. New bill\n");
                    printf("\t\t2. Return product\n");
                    printf("\t\t3. Previous menu\n");
                    printf("\t\t0. Exit\n");

                    ch = kb_getch();

                    switch (ch) {
                        case '1': {
                            if (NewBill(id) == 0) // pass user id to billing
                                printf("\nBill terminated.\n");
                            else
                                printf("\nBill successfully created.\n");
                            break;
                        }
                        case '2': {
                            if (ReturnProduct() == 1)
                                printf("\nSuccessfully returned a product.\n");
                            else
                                printf("\nReturn unsuccessful.\n");
                            break;
                        }
                        case '3':
                            break;
                        case '0':
                            return 0;
                        default:
                            printf("\nWrong Choice Entered.\nRe-enter Your Choice.\n");
                    }
                } while (ch != '3');
                break;
            }

            case '0':
                return 0;

            default:
                printf("\n\tWrong Choice Entered.\n\tRe-enter Your Choice.\n");
        }

    } while (1);
}
