#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define PIN 1234       // Default PIN
#define MAX_ATTEMPTS 3 // Max login attempts

void printSeparator()
{
    printf("\n===========================================\n");
}

bool login()
{
    int inputPin, attempts = 0;

    while (attempts < MAX_ATTEMPTS)
    {
        printf("Enter your 4-digit PIN: ");
        scanf("%d", &inputPin);

        if (inputPin == PIN)
        {
            printf("Login successful!\n");
            return true;
        }
        else
        {
            printf("Incorrect PIN. Try again.\n");
            attempts++;
        }
    }

    printf("Too many incorrect attempts. Access denied.\n");
    return false;
}

void atmMenu()
{
    printf("\nSelect an option:\n");
    printf("1. Check Balance\n");
    printf("2. Deposit\n");
    printf("3. Withdraw\n");
    printf("4. Exit\n");
}

void atmMachine()
{
    double balance = 1000.0; // Default account balance
    int choice;
    double amount;

    while (1)
    {
        printSeparator();
        atmMenu();
        printSeparator();

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Your current balance is: $%.2f\n", balance);
            break;

        case 2:
            printf("Enter the amount to deposit: $");
            scanf("%lf", &amount);
            if (amount > 0)
            {
                balance += amount;
                printf("Deposit successful! New balance: $%.2f\n", balance);
            }
            else
            {
                printf("Invalid amount. Deposit failed.\n");
            }
            break;

        case 3:
            printf("Enter the amount to withdraw: $");
            scanf("%lf", &amount);
            if (amount > 0 && amount <= balance)
            {
                balance -= amount;
                printf("Withdrawal successful! New balance: $%.2f\n", balance);
            }
            else if (amount > balance)
            {
                printf("Insufficient funds. Withdrawal failed.\n");
            }
            else
            {
                printf("Invalid amount. Withdrawal failed.\n");
            }
            break;

        case 4:
            printf("Thank you for using the ATM. Goodbye!\n");
            return;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

int main()
{
    printSeparator();
    printf("Welcome to the ATM Machine\n");
    printSeparator();

    if (login())
    {
        atmMachine();
    }

    printSeparator();
    printf("Program terminated.\n");
    return 0;
}