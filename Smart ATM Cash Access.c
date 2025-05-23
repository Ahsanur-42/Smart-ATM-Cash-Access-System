#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define MAX_ATTEMPTS 3
#define MAX_CARDS 10
#define DAILY_LIMIT 50000.0
#define MONTHLY_LIMIT 1000000.0
#define SINGLE_WITHDRAW_LIMIT 20000.0

#define CARD_FOLDER_PATH "G:\\5th C Project Data"

typedef struct
{
    char name[50];
    char accountNumber[20];
    char cardNumber[20];
    int pin;
    double balance;
    double dailyWithdrawn;
    double monthlyWithdrawn;
    int lastWithdrawMonth;
    char lastWithdrawalDate[11];
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

bool confirmAction(const char *message)
{
    char choice;
    printf("%s (Y/N or y/n): ", message);
    scanf(" %c", &choice);
    return (choice == 'Y' || choice == 'y');
}

bool login(Card *card)
{
    int inputPin, attempts = 0;
    while (attempts < MAX_ATTEMPTS)
    {
        printf("\nEnter your 4-digit PIN: ");
        if (scanf("%d", &inputPin) != 1)
        {
            printf("Invalid input! Please enter a numeric PIN.\n");
            while (getchar() != '\n')
                ;
            continue;
        }
        if (inputPin == card->pin)
        {
            printf("\nLogin successful!\n");
            return true;
        }
        else
        {
            printf("Incorrect PIN. Attempts remaining: %d\n", MAX_ATTEMPTS - attempts - 1);
            attempts++;
        }
    }
    printf("Too many incorrect attempts. Access denied.\n");
    printf("Please, Contact to our nearest official branch for more details.\n");
    printf("Please, Contact to your nearest official branch for reactivate your card!\n");
    exit(1);
}

void resetPin(Card *card)
{
    printHeader("PIN Reset");

    int oldPin, newPin, confirmPin;
    int attempts = 0;
    const int maxAttempts = 3;

    while (attempts < maxAttempts)
    {
        printf("Enter current PIN: ");
        if (scanf("%d", &oldPin) == 1 && oldPin == card->pin)
        {
            printf("Enter new 4-digit PIN: ");
            if (scanf("%d", &newPin) != 1 || newPin < 1000 || newPin > 9999)
            {
                printf("Invalid PIN format. Reset cancelled.\n");
                while (getchar() != '\n')
                    ;
                return;
            }

            printf("Confirm new PIN: ");
            if (scanf("%d", &confirmPin) != 1 || confirmPin != newPin)
            {
                printf("PIN confirmation failed. Reset cancelled.\n");
                while (getchar() != '\n')
                    ;
                return;
            }

            card->pin = newPin;
            printf("PIN reset successful. Please log in again with your new PIN.\n");

            saveCardToTextFile(card);

            return;
        }
        else
        {
            attempts++;
            printf("Incorrect PIN. Attempts remaining: %d\n", maxAttempts - attempts);
            while (getchar() != '\n')
                ;
        }
    }

    printf("Too many incorrect attempts. Access denied.\n");
    exit(1);
}

void showCards(Card cards[], int cardCount)
{
    printHeader("Available Cards");
    for (int i = 0; i < cardCount; i++)
    {
        printf("[%d] Card Ending in %s\n", i + 1, &cards[i].cardNumber[strlen(cards[i].cardNumber) - 2]);
    }
    printSeparator();
}

void saveCardToTextFile(const Card *card)
{
    char filePath[100];
    snprintf(filePath, sizeof(filePath), "%s\\%s.txt", CARD_FOLDER_PATH, card->cardNumber);

    FILE *file = fopen(filePath, "w");
    if (!file)
    {
        perror("Error saving card");
        return;
    }

    fprintf(file, "Name: %s\n", card->name);
    fprintf(file, "AccountNumber: %s\n", card->accountNumber);
    fprintf(file, "CardNumber: %s\n", card->cardNumber);
    fprintf(file, "PIN: %d\n", card->pin);
    fprintf(file, "Balance: %.2f\n", card->balance);
    fprintf(file, "DailyWithdrawn: %.2f\n", card->dailyWithdrawn);
    fprintf(file, "MonthlyWithdrawn: %.2f\n", card->monthlyWithdrawn);
    fprintf(file, "LastWithdrawMonth: %d\n", card->lastWithdrawMonth);
    fprintf(file, "LastWithdrawalDate:%s\n", card->lastWithdrawalDate);

    fclose(file);
}

void saveCardsToFile(Card cards[], int cardCount)
{
    for (int i = 0; i < cardCount; i++)
    {
        saveCardToTextFile(&cards[i]);
    }
}

int loadCardsFromFile(Card cards[])
{
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char searchPath[100];
    snprintf(searchPath, sizeof(searchPath), "%s\\*.txt", CARD_FOLDER_PATH);

    int count = 0;

    hFind = FindFirstFile(searchPath, &findFileData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        printf("No card files found. Starting with default data.\n");
        return 0;
    }

    do
    {
        if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            char filePath[150];
            snprintf(filePath, sizeof(filePath), "%s\\%s", CARD_FOLDER_PATH, findFileData.cFileName);

            FILE *file = fopen(filePath, "r");
            if (!file)
            {
                perror("Error reading card file");
                continue;
            }

            Card card;
            fscanf(file, "Name: %49[^\n]\n", card.name);
            fscanf(file, "AccountNumber: %19s\n", card.accountNumber);
            fscanf(file, "CardNumber: %19s\n", card.cardNumber);
            fscanf(file, "PIN: %d\n", &card.pin);
            fscanf(file, "Balance: %lf\n", &card.balance);
            fscanf(file, "DailyWithdrawn: %lf\n", &card.dailyWithdrawn);
            fscanf(file, "MonthlyWithdrawn: %lf\n", &card.monthlyWithdrawn);
            fscanf(file, "LastWithdrawMonth: %d\n", &card.lastWithdrawMonth);
            fscanf(file, "LastWithdrawalDate:%10s\n", card.lastWithdrawalDate);

            fclose(file);

            if (count < MAX_CARDS)
                cards[count++] = card;
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    return count;
}

int selectCardByLastTwoDigits(Card cards[], int cardCount)
{
    showCards(cards, cardCount);
    char lastTwoDigits[3];

    printf("Enter the last two digits of the card to activate: ");
    scanf("%2s", lastTwoDigits);

    for (int i = 0; i < cardCount; i++)
    {
        if (strcmp(&cards[i].cardNumber[strlen(cards[i].cardNumber) - 2], lastTwoDigits) == 0)
        {
            printf("Card ending in %s activated.\n", lastTwoDigits);
            return i;
        }
    }

    printf("No card found with the last two digits '%s'.\n", lastTwoDigits);
    return -1;
}

void atmMenu()
{
    printHeader("Main Menu");
    printf("1. Instant Deposit\n");
    printf("2. Withdraw\n");
    printf("3. Check Balance\n");
    printf("4. Show Card Details\n");
    printf("5. Switch Card\n");
    printf("6. Card to Mobile Transfer\n");
    printf("7. Add New Card\n");
    printf("8. Reset PIN\n");
    printf("9. Exit\n");
    printSeparator();
}

void checkBalance(Card *card)
{
    printHeader("Balance Information");
    printf("Cardholder Name: %s\n", card->name);
    printf("Account Number: %s\n", card->accountNumber);
    printf("Card Number: %s\n", card->cardNumber);
    printf("Current Balance: $%.2f\n", card->balance);
    printSeparator();
}

void deposit(Card *card)
{
    printHeader("Deposit");
    double amount;

    printf("Enter amount to deposit: $");
    if (scanf("%lf", &amount) != 1 || amount <= 0)
    {
        printf("Invalid amount entered.\n");
        return;
    }

    card->balance += amount;
    printf("Deposit of $%.2f successful! New balance: $%.2f\n", amount, card->balance);

    saveCardToTextFile(card);
}

void withdraw(Card *card)
{
    printHeader("Withdraw");
    double amount;

    time_t t = time(NULL);
    struct tm *now = localtime(&t);

    int currentMonth = now->tm_mon + 1;

    char todayDate[11];
    strftime(todayDate, sizeof(todayDate), "%Y-%m-%d", now);

    if (card->lastWithdrawMonth != currentMonth)
    {
        card->monthlyWithdrawn = 0;
        card->dailyWithdrawn = 0;
        card->lastWithdrawMonth = currentMonth;
    }

    if (strcmp(card->lastWithdrawalDate, todayDate) != 0)
    {
        card->dailyWithdrawn = 0;
    }

    printf("Enter amount to withdraw: $");
    if (scanf("%lf", &amount) != 1 || amount <= 0)
    {
        printf("Invalid amount entered.\n");
        return;
    }

    if (amount > SINGLE_WITHDRAW_LIMIT)
    {
        printf("Cannot withdraw more than $%.2f in a single transaction.\n", SINGLE_WITHDRAW_LIMIT);
        return;
    }

    if (amount + card->dailyWithdrawn > DAILY_LIMIT)
    {
        printf("Daily withdrawal limit of $%.2f exceeded.\n", DAILY_LIMIT);
        return;
    }

    if (amount + card->monthlyWithdrawn > MONTHLY_LIMIT)
    {
        printf("Monthly withdrawal limit of $%.2f exceeded.\n", MONTHLY_LIMIT);
        return;
    }

    if (amount > card->balance)
    {
        printf("Insufficient funds.\n");
        return;
    }

    card->balance -= amount;
    card->dailyWithdrawn += amount;
    card->monthlyWithdrawn += amount;
    strncpy(card->lastWithdrawalDate, todayDate, sizeof(card->lastWithdrawalDate));
    card->lastWithdrawalDate[sizeof(card->lastWithdrawalDate) - 1] = '\0';

    printf("\nWithdrawal of $%.2f is successful! \nNew balance: $%.2f\n\n", amount, card->balance);

    saveCardToTextFile(card);
}

void showCardDetails(Card *card)
{
    printHeader("Card Details");
    printf("Cardholder Name: %s\n", card->name);

    printf("Account Number: %s\n", card->accountNumber);
    printf("Card Number: %s\n", card->cardNumber);
    printf("Balance: $%.2f\n", card->balance);
    printf("Daily Withdrawn: $%.2f\n", card->dailyWithdrawn);
    printf("Monthly Withdrawn: $%.2f\n", card->monthlyWithdrawn);
    printf("Last Withdrawal Month: %d\n", card->lastWithdrawMonth);
    printf("Last Withdrawal Date: %s\n", card->lastWithdrawalDate);

    printSeparator();
}

void cardToMobileTransfer(Card *card)
{
    printHeader("Card to Mobile Transfer");
    char mobileNumber[15];
    double amount;
    int bankChoice;

    printf("Select Mobile Banking Service:\n");
    printf("1. bKash\n");
    printf("2. Rocket\n");
    printf("3. Nagad\n");
    printf("4. Upay\n");
    printf("Enter choice (1-4): ");
    if (scanf("%d", &bankChoice) != 1 || bankChoice < 1 || bankChoice > 4)
    {
        printf("\nInvalid bank selection. Please try again.\n");
        return;
    }

    const char *bankName;
    int expectedLength;

    switch (bankChoice)
    {
    case 1:
        bankName = "bKash";
        expectedLength = 11;
        break;
    case 2:
        bankName = "Rocket";
        expectedLength = 12;
        break;
    case 3:
        bankName = "Nagad";
        expectedLength = 11;
        break;
    case 4:
        bankName = "Upay";
        expectedLength = 11;
        break;
    default:
        printf("\nUnexpected error occurred.\n");
        return;
    }

    while (1)
    {
        printf("Enter %s Number: ", bankName);
        scanf("%14s", mobileNumber);

        int len = strlen(mobileNumber);
        if (len == expectedLength && strncmp(mobileNumber, "01", 2) == 0)
        {
            break; // valid number
        }
        printf("\nInvalid number! %s numbers must:\n", bankName);
        printf("   - Be exactly %d digits\n", expectedLength);
        printf("   - Start with '01'\n\n");
    }

    printf("Enter amount to transfer: $");
    if (scanf("%lf", &amount) != 1 || amount <= 0)
    {
        printf("\nInvalid amount entered.\n");
        return;
    }

    if (amount > card->balance)
    {
        printf("\nInsufficient balance. Your current balance is $%.2f\n", card->balance);
        return;
    }

    card->balance -= amount;
    printf("\nSuccessfully transferred $%.2f to %s number %s.\n", amount, bankName, mobileNumber);
    printf("New balance: $%.2f\n\n", card->balance);

    saveCardToTextFile(card);
}

void addNewCard(Card cards[], int *cardCount)
{
    if (*cardCount >= MAX_CARDS)
    {
        printf("Maximum number of cards reached. Cannot add more.\n");
        return;
    }

    printHeader("Add New Card");
    Card newCard;

    printf("Enter Cardholder Name: ");
    scanf(" %[^\n]", newCard.name);

    printf("Enter Account Number: ");
    scanf("%19s", newCard.accountNumber);

    printf("Enter Card Number: ");
    scanf("%19s", newCard.cardNumber);

    printf("Set 4-digit PIN: ");
    scanf("%d", &newCard.pin);

    printf("Enter Initial Balance: $");
    scanf("%lf", &newCard.balance);

    newCard.dailyWithdrawn = 0;
    newCard.monthlyWithdrawn = 0;
    newCard.lastWithdrawMonth = -1;

    cards[(*cardCount)++] = newCard;

    saveCardToTextFile(&newCard);
    printf("New card added and saved successfully!\n");
}

int main()
{
    Card cards[MAX_CARDS];
    int cardCount = loadCardsFromFile(cards);

    if (cardCount == 0)
    {
        cards[0] = (Card){"MD AHSANUR RAHAMAN", "0322310105101042", "032231042", 1234, 100000.0, 0, 0, -1, ""};
        cards[1] = (Card){"MST SOMAIYA ALAM ASHA", "0322310205101014", "032231014", 5678, 50000.0, 0, 0, -1, ""};
        cards[2] = (Card){"TUSAR CHANDRA", "0322310105101025", "032231025", 9012, 99000.0, 0, 0, -1, ""};
        cardCount = 3;
        saveCardsToFile(cards, cardCount);
    }
    int activeCardIndex = -1;

    printHeader("Welcome to the ATM Machine");

    while (true)
    {
        if (activeCardIndex == -1)
        {
            activeCardIndex = selectCardByLastTwoDigits(cards, cardCount);
            if (activeCardIndex == -1)
                continue;
            if (!login(&cards[activeCardIndex]))
                continue;
        }

        atmMenu();
        printf("Enter your choice: ");
        int choice;
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. \nPlease enter a number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 1:
            deposit(&cards[activeCardIndex]);
            break;
        case 2:
            withdraw(&cards[activeCardIndex]);
            break;
        case 3:
            checkBalance(&cards[activeCardIndex]);
            break;
        case 4:
            showCardDetails(&cards[activeCardIndex]);
            break;
        case 5:
            printf("Switching card...\n");
            activeCardIndex = -1;
            break;
        case 6:
            cardToMobileTransfer(&cards[activeCardIndex]);
            break;
        case 7:
            addNewCard(cards, &cardCount);
            break;
        case 8:
            resetPin(&cards[activeCardIndex]);
            break;
        case 9:
            if (confirmAction("Are you sure you want to exit?"))
            {
                printf("Thank you for using the ATM. Goodbye!\n");
                return 0;
            }
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }

        if (choice != 5 && choice != 9 && !confirmAction("Do you want to perform another action?"))
        {
            printf("Thank you for using the ATM. Goodbye!\n");
            printf("Team Syntax Error!\n");
            printf("Powered by PUB!");
            printf("Powered by CSE, PUB!");
            break;
        }
    }

    return 0;
}
