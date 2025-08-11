#include<stdio.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include<conio.h>
#include<math.h>

struct product
{
    int Code;
    int quantity;
    char p_Name[20];
    long int MRP;
    long int dis;
    long int GST;
    long int Netcost;
    long int Taxable;
}product[100];

int login(int* fla,int* id);
void rfile(int cnt);
void b_Print(long int cmno,int id,int *cnt);
void print_date(FILE *ptr,char ch);
void convt(char arr[30],int i,int l);
void filewrite(int id,int *cnt);
FILE* addto();

//admin functions.
int ReviewBill();
int ChangePassword(int n);
void EditBiller();
// void GetCustomerDetails();//not started

//biller functions.
int NewBill();
int ReturnProduct();

int main()
{
	char ch;
	do{
		printf("\n\t\tWelcome to Connect.\n\t\tPress A: for Admin login.\n\t\tPress U: for User login. \n\t\tPress any other key to exit\n");
		ch=getch();
    	switch(ch){
			case 'A':
			case 'a':{	
			do
			{
				printf("\n\n\t\tPress\n\t\t1.Review Bills\n\t\t2.Change Admin Password\n\t\t3.Edit Biller.\n\t\t4.Previous menu.\n\t\t0.Exit\n");
				ch=getch();
				switch(ch){
					case '1':
					{
						if(ReviewBill()==0)
							printf("\nbill not found\n");
						else
							printf("\n bill found.\n");
						break;
					}
					case '2':{
						if(ChangePassword(0)==1)
						{
							remove("password.txt");
							rename("newpwd.txt","password.txt");
							printf("Password changed successfully.");
						}
						else
							printf("Password not changed.");
						break;
					}
					case '3':{
						do{
							printf("\nPress \n\t1 to add biller.\n\t2 to remove biller\n\t3 Previous Menu.\n");
							ch=getch();
							if(ch=='1'){
							EditBiller();
							printf("\nBiller added successfully\n");
							}
							else if(ch=='2'){
							ChangePassword(1);
							remove("password.txt");
							rename("newpwd.txt","password.txt");
							printf("\nBiller removed successfully\n");
							}
							else if(ch!='3')
							printf("\nWrong choice entered.\nRe-enter your choice.\n");
						}while(ch!='3');
						break;
					}
					case '4':break;
					case '0':return 0;
					default:printf("\nWrong Choice Entered.\nRe-enter Your Choice.\n");
				}
			}while(ch!='4');
			break;
		}

		case 'U':
		case 'u':do{
				printf("\n\n\t\tPress\n\t\t1.New bill\n\t\t2.Return product\n\t\t3 Previous menu\n\t\t0.Exit\n");
				ch=getch();
			switch(ch){
				case '1':{
					if(NewBill()==0)
						printf("\nbill terminated.");
					else
						printf("\nBill Successfully.\n");
					break;
				}
				case '2':{
					if(ReturnProduct()==1)
					printf("\nSuccessfull returned a product.");
					else 
					printf("\nReturn Unsuccessful.");
					break;
				}
				case '3':break;
				case '0':
					return 0;
				default:printf("\nWrong Choice Entered.\nPress 1.Re-enter Your Choice.\n0.Exit.\n");
			}
		}while(ch!='3');
		break;
		case '0':
				return 0;
		default:printf("\n\tWrong Choice Entered.\n\tRe-enter Your Choice.\n");
	}
	}while(1);
}

FILE* addto(){
    time_t T;
	struct tm *local;
    time(&T);
    
	local=localtime(&T);
    char ch[14];
    ch[0]=(local->tm_mon+1)/10+'0';
    ch[1]=(local->tm_mon+1)%10+'0';
    ch[2]='_';
    ch[3]=local->tm_mday/10+'0';
    ch[4]=local->tm_mday%10+'0';
    ch[5]='_';
    ch[6]=(local->tm_year+1900)/1000+'0';
    ch[7]=((local->tm_year+1900)/100)%10+'0';
    ch[8]=((local->tm_year+1900)/10)%10+'0';
    ch[9]=(local->tm_year+1900)%10+'0';
    ch[10]='\0';
    strcat(ch,".txt");
    return fopen(ch,"a");
}

void b_Print(long int cmno,int id,int *cnt)
{
    int i,j;
    long int qnt=*cnt,items,value=0;

	FILE *fptr=addto();
	if(fptr==NULL)
	{
		perror("Unable to open file");
		exit(1);
	}
	fprintf(fptr,"cmno-\n");
	fprintf(fptr,"%d,%ld,",cmno,id);
	
    printf("\n\n\n\n\n\n\n\n\n\n\nConnect\n");
    printf("Quality Is Strength\nConnect Retails\nCIN No xxxxxxxxxx\nNBS Ongole\n");
    printf("D NO xx/x/xxxx QIS COLLEGE,\nVengumukkapalem AP\nTEL:+91xxxxxx3407\nGST No:xxxxxxxxxxxxx\nFSSAI Lisence No:xxxxxxxxxxxx\nTAX Invoice\n");
    printf("---------------------------------------------------------------------------------------------------------\n");
    print_date(fptr,'n');
	
    printf("\nCM NO:%ld\t",cmno);
    printf("\tStaff Code:%d",id);
    printf("\n---------------------------------------------------------------------------------------------------------\n");
    printf("Code:\tName:\t\t\tQuantity\t UnitRate\tValue");
    printf("\n---------------------------------------------------------------------------------------------------------");
	items=qnt;
    for(i=0;i<*cnt;i++)
    {
	if(product[i].quantity>1)
	    qnt=qnt+product[i].quantity-1;
	for(j=i-1;j>=0;j--)
	    if(product[i].Code==product[j].Code)
	    {
		product[i].MRP=product[j].MRP;
		strcpy(product[i].p_Name,product[j].p_Name);
		product[i].dis=product[j].dis;
		product[i].Netcost=product[j].Netcost;
		product[i].GST=product[j].GST;
		items=items-1;
		break;
	    }
	value+=product[i].Netcost*product[i].quantity;
	printf("\n%d\t%10s\t\t\t%d\t\t%d.%d\t\t%d.%d",product[i].Code,product[i].p_Name,product[i].quantity,product[i].Netcost/100,product[i].Netcost%100,(product[i].Netcost*product[i].quantity)/100,(product[i].Netcost*product[i].quantity)%100);
	}
	printf("\nSALE ROUND OFF AMT:\t\t(-)    .%d",value%100);
    printf("\n---------------------------------------------------------------------------------------------------------\n");
    printf("Items:%d\t\tqty:%d\t\t\t%d.00\n",items,qnt,value/100);
	printf("\n---------------------------------------------------------------------------------------------------------\n");
    printf("Thankyou");
	for(i=0;i<*cnt;i++){
		fprintf(fptr,"%d-%d-%d-%d.%d-%d.%d-\n",i+1,product[i].Code,product[i].quantity,product[i].Netcost/100,product[i].Netcost%100,(product[i].Netcost*product[i].quantity)/100,(product[i].Netcost*product[i].quantity)%100);
	}
	fprintf(fptr,"paid amt %d.00\n\n",value/100);
	fclose(fptr);
}
void print_date(FILE *ptr,char ch)
{
    time_t T;
	struct tm *local;
    time(&T);
	local=localtime(&T);
	fprintf(ptr,"%s\n", ctime(&T));
	if(ch=='n')
    printf("Date:%d/%d/%d   Time:%d:%d:%d",local->tm_mday,local->tm_mon+1,local->tm_year+1900,local->tm_hour,local->tm_min,local->tm_sec);
}
int login(int* fla,int* id)
{
	struct passwd
	{
		int id;
		char password[22];
	}inp,file;

	int i=0;
	
	printf("\nStaff ID:");
	scanf("%d",&inp.id);
	*id=inp.id;
	FILE *fptr=fopen("password.txt","r");

	if(fptr==NULL){
		perror("Unable to open password file.");
		exit(1);
	}

	char ch='0';
	fscanf(fptr,"%s",file.password);
	while(1){
		fscanf(fptr,"%d",&file.id);
		ch=fgetc(fptr);
		fscanf(fptr,"%s",file.password);
		ch=fgetc(fptr);
		if(ch==EOF||file.id==inp.id)
		break;
	}
	printf("%s",file.password);
	if(file.id!=inp.id){
		fclose(fptr);
		printf("id not found");
		return 0;
	}
	fclose(fptr);
	if(*fla<3)
	{
		printf("Enter password:");
		for(i=0;;i++)//to display "*" instead of character
		{
			inp.password[i]=getch();
			if(inp.password[i]!=(char)13)
			printf("*");
			else
			break;
		}
		inp.password[i]='\0';
		if(strcmp(inp.password,file.password)==0)
			return 1;
		else
		{
			*fla=*fla+1;
			printf("\nInvalid password");
		}
	}
	else
		printf("\nWrong password Entered to many times.");
	return 0;
}
int ReturnProduct(){
	int l=ReviewBill();
	if(l!=0)
	{
		int n=l,m;
		char str[30];
		int i;
		char  c;
		FILE *fptr=fopen("Return.txt","a+");
		while(1){
			fscanf(fptr,"%d",&m);
			fscanf(fptr,"%[^\n]s",str);
			if(n==m){
				printf("\nReturn not possible.");
				fclose(fptr);
				return 0;
			}
			if(fgetc(fptr)==EOF)
				break;
		}
		printf("\nEnter old bill no.");
		scanf("%d",&n);
		if(l==n){
		fprintf(fptr,"%d",n);
		fputc(',',fptr);
		do{
		printf("\nEnter the product id.");
		scanf("%d",&n);
		fprintf(fptr,"%d",n);
		fputc(',',fptr);
		printf("\nPress Enter to return another product.");
		printf("\nPress any other key to continue.\n");
		}while(getch()==(char)13);
		printf("\nEnter mobile number.");
		scanf("%s",str);
		fprintf(fptr,"%s,",str);
		fputc(',',fptr);
		print_date(fptr,'y');
		fclose(fptr);
		return 1;
		}
	}
	return 0;
}

void rfile(int cnt)
{
    int flag=0,l=0,i=0,t,extcnt=0;
    char line[200];
    FILE *f=fopen("mycssv.csv","r");
    if(f==NULL){
	perror("Unable to open bill file.");
	exit(1);
    }
    while(fgets(line,sizeof(line),f)&&extcnt<cnt)
    {
	char *token;
	token = strtok(line,",");
	t=atoi(token);
	for(i=0;i<cnt;i++)
	{
	    if(t==product[i].Code&&t!=0)
	    {
		l=0;
		flag=1;
		extcnt++;
		break;
	    }
	}
	if(flag!=1)
	    continue;
	while(token!=NULL)
	{
	    if(flag==1)
	    {
		if((l>0&&l<5)||l==6)
		    convt(token,i,l);
		l++;
	    }
	    token=strtok(NULL,",");
	}
	flag=0;
    }
    fclose(f);
}

void convt(char arr[30],int i,int l)
{
    switch(l)
    {
        case 1:strcpy(product[i].p_Name,arr);
            break;
        case 2:product[i].MRP=atoi(arr);
            break;
        case 3:product[i].dis=atoi(arr);
            break;
        case 4:product[i].GST=atoi(arr);
	    break;
        case 6:product[i].Netcost=atoi(arr);
            break;
    }
}
int NewBill(){
	int cnt,id,fla=0,d;
	char ch='1';
	do{
		if(login(&fla,&id))
		{
			cnt=0;
	    	do
	    	{
				if(ch=='1'){
				printf("\nEnter products data and Press any other key to add next product.\nPress x to exit\n");
				do{
				printf("\nEnter product code:");
				scanf("%d",&product[cnt].Code);

				printf("Enter the quantity:");
				scanf("%d",&product[cnt].quantity);
				cnt++;
				ch=getch();
				}while(ch!='x'&&ch!='X'); 
				}
				printf("Press\n\t 1 to add\n\t 2 to delete\n\t 0 to printbill\n\t x to exit");
				ch=getch();
				if(ch=='1')
					continue;
				else if(ch=='2'){
					while(1){
						if(cnt>0){
						for(int i=0;i<cnt;i++)
							printf("\n%d-%d",i+1,product[i].Code);
						printf("\nSelect the item to delete.");
						
						scanf("%d",&d);
						while(d<=cnt){
							product[d-1].Code=product[d].Code;
							product[d-1].quantity=product[d].quantity;
							d+=1;
						}
						cnt-=1;
						while(1)
						{
							printf("Press\n 1 to delete another item\n 2 to goto previous menu.\n");
							ch=getch();
							if(ch=='1'||ch=='2')
							break;
						}
						if(ch=='2')
							break;
						}
						else
							break;
					}
				}
				else if(ch=='0')
					break;
				else if(ch=='x')
					return 0;
				else
					printf("\nwrong choice entered");
	    	}while(cnt<100&&ch!='0');
	    	rfile(cnt);
	    	filewrite(id,&cnt);
			return 1;
		}
		else
			printf("\nPress Enter to Retry.\nPress any other key to exit.");
    }while(getch()==(char)13);
	return 0;
}

void filewrite(int id,int *cnt)
{
	FILE *ptr=fopen("cidupdate.txt","r");
	if(ptr==NULL)
	{
        printf("can not open target file\n");
        exit(1);
	}
	long int cmno=0;
	char ch='0';
	ch=fgetc(ptr);
	while(ch!=EOF){
		cmno=cmno*10+ch-'0';
		ch=fgetc(ptr);
	}
	fclose(ptr);
	ptr=fopen("cidupdate.txt","w");
	fprintf(ptr,"%d",cmno+1);
	fclose(ptr);
	b_Print(cmno,id,cnt);
}

int ReviewBill(){
	int i,j;
	char ch[15];
    printf("Enter Date(DD) : ");
    scanf("%2s",ch+3);
    printf("Enter Month : ");
    scanf("%2s",ch);
    printf("Enter Year : ");
    scanf("%4s",ch+6);
    ch[2]=ch[5]='_';
	printf("Enter the bill no : ");
	scanf("%d",&j);
    strcat(ch,".txt");

    FILE *fptr=fopen(ch,"r");
    if(fptr==NULL){
        printf("\nInvalid Date");
		return 0;
    }

    char c;
    char str[31];
    str[31];
     printf("\n");
    while(1){
        fscanf(fptr,"%[^\n]s",str);
        if(str[0]=='c'&&str[1]=='m'&&str[2]=='n'&&str[3]=='o'&&str[4]=='-')
        {   
            fgetc(fptr);
			fscanf(fptr,"%d",&i);
            fscanf(fptr,"%[^\n]s",str);
            fgetc(fptr);
			fgetc(fptr);
			if(i==j||j==0)
			{
				printf("%d",i);
				printf("%s\n",str);
				while(1)
				{
					fscanf(fptr,"%[^\n]s",str);
					printf("%s\n",str);
					if(str[0]=='p'){

						fgetc(fptr);
						fgetc(fptr);
						c=fgetc(fptr);
						if(j!=0||c==EOF){
						fclose(fptr);
						return j==0?1:j;
						}
					}
					c=fgetc(fptr); 
				}
			}
        }
        
        if(fgetc(fptr)==EOF)
			break;
    }
    fclose(fptr);
	return 0;
}
int ChangePassword(int k){
	int count=0,flag=0;
	struct passwd
	{
	char user[10];
	char password[22];
	}n,file;
	char ch;
	printf("Enter User Id : ");
	scanf("%s",n.user);
	FILE *fptr=fopen("password.txt","r");
	FILE *ptr=fopen("newpwd.txt","w");
	if(fptr==NULL||ptr==NULL){
		printf("\nUnable to open password file.\n");
		return 0;
	}
	while(fscanf(fptr,"%[^,][^\n]s",file.user)!=EOF){
		if(k==0||(k==1&&strcmp(file.user,n.user)!=0)){
		fputs(file.user,ptr);
		ch=fgetc(fptr);
		fputc(ch,ptr);
		}
		fscanf(fptr,"%s",file.password);
		if(k==1&&strcmp(file.user,n.user)==0)
		fgetc(fptr);
		if(k==0){
		if(strcmp(n.user,file.user)==0&&flag==0){
			while(1){
				printf("\nEnter existing password : ");
				scanf("%21s",n.password);
				if(strcmp(n.password,file.password)==0){
					printf("\nEnter new password(8-20 characters only) : ");
					while(1){
						scanf("%21s",file.password);
						if(strlen(file.password)<=20)
							if(strcmp(n.password,file.password)!=0)
								break;
							else
								printf("\nSimilar to old password\n");
						else
							printf("\nPassword too long\n");
					}
					flag=1;
					break;
				}
				else if(count<=3){
					count++;
					printf("\nWrong Password Entered.\n");
				}
				else{
					fclose(ptr);
					fclose(fptr);
					return 0;
				}
			}
		}
	}
	if(k==0||(k==1&&strcmp(file.user,n.user)!=0)){
	fputs(file.password,ptr);
	ch=fgetc(fptr);
	fputc(ch,ptr);
	}
	}
	if(k==0&&flag==0){
		printf("Id not found\n");
		fclose(ptr);
		fclose(fptr);
		return 0;
	}
	fclose(ptr);
	fclose(fptr);
	return 1;
}

void EditBiller(){
	char password[22];
	FILE  *fptr=fopen("password.txt","a");
	if(fptr==NULL){
		printf("Unable to open file");
		exit(1);
	}
	printf("\nEnter new biller:(number)");
	scanf("%s",password);
	fputs(password,fptr);
	fputc(',',fptr);
	printf("Enter password:");
	scanf("%s",password);
	fputs(password,fptr);
	fputc('\n',fptr);
	fclose(fptr);
}