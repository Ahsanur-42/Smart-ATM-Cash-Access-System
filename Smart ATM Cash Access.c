#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define PIN 1234
#define MAX_ATTEMPTS 3
#define MAX_CARDS 3

typedef struct
{
    char accountNumber[20];
    char cardNumber[20];
    double balance;
} Card;

void printSeparator()
{
    printf("\n===========================================\n");
}

void showCards(Card cards[])
{
    printf("\nAvailable Cards:\n");
    for (int i = 0; i < MAX_CARDS; i++)
    {
        printf("Account Number: %s\n", cards[i].accountNumber);
        printf("Card Number: %s\n", cards[i].cardNumber);
        printf("Balance: $%.2f\n", cards[i].balance);
        printSeparator();
    }
}

bool login()
{
    int inputPin, attempts = 0;

    while (attempts < MAX_ATTEMPTS)
    {
        printf("Default pin is '1234'\n");
        printf("Enter your 4-digit PIN: ");
        scanf("%d", &inputPin);

        if (inputPin == PIN)
        {
            printf("\nLogin successful!\n");
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
    printf("1. Exit\n");
    printf("2. Check Balance\n");
    printf("3. Deposit\n");
    printf("4. Withdraw\n");
    printf("5. Switch Card\n");
}

void atmMachine(Card cards[])
{
    int activeCard = -1;
    int choice;
    double amount;
    char lastTwoDigits[3];

    while (1)
    {
        printSeparator();

        if (activeCard == -1)
        {
            showCards(cards);
            printf("Enter the last 2 digits of the card number to activate: ");
            scanf("%s", lastTwoDigits);
            bool found = false;
            for (int i = 0; i < MAX_CARDS; i++)
            {
                if (strlen(cards[i].cardNumber) >= 2 &&
                    strcmp(&cards[i].cardNumber[strlen(cards[i].cardNumber) - 2], lastTwoDigits) == 0)
                {
                    activeCard = i;
                    printf("Activated Card: %s\n", cards[activeCard].cardNumber);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                printf("No card found with the entered last 2 digits. Try again.\n");
            }
            continue;
        }

        printf("Active Card: %s (Balance: $%.2f)\n", cards[activeCard].cardNumber, cards[activeCard].balance);
        atmMenu();
        printSeparator();

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Thank you for using the ATM. Goodbye!\n");
            return;

        case 2:
            printf("Your current balance on Card %s is: $%.2f\n", cards[activeCard].cardNumber, cards[activeCard].balance);
            break;

        case 3:
            printf("Enter the amount to deposit: $");
            scanf("%lf", &amount);
            if (amount > 0)
            {
                cards[activeCard].balance += amount;
                printf("Deposit successful! New balance on Card %s: $%.2f\n", cards[activeCard].cardNumber, cards[activeCard].balance);
            }
            else
            {
                printf("Invalid amount. Deposit failed.\n");
            }
            break;

        case 4:
            printf("Enter the amount to withdraw: $");
            scanf("%lf", &amount);
            if (amount > 0 && amount <= cards[activeCard].balance)
            {
                cards[activeCard].balance -= amount;
                printf("Withdrawal successful! New balance on Card %s: $%.2f\n", cards[activeCard].cardNumber, cards[activeCard].balance);
            }
            else if (amount > cards[activeCard].balance)
            {
                printf("Insufficient funds. Withdrawal failed.\n");
            }
            else
            {
                printf("Invalid amount. Withdrawal failed.\n");
            }
            break;

        case 5:
            printf("Switching card...\n");
            showCards(cards);
            printf("Enter the last 2 digits of the card number to activate: ");
            scanf("%s", lastTwoDigits);

            bool found = false;
            for (int i = 0; i < MAX_CARDS; i++)
            {
                if (strlen(cards[i].cardNumber) >= 2 &&
                    strcmp(&cards[i].cardNumber[strlen(cards[i].cardNumber) - 2], lastTwoDigits) == 0)
                {
                    activeCard = i;
                    printf("Switched to Card: %s (Balance: $%.2f)\n", cards[activeCard].cardNumber, cards[activeCard].balance);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                printf("No card found with the entered last 2 digits. Try again.\n");
                activeCard = -1;
            }
            break;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    }
}

int main()
{
    Card cards[MAX_CARDS] = {
        {"0322310105101042", "032231042", 100000.0},
        {"0322310205101014", "032231014", 50000.0},
        {"0322310105101025", "032231025", 99000.0}};

    printSeparator();
    printf("Welcome to the ATM Machine\n");
    printSeparator();

    if (login())
    {
        atmMachine(cards);
    }

    printSeparator();
    return 0;
}
