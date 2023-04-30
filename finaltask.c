#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "finaltask.h"
void exportTime(char date[]){
time_t currentTime;
time(&currentTime); // Get the current time
strcpy(date,ctime(&currentTime));
strtok(date,"\n");
}
transaction transactionData(){
    transaction t;
    printf("entrer le numero de transaction: ");
    scanf("%d",&t.number);
    exportTime(t.date);
    printf("nom de beneficiaire: ");
    scanf("%s",t.beneficiary);
    printf("montant: ");
    scanf("%d",&t.amount);
    return t;
}
blockchain initialiserBlockchain(){
    blockchain* Blockchain = (blockchain *)malloc(sizeof(blockchain));
    Blockchain->head = NULL;
    Blockchain->tail = NULL;
    return *Blockchain;
}
void calculate_hash(char m[]) {
    int counter;
    int hashAddress = 0;
    for (counter = 0; m[counter] != '\0'; counter++) {
        hashAddress = m[counter] + (hashAddress << 6) + (hashAddress << 16) - hashAddress;
    }
    sprintf(m, "%d", hashAddress);
}
void addblock(transaction pending[],blockchain *Blockchain){
	int i;
    block *newblock = (block* )malloc(sizeof(block));
    newblock->next = NULL;
    exportTime(newblock->header.time);
    for(i=0;i<10;i++){
    	newblock->T[i]=pending[i];
	}
    if (Blockchain->head == NULL) {
        newblock->header.index = 1;
        strcpy(newblock->prevHash,""); 
	    serialiseBlock(*newblock,newblock->Hash);
		calculate_hash(newblock->Hash);
        Blockchain->head = newblock;
        Blockchain->tail = newblock;
    }
    else {
        strcpy(newblock->prevHash , Blockchain->tail->Hash);
        newblock->header.index = Blockchain->tail->header.index + 1;
	    serialiseBlock(*newblock,newblock->Hash);
		calculate_hash(newblock->Hash);
    	Blockchain->tail->next = newblock;
        Blockchain->tail = newblock;
    }
    }
void addtransaction(transaction pending[],int *n,blockchain *Blockchain){
    if (*n==9)
    {
        addblock(pending,Blockchain);
        *n=0;
    }
    pending[*n]=transactionData();
	*n++;
}
void serialiseBlock(block b, char m[]){
    sprintf(m, "%d%s%s", b.header.index, b.header.time, b.prevHash);
    int i;
	for(i=0;i<10;i++){
        sprintf(m,"%d%d%s",b.T[i].amount,b.T[i].number,b.T[i].beneficiary);
    }
}
void showblock(blockchain Blockchain){
        block *p=Blockchain.head;
    	
    	while(p!=NULL){
    		printf("{Hash: %s,\n",p->Hash);
    		printf("index: %d,\n",p->header.index);
    		printf("time: %s\n",p->header.time);
    		
    		int i;
			for(i=0;i<10;i++){
			printf("transaction %d :{ amount :%d,\n", i+1, p->T[i].amount);
			printf(" beneficiary: %s \n",p->T[i].beneficiary);
			printf("date: %s \n,",p->T[i].date);
			printf("number: %d }\n",p->T[i].number);
			}
			printf("prevHash:%s ,\n",p->prevHash);
		p=p->next;
			
		}
}
void save_transactions_to_json(transaction pending[],int n){
    FILE *fptr;
    fptr = fopen("pending.json", "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }

    fprintf(fptr,"[\n");
    int i,j;
    for(i=0;i<n;i++){
        fprintf(fptr,"{\n");        
        
        fprintf(fptr,"\"date\": \"%s\",\n",pending[i].date);
        fprintf(fptr,"\"benificiary\": \"%s\",\n",pending[i].beneficiary);
        fprintf(fptr,"\"number\": %d,\n",pending[i].number);
        fprintf(fptr,"\"amount\": %d\n",pending[i].amount);
        fprintf(fptr,"},\n");
    }
    fprintf(fptr, "]\n");
    fclose(fptr);
}
void save_blockchain_to_json(blockchain Blockchain){
    FILE *fptr;
    fptr = fopen("blockchain.json", "w");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    fprintf(fptr,"[\n");
    block *p=Blockchain.head;
    while(p!=NULL){
        fprintf(fptr,"{\n");
        fprintf(fptr,"\"Hash\": \"%s\",\n",p->Hash);
        fprintf(fptr,"\"index\": %d,\n",p->header.index);
        fprintf(fptr,"\"time\": \"%s\",\n",p->header.time);
        int i;
        for(i=0;i<10;i++){
            fprintf(fptr,"\"transaction %d\": {\n", i+1);
            fprintf(fptr,"\"amount\": %d,\n",p->T[i].amount);
            fprintf(fptr,"\"benificiary\": \"%s\",\n",p->T[i].beneficiary);
            fprintf(fptr,"\"date\": \"%s\",\n",p->T[i].date);
            fprintf(fptr,"\"number\": %d\n",p->T[i].number);
            fprintf(fptr,"},\n");
        }
        fprintf(fptr,"\"prevHash\": \"%s\"\n",p->prevHash);
        fprintf(fptr,"},\n");
        p=p->next;
    }
    fprintf(fptr,"]\n");
    fclose(fptr);

}
transaction generateTransaction(){
    transaction t;
    t.number=rand() % 1000;
    exportTime(t.date);
    strcpy(t.beneficiary,"ayoubjridi");
    t.amount=rand() % 10000;
    return t;
}
void load_transactions_from_json(transaction pending[],int *n){
    FILE *fptr;
    fptr = fopen("pending.json", "r");
    if (fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    char c;
    int i=0;
    while((c = fgetc(fptr)) != EOF){
        if(c=='{'){
            char key[20];
            char value[20];
            int j=0;
            while((c = fgetc(fptr)) != '}'){
                if(c=='\"'){
                    j=0;
                    while((c = fgetc(fptr)) != '\"'){
                        key[j]=c;
                        j++;
                    }
                    key[j]='\0';
                    j=0;
                    while((c = fgetc(fptr)) != '\"'){
                        value[j]=c;
                        j++;
                    }
                    value[j]='\0';
                    if(strcmp(key,"date")==0){
                        strcpy(pending[i].date,value);
                    }
                    if(strcmp(key,"benificiary")==0){
                        strcpy(pending[i].beneficiary,value);
                    }
                    if(strcmp(key,"number")==0){
                        pending[i].number=atoi(value);
                    }
                    if(strcmp(key,"amount")==0){
                        pending[i].amount=atoi(value);
                    }
                }
            }
            i++;
        }
    

    }
    *n=i;
    fclose(fptr);
    
}

int main(){
    blockchain Blockchain = initialiserBlockchain();
    transaction pending[10];
    int n=0;
    int i;
    /*for(i=0;i<10;i++){
        pending[i]=generateTransaction();

        n++;
    }*/

    load_transactions_from_json(pending,&n);
    for(i=0;i<=n;i++){
        printf("%s\n",pending[i].date);
        printf("%s\n",pending[i].beneficiary);
        printf("%d\n",pending[i].number);
        printf("%d\n",pending[i].amount);
        printf("--------------------\n");
    }
    
    
    return 0;
}

