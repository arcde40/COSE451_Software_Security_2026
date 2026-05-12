#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

// gcc -O0 -m32 -fno-stack-protector -z execstack -mpreferred-stack-boundary=2 -o Stage3_Leaked_Simulator.o Stage3_Leaked_Simulator.c

#define MAX_LENGTH 200

char flag[50];
char user_id[50];
char current_log[MAX_LENGTH];
unsigned int access_level;
unsigned int secret_token;

char * get_flag(){
  FILE *file = fopen("flag.txt", "r");
  if (file == NULL) {
    printf("Failed to open flag.txt\n");
    exit(1);
  }

  if (fgets(flag, 50, file) == NULL) {
    printf("Failed to read flag\n");
    exit(1);
  }

  fclose(file);
  return flag;
}

void generate_log_header() {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    char timestamp[40];
    strftime(timestamp, 40, "%Y-%m-%d(%a) %H:%M:%S", tm_info);

    snprintf(current_log, MAX_LENGTH,
             "[%s] [%s] Log entry: ",
             timestamp, user_id);
}

void create_log(char *input) {
    generate_log_header();
    char log_buf[32];
    strncpy(log_buf, input, 31);
    strcat(current_log, input);
    printf(current_log);
    printf("\n");
}

void view_sample_logs() {
    printf("\n===== Sample Logs =====\n");
    printf("[2026-04-20(Mon) 11:58:12] [seungeun] Log entry: Accessed Duzzonku_SSP interface\n");
    printf("[2026-04-22(Wed) 21:13:12] [sunha] Log entry: Accessed Duzzonku_SSP interface\n");
    printf("[2026-04-22(Wed) 21:15:33] [sunha] Log entry: Requested information about restricted section\n");
    printf("[2026-04-22(Wed) 21:16:33] [system] Log entry: Warning - Unusual query pattern detected\n");
    printf("[2026-04-22(Wed) 21:21:10] [sunha] Log entry: Connection terminated unexpectedly\n");
    printf("=======================\n\n");
}

void simulate_log() {
    char input[MAX_LENGTH];
    
    printf("Enter log content to simulate: ");
    read(0, input, MAX_LENGTH-64);

    create_log(input);
}

void admin_panel() {
    if (access_level < 10) {
        printf("[ERROR] Insufficient privileges to access admin panel.\n");
        return;
    }

    printf("[ADMIN] Debug information:\n");
    printf("[ADMIN] Flag address: %p\n", flag);
    printf("[ADMIN] Secret token: 0x%x\n", secret_token);
}

void delete_logs() {
    if (secret_token == 0x12345 && access_level >= 10) {
        printf("[SYSTEM] Deleting logs...\n");
        sleep(1);
        printf("[system] \"All logs have been deleted.\"\n");
        sleep(1);
        get_flag();
        printf("[FLAG] %s\n", flag);
    } else {
        printf("[ERROR] Unauthorized access. This incident will be reported.\n");
        printf("[DEBUG] Current level: %d\n", access_level);
        printf("[DEBUG] Current token: 0x%x\n", secret_token);
    }
}

void display_menu() {
  printf("\n===== Duzzonku Audit Simulator =====\n");
  printf("1. View Sample Logs\n");
  printf("2. Simulate Custom Log\n");
  printf("3. Admin Panel\n");
  printf("4. Delete Logs\n");
  printf("5. Exit\n");
  printf("Choice: ");
}

void prologue(){
    printf("-------------------------------------------------------------------\n");
    printf("                        LOG SIMULATOR v1.0                         \n");
    printf("-------------------------------------------------------------------\n");
    printf("Duzzonku_SSP was forced to audit every hidden record after the incident.\n");
    printf("For the perfect crime, Sunha attempted to erase the logs.\n");
    printf("However, the logs were managed by a separate protected system.\n");
    printf("This simulator was developed to help reconstruct them in an emergency,\n");
    printf("but was accidentally leaked to an unauthorized channel.\n");
    printf("This system might be the only way to finally make the traces disappear...\n");
    printf("-------------------------------------------------------------------\n\n");
}


int main() {
  int choice;
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);

  prologue();

  printf("Enter your user ID: ");
  scanf("%s", user_id);
  printf("Welcome, %s. Your default access level is 1.\n", user_id);
  access_level = 1;
  secret_token = 0xbadc0de;


  while (1) {
    display_menu();
    scanf("%d", &choice);

    switch (choice) {
      case 1:
        view_sample_logs();
        break;
      case 2:
        simulate_log();
        break;
      case 3:
        admin_panel();
        break;
      case 4:
        delete_logs();
        break;
      case 5:
        printf("Exiting...\n");
        return 0;
      default:
        printf("[ERROR] Invalid choice.\n");
    }
  }

  return 0;
}
