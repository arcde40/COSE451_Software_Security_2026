#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// gcc -m32 -O0 -fno-stack-protector -mpreferred-stack-boundary=2 -z noexecstack
// -o Stage1_Line_Cutting.o Stage1_Line_Cutting.c

typedef struct Customer_ {
  char name[16];
  char rank[16];
  int customerID;
} Customer;

typedef struct Store_ {
  char storeName[16];
  int currentWaiting;
  char explaination[32];
  Customer waitingLine[10];
} Store;

void initializeData(Store* store);
void prologue(Store* store);
void menu(Store* store);
void check_waiting(Store* store);
void register_waiting(Store* store);
int sell_duzzonku(Store* store);
int is_valid_VIP(Customer customer);
void change_store_info(Store* store);
void get_flag();

void initializeData(Store* store) {
  char customer_names[9][16] = {"Alice", "Bob",   "Charlie", "David", "Eve",
                                "Frank", "Grace", "Heidi",   "Ivan"};

  strcpy(store->storeName, "Duzzonku_SSP");
  for (int i = 0; i < 9; i++) {
    strcpy(store->waitingLine[i].name, customer_names[i]);

    if (i == 0) {
      store->waitingLine[i].customerID = 20269000 + i;
      strcpy(store->waitingLine[i].rank, "VIP");
    } else {
      store->waitingLine[i].customerID = 20260000 + i;
      strcpy(store->waitingLine[i].rank, "Normal");
    }
  }
  store->currentWaiting = 9;
}

void menu(Store* store) {
  int choice;

  while (1) {
    printf("\n===== Waiting System =====\n");
    printf("1. Check Waiting\n");
    printf("2. Register Waiting\n");
    printf("3. Purchase\n");
    printf("4. Exit\n");
    printf("Choice: ");

    if (scanf("%d", &choice) != 1) {
      printf("Invalid input.\n");
      return;
    }

    switch (choice) {
      case 1:
        check_waiting(store);
        break;
      case 2:
        register_waiting(store);
        break;
      case 3:
        if (sell_duzzonku(store))
          break;
        else
          return;
      case 4:
        printf("Exiting program.\n");
        exit(0);
      case 999:
        change_store_info(store);
        break;
      default:
        printf("Invalid choice.\n");
    }
  }
}

void check_waiting(Store* store) {
  printf("\nCurrent waiting line for %s:\n", store->storeName);
  for (int i = 0; i < store->currentWaiting; i++) {
    printf("%d. %s (Rank: %s)\n", i + 1, store->waitingLine[i].name,
           store->waitingLine[i].rank);
  }
}

void register_waiting(Store* store) {
  int idx = store->currentWaiting;
  char name[40];

  if (idx >= 10) {
    printf("\nSorry, the waiting line is full.\n");
    return;
  }

  store->waitingLine[idx].customerID = 20260000 + idx;
  strcpy(store->waitingLine[idx].rank, "Normal");

  printf("Enter your name: ");
  scanf("%39s", name);

  strcpy(store->waitingLine[idx].name, name);
  store->currentWaiting++;

  printf("\nYou have been registered in the waiting line.\n");

  return;
}

int sell_duzzonku(Store* store) {
  for (int i = 0; i < store->currentWaiting; i++) {
    if (is_valid_VIP(store->waitingLine[i])) {
      printf("\nWelcome VIP! Thank you for your visit.\n");
      if (i == store->currentWaiting - 1) {
        printf("\n----------------------------------------------\n");
        printf("Congratulations! He succeeded in pretending to be a VIP!\n");
        printf("Here is the last duzzonku.\n");
        get_flag();
        printf("\n----------------------------------------------\n");
      } else
        break;
    }
  }

  printf("\nToday's last duzzonku has been sold out.\n");
  printf("Please wait for tomorrow's stock.\n");
  return 0;
}

int is_valid_VIP(Customer customer) {
  char vip[3] = "VIP";

  for (int i = 0; i < 3; i++) {
    if (customer.rank[i] != vip[i]) {
      return 0;
    }
  }

  printf("%d\n", customer.customerID);

  if (customer.customerID >= 20269000 && customer.customerID < 20270000) {
    return 1;
  } else {
    return 0;
  }
}

void change_store_info(Store* store) {
  char newExplaination[72];

  printf("Enter new store's explaination: ");
  scanf("%71s", newExplaination);

  strcpy(store->explaination, newExplaination);

  printf("\nStore explaination for %s updated.\n", store->storeName);

  return;
}

void get_flag() {
  FILE* file = fopen("flag.txt", "r");
  if (!file) {
    printf("Unable to open flag file.\n");
    return;
  }

  char flag[100];
  if (fgets(flag, sizeof(flag), file) != NULL) {
    printf("\nFLAG: %s\n", flag);
  } else {
    printf("Failed to read FLAG.\n");
  }

  fclose(file);
}

void prologue(Store* store) {
  printf("----------------------------------------------\n");
  printf(
      "One day, Seungeun really wanted to eat duzzonku, so he got in line at ");
  printf("Duzzonku_SSP, the most famous duzzonku store in town.\n");
  printf("Currently, there are already 9 customers waiting in line.\n");
  printf("But there is only one duzzonku left for today.\n");
  printf("Could he find a way to cut the line and get served first?\n");
  printf("----------------------------------------------\n\n");
}

int main() {
  Store store;

  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stdin, NULL, _IONBF, 0);

  initializeData(&store);
  prologue(&store);
  menu(&store);

  return 0;
}