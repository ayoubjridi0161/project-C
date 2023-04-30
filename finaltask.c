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
    if (*n==10)
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
    for(i=0;i<n-1;i++){
    	
        fprintf(fptr,"{\n");        
        
        fprintf(fptr,"\"date\": \"%s\",\n",pending[i].date);
        fprintf(fptr,"\"benificiary\": \"%s\",\n",pending[i].beneficiary);
        fprintf(fptr,"\"number\": %d,\n",pending[i].number);
        fprintf(fptr,"\"amount\": %d\n",pending[i].amount);
        fprintf(fptr,"},\n");
    }
    fprintf(fptr,"{\n");        
        
        fprintf(fptr,"\"date\": \"%s\",\n",pending[n-1].date);
        fprintf(fptr,"\"benificiary\": \"%s\",\n",pending[n-1].beneficiary);
        fprintf(fptr,"\"number\": %d,\n",pending[n-1].number);
        fprintf(fptr,"\"amount\": %d\n",pending[n-1].amount);
        fprintf(fptr,"}\n");
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
void load_blockchain_from_json(blockchain *Blockchain){

    FILE *fptr;
    fptr = fopen("blockchain.json", "r");
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
                    if(strcmp(key,"Hash")==0){
                        strcpy(Blockchain->head->Hash,value);
                    }
                    if(strcmp(key,"index")==0){
                        Blockchain->head->header.index=atoi(value);
                    }
                    if(strcmp(key,"time")==0){
                        strcpy(Blockchain->head->header.time,value);
                    }
                    if(strcmp(key,"amount")==0){
                        Blockchain->head->T[i].amount=atoi(value);
                    }
                    if(strcmp(key,"benificiary")==0){
                        strcpy(Blockchain->head->T[i].beneficiary,value);
                    }
                    if(strcmp(key,"date")==0){
                        strcpy(Blockchain->head->T[i].date,value);
                    }
                    if(strcmp(key,"number")==0){
                        Blockchain->head->T[i].number=atoi(value);
                    }
                    if(strcmp(key,"prevHash")==0){
                        strcpy(Blockchain->head->prevHash,value);
                    }
                }
            }
            i++;
        }
    

    }
    fclose(fptr);
    
}
void deleteTransaction(transaction T[],int *n){
    printf("entrer le numero de transaction");
    int num;
    scanf("%d",&num);
    int i,j;
    for ( i = 0; i < n; i++)
    {
        if (T[i].number==num)
        {
            for ( j = i; j < n-1; j++)
            {
                T[j]=T[j+1];
            }
            *n=n-1;
            i=20;
            
        }
        
    }
    if(i!=20)
    printf("transaction non trouvee");

}
int searchTransaction(blockchain Blockchain,int numero){
    block *p=Blockchain.head;
    while(p!=NULL){
        int i;
        for ( i = 0; i < 10; i++)
        {
            if (p->T[i].number==numero)
            {
                printf("transaction trouvee");
                return 0;
            }
            
        }
        p=p->next;
    }
    printf("transaction non trouvee");
}
int main(){
    blockchain Blockchain = initialiserBlockchain();
    transaction pending[11];
    int n=0;
    int i;
   

    
char choix;
    int x=1;
    while (x==1){
    	printf("veuillez introduire votre choix: \n A - pour ajouter une nouvelle transaction \n C - pour chercher une transaction (dans la blockchain ou dans Pending selon un critere de recherche) \n P - pour afficher tous les blocs / ou transactions dans la blockchain ou dans Pending\n S - pour sauvegarder la blochchain ou Pending dans le fichier appropriÃ©\n D - pour supprimer une transaction de Pending\n Q - pour quitter l'applicatio\n ");
    
    
    do{
        scanf("%c",&choix);
    }while(choix=='a' && choix=='c' && choix=='p' && choix=='s' && choix=='d' && choix=='q' );
    switch (choix){
        case 'a':
            printf("");
			transaction t=generateTransaction();
            if (n==10)
            {
                
                printf("pending est plein");
                addblock(pending,&Blockchain);
                n=0;
            }
            
            pending[n]=t;
            n++;
            printf("\n transaction ajoutée");
            break;
        case 'c':
            printf("\n chercher une transaction ... \n");
            printf("donner le numero de transaction \n");
            int numero;
            scanf("%d",&numero);
            searchTransaction(Blockchain,numero);
            break;
        case 'p':
            printf("\n afficher tous les blocs / ou transactions dans la blockchain ou dans Pending\n");
            showblock(Blockchain);
            break;
        case 's':
            printf(" \nsauvegarder la blochchain ou Pending dans le fichier approprie\n");
            save_blockchain_to_json(Blockchain);
            save_transactions_to_json(pending,n);
            break;
        case 'd':
            printf("\nsupprimer une transaction de Pending \n");
            deleteTransaction(pending,&n);
            break;
        case 'q':
            
            x=0;
            printf("\n application quitte");
            break;
       
    }
    }
  /*  for(i=0;i<=n;i++){
        printf("%s\n",pending[i].date);
        printf("%s\n",pending[i].beneficiary);
        printf("%d\n",pending[i].number);
        printf("%d\n",pending[i].amount);
        printf("--------------------\n");
    }
    */
    
    return 0;
}

