#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// gcc -m32 -O0 -fno-stack-protector -mpreferred-stack-boundary=2 -z execstack -o Stage2_Store_Breach.o Stage2_Store_Breach.c

typedef struct Customer_ {
  char name[10];
  int customerID;
} Customer;

typedef struct Admin_ {
  char adminName[10];
  int adminID;
} Admin;

typedef struct Store_ {
  char storeName[16];
  int currentWaiting;
  char explaination[32];
  Customer waitingLine[5];
  Admin admin;
} Store;

void initializeData(Store* store);
void prologue(Store* store);
void menu(Store* store);
void check_waiting(Store* store);
void register_waiting(Store* store);
void change_store_info(Store* store);
int can_access_system(Store* store);


void initializeData(Store* store) {
  char customer_names[4][10] = {"Alice", "Bob", "Charlie", "David"};

  strcpy(store->storeName, "Duzzonku_SSP");
  strcpy(store->explaination, "The world's best Duzzonku!!");
  strcpy(store->admin.adminName, "Admin");
  store->admin.adminID = 99999999;

  for (int i = 0; i < 4; i++) {
    strcpy(store->waitingLine[i].name, customer_names[i]);
    store->waitingLine[i].customerID = 20260000 + i;
  }
  store->currentWaiting = 4;
}

void menu(Store* store) {
  int choice;

  while (1) {
    printf("\n===== Waiting System =====\n");
    printf("1. Check Waiting\n");
    printf("2. Register Waiting\n");
    printf("3. Exit\n");
    printf("Choice: ");

    if (scanf("%d", &choice) != 1) {
      printf("Invalid input.\n");
      return;
    }
    getchar();

    switch (choice) {
      case 1:
        check_waiting(store);
        break;
      case 2:
        register_waiting(store);
        break;
      case 3:
        printf("Exiting program.\n");
        return;
      case 999:
        if (!can_access_system(store)) {
          printf("Access denied.\n");
          break;
        }

        printf("Access granted.\n");
        {
          change_store_info(store);
        }
        break;
      default:
        printf("Invalid choice.\n");
    }
  }
}

void check_waiting(Store* store) {
  printf("\nCurrent waiting line for %s:\n", store->storeName);
  for (int i = 0; i < store->currentWaiting; i++) {
    printf("%d. %s (ID: %d)\n", i + 1,
           store->waitingLine[i].name,
           store->waitingLine[i].customerID);
  }
}

void register_waiting(Store* store) {
  int idx = store->currentWaiting;
  char name[33];

  if (idx >= 5) {
    printf("\nSorry, the waiting line is full.\n");
    return;
  }

  store->waitingLine[idx].customerID = 20260000 + idx;

  printf("Enter your name: ");
  scanf("%32s", name);

  strcpy(store->waitingLine[idx].name, name);
  store->currentWaiting++;

  printf("\nYou have been registered in the waiting line.\n");

  return;
}

int can_access_system(Store* store) {
  int idx = store->currentWaiting - 1;

  if (idx < 0) {
    return 0;
  }

  if (store->waitingLine[idx].customerID == store->admin.adminID && store->waitingLine[idx].customerID < 20270000) {
    return 1;
  } else {
    return 0;
  }
}

void change_store_info(Store* store) {
  printf("Stack address: %p\n", store->explaination);
  
  printf("Enter new store's explaination: ");
  read(0, store->explaination, 200);

  printf("\nStore explaination for %s updated.\n", store->storeName);
  return;
}


void prologue(Store* store) {
  printf("--------------------------------------------------------------------------\n");
  printf("This time, Sunha visited Duzzonku_SSP after hearing Seungeun's recommendation.\n");
  printf("However, after the previous line-cutting incident, the store tightened its security.\n");
  printf("Only approved users can access its hidden management functions.\n\n");

  printf("Still, Sunha refuses to give up.\n");
  printf("Tampering with the system is the only way to reach those hidden functions.\n");
  printf("And to fully take control of it, gaining shell access is essential.\n");
  printf("--------------------------------------------------------------------------\n\n");
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