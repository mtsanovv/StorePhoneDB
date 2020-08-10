#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <float.h>

typedef struct
{
    char ID[12];
    double price;
    char model[20];
    short quantity;
} Phone;

typedef struct
{
  Phone phoneData;
  struct PhonesList *next;
} PhonesList;

void consoleLog(unsigned logLevel, char *message);
void writeToFile(PhonesList *head);
PhonesList* createEntry(Phone phoneData);
PhonesList* fillListFromFile(PhonesList *head);
PhonesList* addPhone(PhonesList *head);
PhonesList* changePhoneQuantity(PhonesList *head);
PhonesList* deletePhone(PhonesList *head, PhonesList *prev, PhonesList *current);
void printExpensivePhones(PhonesList *head);
void printPhoneById(PhonesList *head);
void listAllPhones(PhonesList *head);
void printPhone(Phone phoneData);
void freePhonesList(PhonesList *head);
int isNumeric(char *str, unsigned len);

int main()
{
    PhonesList *head = NULL;
    unsigned commandId;
    printf("\n| | | Marin Tsanov's Store Phone Database\n");
    consoleLog(1, "To find out what the possible options are, please type in 1 when prompted for an option number.");
    head = fillListFromFile(head);
    while(commandId != 0)
    {
        printf("Please enter an option number: ");
        scanf("%u", &commandId);
        switch(commandId)
        {
            case 0:
                freePhonesList(head);
                exit(0);
                break;
            case 1:
                consoleLog(1, "The following options are available: \n\n\t0 - Exit the program\n\t1 - Show options list\n\t2 - Add a new phone\n\t3 - Change quantity of a phone (can be used to delete phones)\n\t4 - Display info about all phones that cost more than the store's average\n\t5 - Display info about a phone by ID\n\t6 - List all phones");
                break;
            case 2:
                head = addPhone(head);
                break;
            case 3:
                head = changePhoneQuantity(head);
                break;
            case 4:
                printExpensivePhones(head);
                break;
            case 5:
                printPhoneById(head);
                break;
            case 6:
                listAllPhones(head);
                break;
            default:
                consoleLog(3, "Invalid option number.");
                break;
        }
    }
    return 0;
}

void consoleLog(unsigned logLevel, char *message)
{
    switch(logLevel)
    {
        case 1:
            printf("\n\n[HELP] %s\n\n", message);
            break;
        case 2:
            printf("\n\n[WARN] %s\n\n", message);
            break;
        case 3:
            printf("\n\n[ERROR] %s\n\n", message);
            break;
        default:
            printf("\n\n[INFO] %s\n\n", message);
            break;
    }
}

PhonesList* createEntry(Phone phoneData)
{
    PhonesList *entry = (PhonesList *) malloc(sizeof(PhonesList));

    if(entry == NULL)
    {
        consoleLog(3, "Failed creating phone entry, it may be as a result of a memory problem. For safety reasons, the program will now quit.");
        exit(1);
    }

    entry->phoneData = phoneData;
    entry->next = NULL;

    return entry;
}

PhonesList* fillListFromFile(PhonesList *head)
{
    Phone phoneData;
    PhonesList *current = NULL;
    FILE *fp;
    fp = fopen("phonedb.bin", "rb");
    if(fp == NULL)
    {
        consoleLog(2, "Phone database file phonedb.bin cannot be opened. No phones have been loaded.");
        return head;
    }
    while(1)
    {
        if(fread(&phoneData, sizeof(Phone), 1, fp) != 1)
        {
            if(!feof(fp))
            {
                consoleLog(3, "The phone database file phonedb.bin is corrupted. Some phones may have been loaded, though. Use option 6 to check what has been loaded.");
                fclose(fp);
                return head;
            }
            break;
        }
        if(head == NULL)
        {
            head = createEntry(phoneData);
            current = head;
        }
        else
        {
            current->next = createEntry(phoneData);
            current = current->next;
        }
    }
    fclose(fp);
    return head;
}

void listAllPhones(PhonesList *head)
{
    consoleLog(0, "List of phones (IDs and models):");
    if(head == NULL)
    {
        consoleLog(0, "No phones available.");
        return;
    }
    for(PhonesList *p = head; p != NULL; p = p->next) printf("\n\tID: %s, Model: %s", p->phoneData.ID, p->phoneData.model);
    printf("\n\n");
}

PhonesList* addPhone(PhonesList *head)
{
    PhonesList *current = head;
    Phone phoneData;
    unsigned hasSucceeded = 0;
    if(head != NULL) while(current->next != NULL) current = current->next;
    printf("\n\nPlease enter phone ID: ");
    scanf("%s", phoneData.ID);
    while(hasSucceeded == 0)
    {
        if(!isNumeric(phoneData.ID, strlen(phoneData.ID)))
        {
            consoleLog(3, "Invalid phone ID. IDs can contain only numbers.");
            printf("Please enter phone ID: ");
            scanf("%s", phoneData.ID);
            continue;
        }
        if(head == NULL) break;
        for(PhonesList *p = head; p != NULL; p = p->next, hasSucceeded++)
        {
            if(strcmp(phoneData.ID, p->phoneData.ID) == 0)
            {
                consoleLog(3, "The ID you have entered already exists. Please try again.");
                hasSucceeded = 0;
                printf("Please enter phone ID: ");
                scanf("%s", phoneData.ID);
                break;
            }
        }
    }
    printf("Please enter phone model (max 20 characters): ");
    fflush(stdin);
    gets(phoneData.model);
    fflush(stdin);
    printf("Please enter phone price: ");
    scanf("%lf", &phoneData.price);
    printf("Please enter phone quantity (enter 0 to cancel adding phone): ");
    scanf("%d", &phoneData.quantity);
    if(phoneData.quantity >= INT_MAX)
    {
        consoleLog(2, "You may have entered a too big number as quantity. Quantity will be set to the max value of 2 147 483 647");
        phoneData.quantity = INT_MAX;
    }
    while(phoneData.quantity < 0)
    {
        printf("Invalid phone quantity. Please enter phone quantity (enter 0 to cancel adding phone): ");
        scanf("%d", &phoneData.quantity);
    }
    if(phoneData.quantity)
    {
        if(head != NULL) current->next = createEntry(phoneData);
        else head = createEntry(phoneData);
    }
    writeToFile(head);
    printf("\n");
    return head;
}

void writeToFile(PhonesList *head)
{
    FILE *fp = fopen("phonedb.bin", "wb");
    if(fp == NULL)
    {
        consoleLog(2, "Cannot write to phone database file phonedb.bin.");
        return;
    }
    for(PhonesList *p = head; p != NULL; p = p->next)
    {
        if(fwrite(&p->phoneData, sizeof(Phone), 1, fp) != 1)
        {
            consoleLog(2, "Failed to write some of the data to the phone database file phonedb.bin. Whatever was written so far has been written successfuly.");
            fclose(fp);
            return;
        }
    }
    fclose(fp);
}

PhonesList* changePhoneQuantity(PhonesList *head)
{
    PhonesList *prev = head;
    Phone phoneData;
	short quantityToChange;
	if(head == NULL)
    {
        consoleLog(2, "No phones available.");
        return head;
    }
    printf("\n\nPlease enter ID of a phone to change its quantity: ");
    scanf("%s", phoneData.ID);
    for(PhonesList* p = head; p != NULL; prev = p, p = p->next)
    {
        if(strcmp(p->phoneData.ID, phoneData.ID) == 0)
        {
            printf("\nPlease enter quantity to add/remove for phone with ID %s \n(with - to remove and if quantity goes down to 0, phone will be removed): ", phoneData.ID);
            scanf("%d", &quantityToChange);
            quantityToChange += p->phoneData.quantity;
            if(quantityToChange == 0)
            {
                head = deletePhone(head, prev, p);
                writeToFile(head);
                consoleLog(0, "Successfully deleted phone from store.");
                return head;
            }
            else if(quantityToChange > 0)
            {
                p->phoneData.quantity = quantityToChange;
                writeToFile(head);
                consoleLog(0, "Successfully changed phone quantity.");
                return head;
            }
            else
            {
                consoleLog(3, "Invalid quantity, it seems like you have decided to remove too many phones or there may be another error.");
                return head;
            }
        }
    }
    consoleLog(3, "Could not find phone with the specified ID.");
    return head;
}

PhonesList* deletePhone(PhonesList *head, PhonesList *prev, PhonesList *current)
{
    if(current == head)
    {
        head = head->next;
        return head;
    }
    else if(head->next != NULL)
    {
        prev->next = current->next;
        free(current);
        return head;
    }
    free(head);
    head = NULL;
    return head;
}

void printExpensivePhones(PhonesList *head)
{
    double averagePrice = 0;
    unsigned long long phoneCount = 0;
    if(head == NULL)
    {
        consoleLog(2, "No phones available.");
        return;
    }
    for(PhonesList *p = head; p != NULL; p = p->next)
    {
        if((averagePrice + p->phoneData.quantity * p->phoneData.price) >= DBL_MAX || (averagePrice + p->phoneData.quantity * p->phoneData.price) < 0)
        {
            consoleLog(3, "You have too many phones in the store and that results in failure with operations. Please consider removing some.");
            return;
        }
        averagePrice += p->phoneData.quantity * p->phoneData.price;
        phoneCount += p->phoneData.quantity;
    }
    averagePrice /= (double) phoneCount;
    for(PhonesList *p = head; p != NULL; p = p->next)  if(p->phoneData.price > averagePrice) printPhone(p->phoneData);
    printf("\n");
}

void printPhone(Phone phoneData)
{
    printf("\n\tPhone #%s:\n\t\tModel: %s\n\t\tPrice: %.2lf\n\t\tQuantity: %d\n", phoneData.ID, phoneData.model, phoneData.price, phoneData.quantity);
}

void printPhoneById(PhonesList *head)
{
    Phone phoneData;
    if(head == NULL)
    {
        consoleLog(0, "No phones available.");
        return;
    }
    printf("\n\nPlease enter ID of a phone to print info about: ");
    scanf("%s", phoneData.ID);
    for(PhonesList *p = head; p != NULL; p = p->next)
    {
        if(strcmp(p->phoneData.ID, phoneData.ID) == 0)
        {
            printPhone(p->phoneData);
            printf("\n");
            return;
        }
    }
    consoleLog(3, "Could not find phone with the specified ID.");
}

void freePhonesList(PhonesList *head)
{
    PhonesList *p,*q;

    for(p = q = head; p != NULL; p = p->next, free(q), q = p);
}

int isNumeric(char *str, unsigned len)
{
    for(int i = 0; i < len; i++) if(str[i] < 48 || str[i] > 57) return 0;
    return 1;
}
