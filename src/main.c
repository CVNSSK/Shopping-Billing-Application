#include "quickbill.h"

int main() {
    char ch;
    do {
        printf("\n\t\tWelcome to Connect.\n\t\tPress A: for Admin login.\n\t\tPress U: for User login. \n\t\tPress any other key to exit\n");
        ch = getch();
        switch(ch) {
            case 'A':
            case 'a':
                do {
                    printf("\n\n\t\tPress\n\t\t1.Review Bills\n\t\t2.Change Admin Password\n\t\t3.Edit Biller.\n\t\t4.Previous menu.\n\t\t0.Exit\n");
                    ch = getch();
                    switch(ch) {
                        case '1':
                            if(ReviewBill() == 0)
                                printf("\nBill not found\n");
                            else
                                printf("\nBill found.\n");
                            break;
                        case '2':
                            if(ChangePassword(0) == 1) {
                                remove("data/password.txt");
                                rename("data/newpwd.txt", "data/password.txt");
                                printf("Password changed successfully.");
                            } else {
                                printf("Password not changed.");
                            }
                            break;
                        case '3':
                            do {
                                printf("\nPress \n\t1 to add biller.\n\t2 to remove biller\n\t3 Previous Menu.\n");
                                ch = getch();
                                if(ch == '1') {
                                    EditBiller();
                                    printf("\nBiller added successfully\n");
                                } else if(ch == '2') {
                                    ChangePassword(1);
                                    remove("data/password.txt");
                                    rename("data/newpwd.txt", "data/password.txt");
                                    printf("\nBiller removed successfully\n");
                                } else if(ch != '3') {
                                    printf("\nWrong choice entered.\nRe-enter your choice.\n");
                                }
                            } while(ch != '3');
                            break;
                        case '4': break;
                        case '0': return 0;
                        default: printf("\nWrong Choice Entered.\nRe-enter Your Choice.\n");
                    }
                } while(ch != '4');
                break;

            case 'U':
            case 'u':
                do {
                    printf("\n\n\t\tPress\n\t\t1.New bill\n\t\t2.Return product\n\t\t3 Previous menu\n\t\t0.Exit\n");
                    ch = getch();
                    switch(ch) {
                        case '1':
                            if(NewBill() == 0)
                                printf("\nBill terminated.");
                            else
                                printf("\nBill Successful.\n");
                            break;
                        case '2':
                            if(ReturnProduct() == 1)
                                printf("\nSuccessfully returned a product.");
                            else 
                                printf("\nReturn Unsuccessful.");
                            break;
                        case '3': break;
                        case '0': return 0;
                        default: printf("\nWrong Choice Entered.\nPress 1.Re-enter Your Choice.\n0.Exit.\n");
                    }
                } while(ch != '3');
                break;

            case '0': return 0;
            default: printf("\n\tWrong Choice Entered.\n\tRe-enter Your Choice.\n");
        }
    } while(1);
}
