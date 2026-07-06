#define LINEMAXSIZE 10000
#define USERNAMENB 15
#define PASSWORDNB 20
#define MAX_CUSTOMERS 50
#define MAX_RIB_LENGTH 23


//transfer_money_byadvisor ne modifie pas les files :(
typedef struct
{
    char RIB[23];
    int advisorID;
    char username[USERNAMENB];
    char password[PASSWORDNB];
    char date_of_birth[11];
    float net_monthly_salary;
    float total_monthly_loan_payments;
    float current_account_balance;
}Customer;

typedef struct
{
    int RIB;
    int adID;
    int un;
    int pw;
    int bd;
    int nms;
    int tmlp;
    int cab;
}cust_file_pos;

typedef struct {
    int adID;
    int aRIB;
    int aun;
    int apw;
} ad_file_pos;

typedef struct {
    int advisorID;
    char username[USERNAMENB];
    char password[PASSWORDNB];
    char RIB[MAX_CUSTOMERS][23];
}Advisor;

char userInput[3];

