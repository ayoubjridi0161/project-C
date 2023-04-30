
//structures
typedef struct 
{   
    int index;
    char time[250];
} blockheader;

typedef struct 
{   char date[250];
    char beneficiary[250];
    int number;
    int amount;
} transaction;

typedef struct block
{
    char prevHash[64];
    blockheader header;
    transaction T[10];
    char Hash[64];
    struct block *next;

} block;

typedef struct 
{
    block *head;
    block *tail; 
}blockchain;
//structures

//functions prototypes
    void addtransaction(transaction pending[],int *n,blockchain *Blockchain);
    transaction transactionData();
    void addblock(transaction pending[],blockchain *Blockchain);
    void calculate_hash(char m[]);
    blockchain initialiserBlockchain();
    void exportTime(char date[]);
    void serialiseBlock(block b, char m[]);
    void showblock(blockchain Blockchain);
    void save_transactions_to_json(transaction pending[],int n);
    void save_blockchain_to_json(blockchain Blockchain);
    transaction generateTransaction();
    void load_transactions_from_json(transaction pending[],int *n);

//endofprototypes
