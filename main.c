#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers.h" //include all hearders from headers file

Customer check_customer_details(char username[],cust_file_pos *pos,int *error);
Customer customer_login(int *error,cust_file_pos *pos);
void customer_action(Customer customer, Advisor advisor, cust_file_pos *pos);
void modify_customer_file(int position ,char text[], int last_token);
Customer update_customer_data(Customer customer,cust_file_pos *pos);
Customer withdraw(Customer customer, cust_file_pos *pos, float amount);
Customer deposit(Customer customer, cust_file_pos *pos, float amount);
void transfer_money(Customer customer, cust_file_pos *pos);
Customer loan_eligibility(Customer customer, cust_file_pos *pos);

void send_message(Customer customer, Advisor advisor);

Advisor advisor_login(int *error, ad_file_pos *pos);
void advisor_action(Advisor advisor, ad_file_pos *pos);
Advisor check_advisor_details(char username[], ad_file_pos *pos, int *error);
void create_customer(Advisor advisor);
void delete_customer(Advisor advisor);

void clear_buffer() { //allow to clear the input field
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void end_function() { //ask the user before exiting a menu before the terminal clearing
    do {
        printf("Enter 'ok' to close this menu: ");
        fgets(userInput, sizeof(userInput), stdin);
        userInput[strcspn(userInput, "\n")] = '\0'; // Remove the newline character
    } while (strcmp(userInput, "ok") != 0);
    system("cls");
}

Advisor read_advisor_from_file(int aID) {
    FILE *file = fopen("Advisor_file.txt", "r");
    if (!file) {
        perror("Failed to open file");
        return (Advisor){0}; // Return an empty Advisor on error
    }

    Advisor advisor = {0};
    char line[LINEMAXSIZE];
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        int current_advisorID = atoi(token);
        if (current_advisorID == aID) {
            advisor.advisorID = current_advisorID;

            token = strtok(NULL, ",");
            if (token) strcpy(advisor.username, token);

            token = strtok(NULL, ",");
            if (token) strcpy(advisor.password, token);

            int i = 0;
            while ((token = strtok(NULL, ",")) && i < 23) {
                strcpy(advisor.RIB[i++], token);
            }

            found = 1;
            break;
        }
    }

    fclose(file);
    if (!found) {
        printf("Advisor with ID %d not found.\n", aID);
        return (Advisor){0}; // Return an empty Advisor if not found
    }

    return advisor;
}


Customer customer_login(int *error, cust_file_pos* pos)
{
    char username[USERNAMENB];
    char password[PASSWORDNB];
    int found = 0;
    Customer customer;

    while (1) {
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        printf("Enter your username (write 'stop' to stop): ");
        fgets(username, USERNAMENB, stdin);
        username[strcspn(username, "\n")] = '\0';

        if (strcmp(username, "stop") == 0) {
            *error = 1;
            break;
        }

        printf("Enter password: ");
        fgets(password, PASSWORDNB, stdin);
        password[strcspn(password, "\n")] = '\0';

        customer = check_customer_details(username, pos, error);

        // Check if the username exists and the password matches
        if (*error == 0 && strcmp(customer.password, password) == 0) {
            found = 1;
            break;
        } else {
            printf("\nWrong password or username, please retry\n");
        }
    }

    if (!found) {
        memset(&customer, 0, sizeof(customer));
    }

    return customer;
}

Advisor advisor_login(int *error, ad_file_pos *pos) {
    Advisor advisor;
    char username[USERNAMENB];
    char password[PASSWORDNB];
    int found = 0;

    while (1) {
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        printf("\n-Advisor login-\nEnter your username (write 'stop' to stop): ");
        fgets(username, USERNAMENB, stdin);
        username[strcspn(username, "\n")] = '\0';

        if (strcmp(username, "stop") == 0) {
            *error = 1;
            break;
        }

        printf("Enter password: ");
        fgets(password, PASSWORDNB, stdin);
        password[strcspn(password, "\n")] = '\0';

        advisor = check_advisor_details(username, pos, error);

        // Check if the username exists and the password matches
        if (*error == 0 && strcmp(advisor.password, password) == 0) {
            found = 1;
            break;
        } else {
            printf("\nWrong password or username, please retry\n");
            break;
        }
    }

    if (!found) {
        memset(&advisor, 0, sizeof(advisor));
    }

    return advisor;
}


Customer check_customer_details(char username[], cust_file_pos *pos, int *error)
{
    FILE *customerFile;
    customerFile = fopen("Customer_file.txt", "r");
    if (customerFile == NULL)
    {
        printf("Failed to open file.\n");
        exit(1);
    }

    Customer customer;
    char line[LINEMAXSIZE];
    char *token;
    int found = 0;
    int ligne_pos = 0;
    *error = 0;
    int len_line = 0;
    char str_tmlp[50];
    char str_nms[50];

    printf("1");
    while (fgets(line, LINEMAXSIZE, customerFile) != NULL)
    {
        len_line=strlen(line);
        token = strtok(line, ",");

        // Parse RIB
        if (token != NULL)
            strcpy(customer.RIB, token);

        // Parse advisorID
        token = strtok(NULL, ",");
        if (token != NULL)
            customer.advisorID = atoi(token+1);

        // Parse username
        token = strtok(NULL, ",");
        if (token != NULL)
            strcpy(customer.username, token+1);

        // Parse password
        token = strtok(NULL, ",");
        if (token != NULL)
            strcpy(customer.password, token+1);

        // Parse date_of_birth
        token = strtok(NULL, ",");
        if (token != NULL)
            strcpy(customer.date_of_birth, token+1);

        // Parse net_monthly_salary
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            customer.net_monthly_salary = atof(token+1);
            strcpy(str_nms, token+1);
        }

        // Parse total_monthly_loan_payments
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            customer.total_monthly_loan_payments = atof(token+1);
            strcpy(str_tmlp, token+1);
        }

        // Parse current_account_balance
        token = strtok(NULL, ",");
        if (token != NULL)
            customer.current_account_balance = atof(token+1);

        if (strcmp(customer.username, username) == 0)
        {
            found = 1;
            break;
        }
        ligne_pos += len_line;

    }

    if (!found)
    {
        *error = 1;
        memset(&customer, 0, sizeof(customer));
    }

    else
    {
    //define data position
        char buffer[500];
        pos->RIB = ligne_pos;
        pos->adID = pos->RIB + strlen(customer.RIB) + 2; // Adding 2 for ", "
        pos->un = pos->adID + snprintf(buffer, 6, "%d", customer.advisorID) + 2;
        pos->pw = pos->un + strlen(customer.username) + 2;
        pos->bd = pos->pw + strlen(customer.password) + 2;
        pos->nms = pos->bd + strlen(customer.date_of_birth) + 2;
        pos->tmlp = pos->nms + strlen(str_nms) +2;
        pos->cab = pos->tmlp + strlen(str_tmlp) + 2;
    }
    printf("3");
    fclose(customerFile);
    return customer;
}

Advisor check_advisor_details(char username[], ad_file_pos *pos, int *error) {
    FILE *advisorFile = fopen("Advisor_file.txt", "r");
    if (!advisorFile) {
        perror("Failed to open file");
        *error = 1;
        return (Advisor){0}; // Return an empty Advisor on error
    }

    Advisor advisor = {0};
    char line[LINEMAXSIZE];
    int found = 0;
    int line_pos = 0;
    int current_pos = 0;

    while (fgets(line, sizeof(line), advisorFile)) {
        current_pos = line_pos; // Save the start position of the current line

        char *token = strtok(line, ",");
        if (token) advisor.advisorID = atoi(token);

        token = strtok(NULL, ",");
        if (token) strcpy(advisor.username, token);

        token = strtok(NULL, ",");
        if (token) strcpy(advisor.password, token);

        int ribIndex = 0;
        while ((token = strtok(NULL, ",")) && ribIndex < 23) {
            strcpy(advisor.RIB[ribIndex++], token);
        }

        if (strcmp(advisor.username, username) == 0) {
            found = 1;
            break;
        }

        line_pos += strlen(line);
    }

    if (!found) {
        *error = 1;
        fclose(advisorFile);
        return (Advisor){0}; // Return an empty Advisor if not found
    }

    // Set positions in the ad_file_pos struct
    pos->adID = current_pos;
    pos->aun = current_pos + sizeof(advisor.advisorID) + 1; // position after ID and comma
    pos->apw = pos->aun + strlen(advisor.username) + 1;     // position after username and comma
    pos->aRIB = pos->apw + strlen(advisor.password) + 1;    // position after password and comma

    fclose(advisorFile);
    *error = 0; // No error
    return advisor;
}


void modify_customer_file( int position, char text[],int last_token)//last_token variable to provide last token error
{
    FILE* customerfile;
    FILE* tempfile;
    int buffer;


    customerfile = fopen ("Customer_file.txt", "r");
    tempfile = fopen("temporary_file.txt", "w");
    if (customerfile == NULL || tempfile == NULL) {
        printf("Failed to open file(s).\n");
        exit(1);
    }
    // Copy lines before the modified line to tempfile
    rewind(customerfile);

    int current_pos = 0;

    while (current_pos < position && (buffer = fgetc(customerfile)) != EOF) {
        fputc(buffer, tempfile);
        current_pos++;
    }

    // Write the new text
    fprintf(tempfile, "%s", text);

    // Skip the old line
    while ((buffer = fgetc(customerfile)) != EOF && buffer != ',' && buffer != '\n' );
    if (last_token == 0)
        fputc(',', tempfile);
    else
        fputc('\n', tempfile);

    // Copy the rest of the lines to tempfile
    while ((buffer = fgetc(customerfile)) != EOF)
    {
        fputc(buffer, tempfile);
    }

    fclose(customerfile);
    fclose(tempfile);

    // Replace the original file with the temporary file
    remove("Customer_file.txt");
    rename("temporary_file.txt", "Customer_file.txt");
}

Customer update_customer_data(Customer customer, cust_file_pos *pos)
{
    char rep[50];
    int stop = 0;
    int action;
    int error;

    while (stop == 0)
    {
        printf("\nwhat do you want to change? (write 'stop' to stop)\n1)username\n2)password\n3)birth date\n4)leave\n");
        scanf("%d",&action);
        // Clear input buffer
        while (getchar() != '\n');

        switch(action)
        {
        case 1:

            printf("what do you want to replace it with?");
            scanf("%s",rep);
            while (strlen(rep)>USERNAMENB)
            {
                printf("Please enter only %d characters\n", USERNAMENB);
                scanf("%s",rep);
            }
            modify_customer_file(pos->un,rep,0);

            customer = check_customer_details(rep, pos,&error);

            break;
        case 2:

            printf("what do you want to replace it with?");
            scanf("%s",rep);
            while (strlen(rep)>PASSWORDNB)
            {
                printf("Please enter only %d characters\n", PASSWORDNB);
                scanf("%s",rep);
            }
            modify_customer_file(pos->pw,rep,0);

            customer = check_customer_details(customer.username, pos, &error);

            break;
        case 3:

            printf("what do you want to replace it with?");
            scanf("%s",rep);
            while (strlen(rep)!=10)
            {
                printf("please enter it with the form dd/mm/yyyy");
                scanf("%s",rep);
            }
            modify_customer_file(pos->bd,rep,0);

            customer = check_customer_details(customer.username, pos,&error);

            break;
        case 4:
            stop = 1;
            break;
        default:
            printf("please enter a digit to perform an action\n");
        }
    }

    return customer;
}

Customer withdraw(Customer customer, cust_file_pos *pos, float amount)
{
    float amount_left;
    char str[50];
    int error;

    amount_left = customer.current_account_balance - amount;
    snprintf(str, sizeof(str), "%.2f", amount_left);

    modify_customer_file(pos->cab, str,1);

    customer = check_customer_details(customer.username, pos, &error);
    return customer;
}

Customer deposit(Customer customer, cust_file_pos *pos, float amount)
{
    float amount_left;
    char str[100];
    int error;


    amount_left = customer.current_account_balance + amount;
    sprintf(str, "%.2f", amount_left);

    modify_customer_file(pos->cab, str, 1);

    customer = check_customer_details(customer.username, pos, &error);
    return customer;
}

void transfer_money(Customer customer, cust_file_pos *pos)
{
    Customer banana2;
    char username[USERNAMENB];
    cust_file_pos pos2;
    int error = 0;
    float amount;

    printf("How much\n");
    scanf("%f",&amount);
    getchar();

    while(1)
    {
        printf("to who?\n");
        fgets(username, USERNAMENB, stdin);

        // Remove newline character if present
        if (username[strlen(username) - 1] == '\n')
            username[strlen(username) - 1] = '\0';

        printf("--%s--",username);
        banana2 = check_customer_details(username, &pos2, &error);
        if (error == 0)
            break;
        printf("user not found\n");
    }

    printf("%.2f, %.2f",banana2.current_account_balance, banana2.total_monthly_loan_payments);
    deposit(banana2, &pos2, amount);
    printf("%.2f",banana2.current_account_balance);
    withdraw(customer, pos, amount);
}

/*Customer loan_eligibility(Customer customer, cust_file_pos* pos)
{
    int i = 0;
    char answer[4];
    float total_mlp = customer.total_monthly_loan_payments;
    float Loan_eligibility, amount, time;

    i = 0;

    printf("How much would you like to borrow? ");
    scanf("%f", &amount);
    printf("During how many months? ");
    scanf("%f", &time);

    Loan_eligibility = (customer.net_monthly_salary * 0.33) - customer.total_monthly_loan_payments;
    printf("%f\n",Loan_eligibility);
    printf("%f",amount / time);
    if ((amount / time) <= Loan_eligibility)
    {
        while (amount > 0) {
            amount -= Loan_eligibility;
            i++;
        }
        printf("You will pay your debt in %i months\n", i);
        printf("You can do it, would you like to proceed? (yes or no) ");

        // Clear the input buffer before reading a new line
        while (getchar() != '\n');
        fgets(answer, sizeof(answer), stdin);
        answer[strcspn(answer, "\n")] = '\0';

        if (strcmp(answer, "yes") == 0) {
            total_mlp += (Loan_eligibility * i);
            char temp[20];
            gcvt(total_mlp, 20, total_mlp);
            modify_customer_file(pos->tmlp, temp, 0);

            customer = check_customer_details(customer.username, pos, 0);
        }
        else
        {
            printf("Loan application canceled.\n");
        }
    }
    else
    {
        printf("Loan amount per month exceeds your eligibility.\n");
    }
    return customer;
}
*/
void send_message(Customer customer, Advisor advisor) {
    FILE *messageFile = fopen("messages.txt", "a"); // Open file in append mode
    if (!messageFile) {
        perror("Failed to open file");
        return;
    }

    char message[LINEMAXSIZE];
    printf("Enter your message (max 100 caracters):\n");
    fgets(message, LINEMAXSIZE, stdin);

    // Remove newline character if present
    if (message[strlen(message) - 1] == '\n') {
        message[strlen(message) - 1] = '\0';
    }

    // Write message to file
    fprintf(messageFile, "%s,%s: %s\n", customer.RIB, customer.username, message);
    fclose(messageFile);

    printf("Message sent successfully to %s!\n", advisor.username);
}

void create_customer(Advisor advisor) {
    // Open the file for appending new customers

    FILE *customerFile = fopen("Customer_file.txt", "a");
    if (!customerFile) {
        perror("Failed to open Customer_file.txt for appending");
        return;
    }

    int stop = 0;
    while (stop == 0) {
        // Collect new customer details
        Customer newCustomer;

        printf("\nEnter new customer details:\n");

        printf("RIB (up to %d characters): ", MAX_RIB_LENGTH);
        fgets(newCustomer.RIB, sizeof(newCustomer.RIB), stdin);
        newCustomer.RIB[strcspn(newCustomer.RIB, "\n")] = '\0'; // Remove newline character

        printf("Username (up to %d characters): ", USERNAMENB);
        fgets(newCustomer.username, sizeof(newCustomer.username), stdin);
        newCustomer.username[strcspn(newCustomer.username, "\n")] = '\0'; // Remove newline character

        printf("Password (up to %d characters): ", PASSWORDNB);
        fgets(newCustomer.password, sizeof(newCustomer.password), stdin);
        newCustomer.password[strcspn(newCustomer.password, "\n")] = '\0'; // Remove newline character

        printf("Date of Birth (dd/mm/yyyy): ");
        fgets(newCustomer.date_of_birth, sizeof(newCustomer.date_of_birth), stdin);
        newCustomer.date_of_birth[strcspn(newCustomer.date_of_birth, "\n")] = '\0'; // Remove newline character

        printf("Net Monthly Salary: ");
        while (scanf("%f", &newCustomer.net_monthly_salary) != 1 || newCustomer.net_monthly_salary < 0) {
            clear_buffer();
            printf("Invalid input. Please enter a non-negative number: ");
        }

        printf("Total Monthly Loan Payments: ");
        while (scanf("%f", &newCustomer.total_monthly_loan_payments) != 1 || newCustomer.total_monthly_loan_payments < 0) {
            clear_buffer();
            printf("Invalid input. Please enter a non-negative number: ");
        }

        printf("Current Account Balance: ");
        while (scanf("%f", &newCustomer.current_account_balance) != 1 || newCustomer.current_account_balance < 0) {
            clear_buffer();
            printf("Invalid input. Please enter a non-negative number: ");
        }

        // Clear input buffer
        clear_buffer();

        // Write the new customer details to the file
        if (fprintf(customerFile, "%s,%d,%s,%s,%s,%.2f,%.2f,%.2f\n", newCustomer.RIB, advisor.advisorID, newCustomer.username,
                newCustomer.password, newCustomer.date_of_birth, newCustomer.net_monthly_salary,
                newCustomer.total_monthly_loan_payments, newCustomer.current_account_balance) < 0) {
            perror("Error writing to Customer_file.txt");
            break;
        }
        printf("\nNew customer added successfully!\n");

        printf("\nDo you want to add another customer?\nYes-->0\nNo-->1\n");
        while (scanf("%d", &stop) != 1 || (stop != 0 && stop != 1)) {
            clear_buffer();
            printf("Invalid input. Please enter 0 or 1: ");
        }
    }
    fclose(customerFile);
}

void delete_customer(Advisor advisor) {
    FILE *customerFile = fopen("Customer_file.txt", "r");
    if (!customerFile) {
        perror("Failed to open Customer_file.txt for reading");
        return;
    }

    // Display all customers
    printf("List of customers:\n");
    Customer customer;
    char line[LINEMAXSIZE];
    while (fgets(line, sizeof(line), customerFile)) {
        sscanf(line, "%20[^,],%d,%50[^,],%50[^,],%10[^,],%f,%f,%f",
               customer.RIB, &customer.advisorID, customer.username, customer.password,
               customer.date_of_birth, &customer.net_monthly_salary,
               &customer.total_monthly_loan_payments, &customer.current_account_balance);

        printf("RIB: %s\n", customer.RIB);
        printf("Username: %s\n", customer.username);
        printf("Password: %s\n", customer.password);
        printf("Date of Birth: %s\n", customer.date_of_birth);
        printf("Net Monthly Salary: %.2f\n", customer.net_monthly_salary);
        printf("Total Monthly Loan Payments: %.2f\n", customer.total_monthly_loan_payments);
        printf("Current Account Balance: %.2f\n\n", customer.current_account_balance);
    }
    fclose(customerFile);

    // Ask which customer to delete
    char ribToDelete[MAX_RIB_LENGTH + 1];
    printf("\nEnter the RIB of the customer you want to delete: ");
    fgets(ribToDelete, sizeof(ribToDelete), stdin);
    ribToDelete[strcspn(ribToDelete, "\n")] = '\0'; // Remove newline character

    // Open a temporary file for writing
    FILE *tempFile = fopen("temp_Customer_file.txt", "w");
    if (!tempFile) {
        perror("Failed to open temp_Customer_file.txt for writing");
        return;
    }

    // Reopen the original customer file for reading
    customerFile = fopen("Customer_file.txt", "r");
    if (!customerFile) {
        perror("Failed to open Customer_file.txt for reading");
        fclose(tempFile);
        return;
    }

    // Copy customers to the temporary file, excluding the one to be deleted
    int customerFound = 0;
    while (fgets(line, sizeof(line), customerFile)) {
        sscanf(line, "%20[^,],%d,%50[^,],%50[^,],%10[^,],%f,%f,%f",
               customer.RIB, &customer.advisorID, customer.username, customer.password,
               customer.date_of_birth, &customer.net_monthly_salary,
               &customer.total_monthly_loan_payments, &customer.current_account_balance);

        if (strcmp(customer.RIB, ribToDelete) != 0) {
            fprintf(tempFile, "%s,%d,%s,%s,%s,%.2f,%.2f,%.2f\n",
                    customer.RIB, customer.advisorID, customer.username, customer.password,
                    customer.date_of_birth, customer.net_monthly_salary,
                    customer.total_monthly_loan_payments, customer.current_account_balance);
        } else {
            customerFound = 1;
        }
    }

    // Close both files
    fclose(customerFile);
    fclose(tempFile);

    // Remove the original customer file and rename the temporary file if customer was found
    if (customerFound) {
        if (remove("Customer_file.txt") != 0) {
            perror("Failed to delete Customer_file.txt");
            return;
        }
        if (rename("temp_Customer_file.txt", "Customer_file.txt") != 0) {
            perror("Failed to rename temp_Customer_file.txt");
            return;
        }
        printf("Customer with RIB %s has been deleted successfully.\n", ribToDelete);
    } else {
        // Remove the temporary file if customer was not found
        remove("temp_Customer_file.txt");
        printf("Customer with RIB %s not found.\n", ribToDelete);
    }
}

void take_money_by_advisor() {
    char rib[MAX_RIB_LENGTH];
    float amount;
    int found = 0;

        // Open the file for reading and writing
    FILE *customerFile = fopen("Customer_file.txt", "r+");
    if (!customerFile) {
        perror("Failed to open Customer_file.txt");
        return;
    }
    print_all_customers();

    // Prompt for the RIB of the customer
    printf("Enter the RIB of the customer from whom you want to withdraw money: ");
    scanf("%s", rib);



    // Temporarily store each line of the file
    char line[LINEMAXSIZE];

    // Read and process each line until the end of file
    while (fgets(line, sizeof(line), customerFile)) {
        Customer customer;
        sscanf(line, "%20[^,],%d,%49[^,],%49[^,],%10[^,],%f,%f,%f",
               customer.RIB, &customer.advisorID, customer.username, customer.password,
               customer.date_of_birth, &customer.net_monthly_salary,
               &customer.total_monthly_loan_payments, &customer.current_account_balance);

        // Check if the RIB matches the customer we want to withdraw from
        if (strcmp(customer.RIB, rib) == 0) {
            found = 1;
            printf("\nCurrent Account Balance: %.2f\n", customer.current_account_balance);

            // Prompt for the amount to withdraw
            printf("Enter the amount to withdraw: ");
            scanf("%f", &amount);

            // Check if sufficient balance
            if (amount > customer.current_account_balance) {
                printf("Insufficient balance. Withdrawal cancelled.\n");
                fclose(customerFile);
                return;
            }

            // Update customer's account balance
            customer.current_account_balance -= amount;

            // Update the customer's file with the new balance
            fseek(customerFile, -strlen(line), SEEK_CUR); // Move back to the beginning of the line
            fprintf(customerFile, "%s,%d,%s,%s,%s,%.2f,%.2f,%.2f\n",
                    customer.RIB, customer.advisorID, customer.username, customer.password,
                    customer.date_of_birth, customer.net_monthly_salary,
                    customer.total_monthly_loan_payments, customer.current_account_balance);

            printf("\nWithdrawal successful. New Account Balance: %.2f\n", customer.current_account_balance);
            break;
        }
    }

    // Close the file
    fclose(customerFile);

    if (!found) {
        printf("Customer with RIB %s not found.\n", rib);
    }
}

void add_money_by_advisor() {
    // Print all customers
    print_all_customers();

    // Ask advisor to choose a customer by RIB
    char customerRIB[MAX_RIB_LENGTH];
    printf("Enter the RIB of the customer you want to add money to: ");
    fgets(customerRIB, sizeof(customerRIB), stdin);
    customerRIB[strcspn(customerRIB, "\n")] = '\0';  // Remove newline character

    // Open customer file to read customer details
    FILE *customerFile = fopen("Customer_file.txt", "r");
    if (!customerFile) {
        perror("Failed to open Customer_file.txt");
        return;
    }

    char line[LINEMAXSIZE];
    int found = 0;
    Customer customer;
    long position = 0;  // Position of the customer in the file

    // Temporary file for writing updated content
    FILE *tempFile = fopen("temp_Customer_file.txt", "w");
    if (!tempFile) {
        perror("Failed to open temp_Customer_file.txt");
        fclose(customerFile);
        return;
    }

    // Find the customer by RIB and update the file
    while (fgets(line, sizeof(line), customerFile)) {
        sscanf(line, "%[^,],%d,%[^,],%[^,],%[^,],%f,%f,%f",
               customer.RIB, &customer.advisorID, customer.username, customer.password,
               customer.date_of_birth, &customer.net_monthly_salary,
               &customer.total_monthly_loan_payments, &customer.current_account_balance);

        if (strcmp(customer.RIB, customerRIB) == 0) {
            found = 1;
            // Ask for the amount to add
            float amount;
            printf("Enter the amount to add to the customer's account: ");
            scanf("%f", &amount);
            getchar(); // Clear input buffer

            // Update the current account balance
            customer.current_account_balance += amount;
            printf("Money added successfully. New balance: %.2f\n", customer.current_account_balance);

            // Write the updated customer data to the temporary file
            fprintf(tempFile, "%s,%d,%s,%s,%s,%.2f,%.2f,%.2f\n",
                    customer.RIB, customer.advisorID, customer.username, customer.password,
                    customer.date_of_birth, customer.net_monthly_salary,
                    customer.total_monthly_loan_payments, customer.current_account_balance);
        } else {
            // Write the original line to the temporary file
            fputs(line, tempFile);
        }
    }

    if (!found) {
        printf("Customer with RIB %s not found.\n", customerRIB);
    }

    // Close files and replace the original file with the updated temporary file
    fclose(customerFile);
    fclose(tempFile);

    if (found) {
        if (remove("Customer_file.txt") != 0) {
            perror("Failed to delete original Customer_file.txt");
        } else if (rename("temp_Customer_file.txt", "Customer_file.txt") != 0) {
            perror("Failed to rename temp_Customer_file.txt to Customer_file.txt");
        }
    } else {
        remove("temp_Customer_file.txt");
    }
}

void print_all_customers() {
    FILE *customerFile = fopen("Customer_file.txt", "r");
    if (!customerFile) {
        perror("Failed to open Customer_file.txt");
        return;
    }

    printf("Customers:\n");

    char line[LINEMAXSIZE];
    while (fgets(line, sizeof(line), customerFile)) {
        Customer customer;
        sscanf(line, "%[^,],%d,%[^,],%[^,],%[^,],%f,%f,%f",
               customer.RIB, &customer.advisorID, customer.username, customer.password,
               customer.date_of_birth, &customer.net_monthly_salary,
               &customer.total_monthly_loan_payments, &customer.current_account_balance);
        printf("\nRIB: %s, Advisor ID: %d, Username: %s, Password: %s, Date of Birth: %s, Net Monthly Salary: %.2f, Total Monthly Loan Payments: %.2f, Current Account Balance: %.2f\n\n",
               customer.RIB, customer.advisorID, customer.username, customer.password,
               customer.date_of_birth, customer.net_monthly_salary,
               customer.total_monthly_loan_payments, customer.current_account_balance);
    }

    fclose(customerFile);
}

void change_customer_details() {
    // Open the file for reading and writing
    FILE *customerFile = fopen("Customer_file.txt", "r+");
    if (!customerFile) {
        perror("Failed to open Customer_file.txt");
        return;
    }
    print_all_customers();
    char line[LINEMAXSIZE];
    char rib[MAX_RIB_LENGTH + 1];
    printf("Enter the RIB of the customer you want to update: ");
    scanf("%20s", rib); // Limit input to 20 characters to match RIB length

    // Variables to store updated customer details
    Customer updatedCustomer;
    int found = 0;

    // Go through each line in the file
    while (fgets(line, sizeof(line), customerFile)) {
        // Parse the line to extract customer details
        Customer customer;
        sscanf(line, "%20[^,],%d,%50[^,],%50[^,],%10[^,],%f,%f,%f",
               customer.RIB, &customer.advisorID, customer.username, customer.password,
               customer.date_of_birth, &customer.net_monthly_salary,
               &customer.total_monthly_loan_payments, &customer.current_account_balance);

        // Check if the RIB matches the customer we want to update
        if (strcmp(customer.RIB, rib) == 0) {
            // Customer found, prompt for updated details
            found = 1;
            printf("\nEnter updated details for customer with RIB %s:\n", rib);
            printf("Username: ");
            scanf("%50s", updatedCustomer.username);
            printf("Password: ");
            scanf("%50s", updatedCustomer.password);
            printf("Date of Birth (dd/mm/yyyy): ");
            scanf("%10s", updatedCustomer.date_of_birth);
            printf("Net Monthly Salary: ");
            scanf("%f", &updatedCustomer.net_monthly_salary);
            printf("Total Monthly Loan Payments: ");
            scanf("%f", &updatedCustomer.total_monthly_loan_payments);
            printf("Current Account Balance: ");
            scanf("%f", &updatedCustomer.current_account_balance);

            // Move file pointer back to beginning of the line
            fseek(customerFile, -strlen(line), SEEK_CUR);

            // Write updated details to the file
            fprintf(customerFile, "%s,%d,%s,%s,%s,%.2f,%.2f,%.2f\n",
                    rib, customer.advisorID,
                    updatedCustomer.username, updatedCustomer.password, updatedCustomer.date_of_birth,
                    updatedCustomer.net_monthly_salary, updatedCustomer.total_monthly_loan_payments,
                    updatedCustomer.current_account_balance);
            printf("Customer details updated successfully!\n");
            break;
        }
    }

    if (!found) {
        printf("Customer with RIB %s not found.\n", rib);
    }

    fclose(customerFile);
}

void transfer_money_by_advisor() {
    print_all_customers(); // Print the list of customers

    // Prompt for sender RIB
    char senderRIB[MAX_RIB_LENGTH];
    printf("\nEnter the RIB of the sender: ");
    if (scanf("%19s", senderRIB) != 1) {
        printf("Error reading sender RIB.\n");
        clear_buffer();
        return;
    }
    clear_buffer(); // Clear input buffer

    // Prompt for receiver RIB
    char receiverRIB[MAX_RIB_LENGTH];
    printf("Enter the RIB of the receiver: ");
    if (scanf("%19s", receiverRIB) != 1) {
        printf("Error reading receiver RIB.\n");
        clear_buffer();
        return;
    }
    clear_buffer(); // Clear input buffer

    // Prompt for transfer amount
    float amount;
    printf("Enter the amount to transfer (in $): ");
    if (scanf("%f", &amount) != 1) {
        printf("Error reading transfer amount.\n");
        clear_buffer();
        return;
    }
    clear_buffer(); // Clear input buffer

    // Open customer file for reading and writing
    FILE *customerFile = fopen("Customer_file.txt", "r+");
    if (!customerFile) {
        perror("Failed to open Customer_file.txt");
        return;
    }

    // Variables for managing customer details and flags
    Customer customer;
    int foundSender = 0, foundReceiver = 0;
    long int positionSender = 0, positionReceiver = 0;
    char line[LINEMAXSIZE];
    FILE *tempFile = fopen("temp_Customer_file.txt", "w");
    if (!tempFile) {
        perror("Failed to open temp_Customer_file.txt");
        fclose(customerFile);
        return;
    }
    float amount2;
    amount2 = customer.current_account_balance;

    // Iterate through the customers to find the sender and receiver
    while (fgets(line, sizeof(line), customerFile)) {
        sscanf(line, "%19[^,],%d,%49[^,],%49[^,],%10[^,],%f,%f,%f",
               customer.RIB, &customer.advisorID, customer.username, customer.password,
               customer.date_of_birth, &customer.net_monthly_salary,
               &customer.total_monthly_loan_payments, &customer.current_account_balance);

        if (strcmp(customer.RIB, senderRIB) == 0) {
            foundSender = 1;
            // Check if sender has sufficient balance
            if (customer.current_account_balance < amount) {
                printf("Sender with RIB %s does not have enough balance to transfer %.2f.\n", senderRIB, amount);
                fclose(tempFile);
                fclose(customerFile);
                remove("temp_Customer_file.txt");
                return;
            }
            // Update sender's balance
            customer.current_account_balance -= amount;
            positionSender = ftell(customerFile) - strlen(line); // Get position for sender update
        } else if (strcmp(customer.RIB, receiverRIB) == 0) {
            foundReceiver = 1;
            // Update receiver's balance
            customer.current_account_balance += amount;
            positionReceiver = ftell(customerFile) - strlen(line); // Get position for receiver update
        }

        // Write customer details to temporary file
        fprintf(tempFile, "%s,%d,%s,%s,%s,%.2f,%.2f,%.2f\n",
                customer.RIB, customer.advisorID, customer.username, customer.password,
                customer.date_of_birth, customer.net_monthly_salary,
                customer.total_monthly_loan_payments, customer.current_account_balance);
    }

    // Check if both sender and receiver were found
    if (!foundSender) {
        printf("Sender with RIB %s not found.\n", senderRIB);
        fclose(tempFile);
        fclose(customerFile);
        remove("temp_Customer_file.txt");
        return;
    }

    if (!foundReceiver) {
        printf("Receiver with RIB %s not found.\n", receiverRIB);
        fclose(tempFile);
        fclose(customerFile);
        remove("temp_Customer_file.txt");
        return;
    }

    // Update Customer_file.txt with new balances
    rewind(customerFile); // Move file pointer to beginning
    fseek(customerFile, positionSender, SEEK_SET); // Move to sender's position
    fprintf(customerFile, "%s,%d,%s,%s,%s,%.2f,%.2f,%.2f\n",
            customer.RIB, customer.advisorID, customer.username, customer.password,
            customer.date_of_birth, customer.net_monthly_salary,
            customer.total_monthly_loan_payments, customer.current_account_balance);
    fseek(customerFile, positionReceiver, SEEK_SET); // Move to receiver's position
    fprintf(customerFile, "%s,%d,%s,%s,%s,%.2f,%.2f,%.2f\n",
            customer.RIB, customer.advisorID, customer.username, customer.password,
            customer.date_of_birth, customer.net_monthly_salary,
            customer.total_monthly_loan_payments, customer.current_account_balance);

    fclose(customerFile);
    fclose(tempFile);
    remove("temp_Customer_file.txt");

    printf("\nTransfer of %.2f from %s to %s completed successfully.\n", amount, senderRIB, receiverRIB);

}



void read_messages_for_advisor(Advisor advisor) {
    FILE *messageFile = fopen("messages.txt", "r");
    if (!messageFile) {
        perror("Failed to open messages.txt");
        return;
    }

    char line[LINEMAXSIZE];
    char advisorIDStr[10];
    char customerRIB[MAX_RIB_LENGTH];
    char message[100];

    printf("Messages for Advisor ID %d:\n", advisor.advisorID);
    int messageFound = 0;
    while (fgets(line, sizeof(line), messageFile)) {
        // Extract advisor ID, customer RIB, and message from the line
        sscanf(line, "%[^,], %[^,], %[^\n]", advisorIDStr, customerRIB, message);

        // Check if the message is for the current advisor
        if (atoi(advisorIDStr) == advisor.advisorID) {
            printf("From Customer (RIB: %s): %s\n\n", customerRIB, message);
            messageFound = 1;
        }
    }

    if (!messageFound) {
        printf("No messages found for Advisor ID %d.\n", advisor.advisorID);
    }

    fclose(messageFile);
}

void customer_action(Customer customer,Advisor advisor, cust_file_pos *pos){
    int action;
    int stop = 0;
    customer = check_customer_details(customer.username,pos,stop);
    while (stop == 0)
        {
            float amount=0;
            printf("\nWhat action do you want to do? :\n1)Check Personal Details\n2)Update Personal Details\n3)Transfer money\n4)Withdraw\n5)Deposit\n6)Send message\n7)Log out\n");
            scanf("%d",&action);
            getchar();
            system("cls");
            switch(action)
            {
            case 1:
                printf("RIB: %s\nusername: %s\npassword: %s\ndate_of_birth: %s\nnet_monthly_salary: %.2f\n"
                       "total_monthly_loan_payments: %.2f\ncurrent_account_balance:%.2f\n",customer.RIB,
                       customer.username,customer.password,customer.date_of_birth,customer.net_monthly_salary,
                       customer.total_monthly_loan_payments,customer.current_account_balance);
                       printf("%d",pos->pw);
                break;
            case 2:
                customer = update_customer_data(customer, pos);
                printf("success2");
                break;
            case 3:
                transfer_money(customer, pos);
                break;
            case 4:
                printf("\nHow much money do you want to withdraw?\n");
                while (1) {
                    if (scanf("%f", &amount) == 1) {
                        if (amount <= customer.current_account_balance) break;
                        else printf("Insufficient funds. You only have %.2f\n", customer.current_account_balance);
                    } else {
                        printf("Invalid input. Enter digits.\n");
                        while (getchar() != '\n');
                    }
                }

                customer = withdraw(customer, pos, amount);
                break;
            case 5:
                printf("\nHow much money do you want to deposit?\n");
                if (scanf("%f", &amount) == 1) {
                    customer = deposit(customer, pos, amount);
                } else {
                    printf("Invalid input. Enter digits.\n");
                    while (getchar() != '\n'); // Clear invalid input
                }
                break;
            case 6:
                send_message(customer,advisor);
                break;
            case 7:
                stop = 1;
                printf("\n\nLogging out...");
                break;

            default:
                printf("Please enter a digit to perform an action\n1)Check Personal Details\n2)Update Personal Details\n3)Transfer money\n4)Withdraw money\n5)Deposit money\n6)Send message\n7)Log out\n\n|");
            }
        }
}

void advisor_action(Advisor advisor, ad_file_pos *apos) { //group all advisor's action
    int action;
    int stop = 0;

    while (stop == 0) {
        printf("\nPlease enter a digit to perform an action:\n1)Check Customer Details\n2)Update Customer Details\n3)Transfer money\n4)Withdraw money\n5)Deposit money\n6)View messages\n7)Create customer\n8)Delete customer\n9)Log out\n\n|");
        scanf("%d", &action);
        getchar(); // Clear input buffer
        system("cls"); //clear the terminal

        switch (action) {
            case 1:
                print_all_customers(); //working
                end_function();
                break;
            case 2:
                change_customer_details(); //working
                clear_buffer();
                end_function();
                break;
            case 3:
                transfer_money_by_advisor();//
                clear_buffer();
                end_function();
                break;
            case 4:
                take_money_by_advisor(advisor);//working
                clear_buffer();
                end_function();
                break;
            case 5:
                add_money_by_advisor();//working
                clear_buffer();
                end_function();
                break;
            case 6:
                read_messages_for_advisor(advisor); //working
                clear_buffer();
                end_function();
                break;
            case 7:
                create_customer(advisor); //working
                clear_buffer();
                end_function();
                break;
            case 8:
                delete_customer(advisor); //working
                clear_buffer();
                end_function();
                break;
            case 9:
                stop = 1; //working
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 9.\n");
        }
    }
}


int main ()
{
    int logaction;
    int aID;
    int stop = 0;
    int error = 0;
    cust_file_pos pos;
    ad_file_pos apos;
    printf("Tom and Benjamin Banking System©\nWelcome! \n\n");
    printf("Notes: press enter if nothing is asked.\n\n");
    while (stop==0)
    {
        printf("\n\nLog in :\nCustomer-->1\nAdvisor-->2\nShut down-->3\n\n|");
        scanf("%d",&logaction);

        if (logaction==1){
            Customer customer = customer_login(&error, &pos);
            if (error){
                system("cls");
                printf("\nAn error occurred during login.\n");
            }
            else {
                system("cls");
                printf("\nLogin successful. Welcome, %s!\n\n", customer.username);
                aID=customer.advisorID;
                Advisor advisor=read_advisor_from_file(aID);
                customer_action(customer, advisor, &pos);
            }
             break;
        }

        else if (logaction==2){
            Advisor advisor = advisor_login(&error, &apos);
            if (error){
                system("cls");
                printf("\nAn error occurred during login.");
            }
            else {
                system("cls");
                printf("\nLogin successful. Welcome, %s!\n", advisor.username);
                advisor_action(advisor, &pos);
            }
        }
        else if (logaction==3){
            printf("Shutting down...");
            break;
        }
        else{
            printf("\nWrong input. Try again.\n");
            continue;
        }
    }
}



