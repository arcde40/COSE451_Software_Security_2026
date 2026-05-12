#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// gcc -O0 -m32 -mpreferred-stack-boundary=2 -o Stage4_Shopping_Event.o Stage4_Shopping_Event.c -lpthread

#define MAX_QUEUE_SIZE 5
#define MAX_NAME_LEN 20
#define MAX_THREADS 4

typedef struct {
  int id;
  char name[MAX_NAME_LEN];
  unsigned int price;
} Product;

typedef struct {
  int product_count;
  unsigned int balance;
  Product purchased_products[MAX_QUEUE_SIZE];
  int id;
  char validate_string[MAX_NAME_LEN];
  unsigned int total_price;
} Cart;

void process();
void prologue();
void display_menu();
void init_cart();
void* display_cart();
void* purchase_product(void* arg);
void* resell_product(void* arg);
void* validate_SSP_event();
int is_VIP_wallet();

Cart myCart;

Product products[] = {{1, "Cloud_Mug", 200},
                      {2, "Daily_Bag", 500},
                      {3, "Travel_Bottle", 2000},
                      {4, "Cotton_Shirt", 5000},
                      {5, "SSP_Coin", 99999999}};

void process() { usleep(50000); }

void init_cart() {
  memset(&myCart, 0, sizeof(Cart));
  for (int i = 0; i < 4; i++) {
    myCart.purchased_products[i].id = products[2].id;
    strcpy(myCart.purchased_products[i].name, products[2].name);
    myCart.purchased_products[i].price = products[2].price;
  }
  myCart.product_count = 4;
  myCart.balance = 11000;
  myCart.id = 12345;
  strcpy(myCart.validate_string, "Normal");
  myCart.total_price = 8000;
}

void* display_cart() {
  process();
  printf("\n\n===== Current Cart =====\n");
  printf("Balance: %u\n", myCart.balance);
  if (myCart.product_count == 0) {
    printf("\n[!] No products in the cart.\n\n");
  } else {
    printf("Products in Cart: [%d]\n", myCart.product_count);
    for (int i = 0; i < myCart.product_count; i++) {
      printf(" - ID: %d, Name: %s, Price: %u\n",
             myCart.purchased_products[i].id, myCart.purchased_products[i].name,
             myCart.purchased_products[i].price);
    }
    printf("-------------------------------------\n");
  }

  printf("Total Price: %u\n", myCart.total_price);
  printf("===================================\n");
}

void* purchase_product(void* arg) {
  Product* prod = (Product*)arg;

  if (myCart.product_count < MAX_QUEUE_SIZE) {
    if (myCart.balance < prod->price) {
      printf("\n[!] Insufficient balance to add this product.\n\n");
      return NULL;
    } else {
      process();
      myCart.balance -= prod->price;
      myCart.total_price += prod->price;
      myCart.purchased_products[myCart.product_count].id = prod->id;
      strcpy(myCart.purchased_products[myCart.product_count].name, prod->name);
      myCart.purchased_products[myCart.product_count].price = prod->price;

      if (myCart.product_count < MAX_QUEUE_SIZE) {
        myCart.product_count++;
      }

      printf("\nPurchase successful! Remaining balance: %u\n\n",
             myCart.balance);
      return NULL;
    }
  }

  printf("\n[!] Cart is full! Cannot add more products.\n\n");

  return NULL;
}

void* resell_product(void* arg) {
  Product* prod = (Product*)arg;
  process();
  int found = -1;
  for (int i = 0; i < myCart.product_count; i++) {
    if (myCart.purchased_products[i].id == prod->id) {
      found = i;
      break;
    }
  }

  if (found != -1) {
    for (int i = found; i < myCart.product_count - 1; i++) {
      strcpy(myCart.purchased_products[i].name,
             myCart.purchased_products[i + 1].name);
      myCart.purchased_products[i].id = myCart.purchased_products[i + 1].id;
      myCart.purchased_products[i].price =
          myCart.purchased_products[i + 1].price;
    }
    myCart.balance += prod->price;
    myCart.total_price -= prod->price;
    myCart.product_count--;
  } else {
    printf("\n[!] Product not found in cart! Cannot resell.\n\n");
  }
  return NULL;
}

void* validate_SSP_event() {
  process();

  if (!is_VIP_wallet()) {
    printf("\n[!] VIP Wallet validation failed! Access denied.\n\n");
    return NULL;
  }

  for (int i = 0; i < myCart.product_count; i++) {
    if (myCart.purchased_products[i].id == 5) {
      printf(
          "\nCongratulations! You have successfully validated your SSP "
          "Coin!\n");
      printf(
          "You are now eligible for the special event prize: free flights for "
          "an entire year!\n");
      printf("Flag: ");
      system("/bin/cat flag.txt");
      return NULL;
    }
  }

  printf(
      "\n[!] SSP Coin not found in cart! Please purchase it to validate.\n\n");

  return NULL;
}

int is_VIP_wallet() {
  char vip_string[MAX_NAME_LEN] = "VIP_auth_pass!";
  int wallet_value = 0;
  int vip_value = 0;

  for (int i = 0; myCart.validate_string[i] != '\0'; i++) {
    wallet_value += (unsigned char)myCart.validate_string[i];
  }

  for (int i = 0; vip_string[i] != '\0'; i++) {
    vip_value += (unsigned char)vip_string[i];
  }

  if (wallet_value != vip_value) {
    return 0;
  }
  return 1;
}

void display_menu() {
  printf("\n\n===== Shopping Program =====\n");
  printf("\nPlease select an operation:\n");
  printf("1. View My Cart\n");
  printf("2. Purchase Product\n");
  printf("3. Re-Sell Product\n");
  printf("4. Event Validation\n");
  printf("5. Exit\n");
  printf("Choice: ");
}

void prologue() {
  printf(
      "One day, an online shopping mall launched a special event for its VIP "
      "customers.\n");
  printf(
      "To participate in the event, customers must first purchase an expensive "
      "SSP Coin and then verify that they actually own it.\n\n");
  printf(
      "Participants who successfully complete the verification process are "
      "eligible for a special first-come, first-served prize:\n");
  printf("free flights for an entire year.\n\n");
  printf(
      "However, the event is only available to VIP customers, and the SSP Coin "
      "is far too expensive.\n");
  printf(
      "As a regular customer, Seungeun cannot participate in the event through "
      "normal means.\n\n");
  printf("Still, Seungeun cannot give up on such an incredible prize.\n");
  printf(
      "How can Seungeun purchase an SSP Coin, pass the verification, and join "
      "the event?\n");
  printf(
      "-------------------------------------------------------------------"
      "\n\n");
}

int main() {
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);

  prologue();

  init_cart();

  printf("=== Welcome to the SSP Shopping Program! ===\n");

  display_cart();

  int choice = 0;
  int prod_choice = 0;
  pthread_t threads[MAX_THREADS];
  int thread_count = 0;

  while (1) {
    if (thread_count >= MAX_THREADS) {
      printf("Too many operations! Please wait for some to complete.\n");
      for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i], NULL);
      }
      thread_count = 0;
    }
    display_menu();
    scanf("%d", &choice);

    switch (choice) {
      case 1:
        pthread_create(&threads[thread_count++], NULL, &display_cart, NULL);
        break;
      case 2:
        printf("\n\n===== Available Products =====\n");
        for (int i = 0; i < 5; i++) {
          printf(" - ID: %d, Name: %s, Price: %u\n", products[i].id,
                 products[i].name, products[i].price);
        }
        printf("===================================\n");
        printf("\nEnter the product ID to purchase: ");
        scanf("%d", &prod_choice);
        if (prod_choice < 1 || prod_choice > 5) {
          printf("\n[!] Invalid product ID. Please try again.\n\n");
        } else {
          pthread_create(&threads[thread_count++], NULL, &purchase_product,
                         &products[prod_choice - 1]);
        }
        break;
      case 3:
        printf("\nEnter the Product Id to resell: ");
        scanf("%d", &prod_choice);
        if (prod_choice < 1 || prod_choice > 5) {
          printf("\n[!] Invalid product ID. Please try again.\n\n");
        } else {
          pthread_create(&threads[thread_count++], NULL, &resell_product,
                         &products[prod_choice - 1]);
        }
        break;
      case 4:
        pthread_create(&threads[thread_count++], NULL, &validate_SSP_event,
                       NULL);
        break;
      case 5:
        for (int i = 0; i < thread_count; i++) {
          pthread_join(threads[i], NULL);
        }
        printf("Goodbye!\n");
        return 0;
      default:
        printf("Invalid choice. Please try again.\n");
    }
  }

  return 0;
}
