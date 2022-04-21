#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char username[50];
    char password[50];// 100 byte
    bool voted;
} user_t;


user_t users[4] = { { "User1", "pwd1" }, { "User2", "pwd2" }, {"User3", "pwd3"}, {"User4", "pwd4" } }; // 400 byte , 400 / 100 = 4
int currentUser;

bool login() {
    user_t buffer = { "", "" };
    printf("Please enter a username: ");
    scanf_s("%s", &buffer.username, sizeof(buffer.username));
    printf("Please enter a password: ");
    scanf_s("%s", &buffer.password,sizeof(buffer.password)); //unsigned long long = size_t
    for (size_t i = 0; i < (sizeof(users) / sizeof(users[0])); i++) {
        if (strcmp(buffer.username, users[i].username) == 0 && strcmp(buffer.password, users[i].password) == 0) {
            currentUser = i;
            return 1;
        }
    }
    printf("User not found, username or password incorrect.\n");
    return 0;
}
int yes = 0;
int no = 0;

void vote() {
    if (users[currentUser].voted) {
        printf("user already voted\n");
        return;
    }
    char voter[4] = "";
    printf("Do you want free coffee (YES/NO): \n");
    scanf_s("%s", voter, sizeof(voter));
    users[currentUser].voted = 1;
    if (!strcmp(voter, "YES")) {
        yes++;
    }
    else if (!strcmp(voter, "NO")) {
        no++;
    }
    else {
        printf("Invalid input please type either YES or NO\n");
        users[currentUser].voted = 0;
    }
}


int main() {

    while (yes + no < 4) {
        if (login() == 1)
            vote();
        if (yes == 3) {
            printf("Yes we want free coffee\n");
            break;
        }
        else if (no == 3) {
            printf("No worries we can buy our own\n");
            break;
        }
    }if (yes == no) {
        printf("We had a draw");
    }
}
