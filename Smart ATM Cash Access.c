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
    printf("\n-------------------------------------------\n");
}

void printHeader(const char *title)
{
    printf("\n===========================================\n");
    printf("         %s         \n", title);
    printf("===========================================\n");
}

void showCards(Card cards[])
{
    printHeader("Available Cards");
    for (int i = 0; i < MAX_CARDS; i++)
    {
        printf("[%d] Card Ending in %s\n", i + 1, &cards[i].cardNumber[strlen(cards[i].cardNumber) - 2]);
    }
    printSeparator();
}

void showCardDetails(Card *card)
{
    printHeader("Card Details");
    printf("Account Number: %s\n", card->accountNumber);
    printf("Card Number: %s\n", card->cardNumber);
    printf("Balance: $%.2f\n", card->balance);
    printSeparator();
}

bool login()
{
    int inputPin, attempts = 0;

    while (attempts < MAX_ATTEMPTS)
    {
        printf("\nDefault pin: 1234\n");
        printf("\nEnter your 4-digit PIN: ");

        if (scanf("%d", &inputPin) != 1)
        {
            printf("Invalid input! Please enter a numeric PIN.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

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
    exit(1);
}

void atmMenu()
{
    printHeader("Main Menu");
    printf("1. Exit\n");
    printf("2. Check Balance\n");
    printf("3. Deposit Balance\n");
    printf("4. Withdraw Balance\n");
    printf("5. Switch Card\n");
    printf("6. Show Card Details\n");
    printSeparator();
}

void atmMachine(Card cards[])
{
    int activeCard = -1;
    int choice;
    double amount;
    char lastTwoDigits[3];
    char continueOption;

    while (1)
    {
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
                continue;
            }
        }

        while (1)
        {
            printf("\nActivated Card: %s (Balance: $%.2f)\n", cards[activeCard].cardNumber, cards[activeCard].balance);
            atmMenu();

            printf("\nEnter your choice: ");
            if (scanf("%d", &choice) != 1)
            {
                printf("Invalid input! Please enter a number.\n");
                while (getchar() != '\n')
                    ;
                continue;
            }

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
                if (scanf("%lf", &amount) != 1 || amount <= 0)
                {
                    printf("Invalid amount. Deposit failed.\n");
                    while (getchar() != '\n')
                        ;
                    continue;
                }
                cards[activeCard].balance += amount;
                printf("Deposit successful! New balance: $%.2f\n", cards[activeCard].balance);
                break;

            case 4:
                printf("Enter the amount to withdraw: $");
                if (scanf("%lf", &amount) != 1 || amount <= 0)
                {
                    printf("Invalid amount. Withdrawal failed.\n");
                    while (getchar() != '\n')
                        ;
                    continue;
                }
                if (amount > cards[activeCard].balance)
                {
                    printf("Insufficient funds. Withdrawal failed.\n");
                }
                else
                {
                    cards[activeCard].balance -= amount;
                    printf("Withdrawal successful! New balance: $%.2f\n", cards[activeCard].balance);
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
                }
                break;

            case 6:
                showCardDetails(&cards[activeCard]);

                printf("\nDo you want to perform another action? (Y/N): ");
                scanf(" %c", &continueOption);

                if (continueOption == 'N' || continueOption == 'n')
                {
                    printf("Exiting ATM. Thank you!\n");
                    return;
                }
                break;

            default:
                printf("Invalid choice. Please try again.\n");
            }
        }
    }
}

int main()
{
    Card cards[MAX_CARDS] = {
        {"0322310105101042", "032231042", 100000.0},
        {"0322310205101014", "032231014", 50000.0},
        {"0322310105101025", "032231025", 99000.0}};

    printHeader("Welcome to the ATM Machine");

    if (login())
    {
        atmMachine(cards);
    }

    printHeader("Thank You for Using Our ATM!");
    return 0;
}
