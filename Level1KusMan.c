#include <stdbool.h>//this allowed me to use bool variables in login
#include <stdio.h>//this is a standard library 
#include <string.h>//the command strcmp is taken from here

typedef struct {
    char username[50];
    char password[50];// 100 byte
    bool voted;
} user_t;
//above struct is user to define what a user is, it also defines the parameters of user where username consists of 50 characters and password consists of 50 too

user_t users[4] = { { "User1", "pwd1" }, { "User2", "pwd2" }, {"User3", "pwd3"}, {"User4", "pwd4" } }; // here using the struct defined before we create "array" of user_t
int currentUser;//this variable is used to assign a vote for the user currently logged in

bool login() {//this function is bool, it returns the value of true when a username and password match,or false when a match is not found.
    user_t buffer = { "", "" };//A buffer is defined to saved inputs using struct
    printf("Please enter a username: ");
    scanf_s("%s", &buffer.username, sizeof(buffer.username));//here an input is asked and saved to buffer struct username
    printf("Please enter a password: ");
    scanf_s("%s", &buffer.password, sizeof(buffer.password)); //here the password is asked for and saved into buffer struct password
    for (size_t i = 0; i < (sizeof(users) / sizeof(users[0])); i++) {
        if (strcmp(buffer.username, users[i].username) == 0 && strcmp(buffer.password, users[i].password) == 0) {//here the values saved into buffer are compared to the array we defined earlier
            currentUser = i;//when the user is signed in current user is defined to them
            return 1;//if username and password are both correct login is given as true
        }
    }
    printf("User not found, username or pcassword incorrect.\n");
    return 0;//if username or password are incorrect login returns as false
}
int yes = 0;
int no = 0;//int yes and int no are used to save votes

void vote() {//this function edits the global variables yes and no.
    if (users[currentUser].voted) {
        printf("user already voted\n");
        return;//this checks if the user signed in has already voted to avoid unfairness
    }
    char voter[4] = "";//this is used to as a saver for votes taken from user
    printf("Do you want free coffee (YES/NO): \n");//here the user is asked to vote 
    scanf_s("%s", voter, sizeof(voter));
    users[currentUser].voted = 1;//this defines the user as a voted that means he cant vote again
    if (!strcmp(voter, "YES")) {
        yes++;//this adds to the yes votes and checks if he input form is valid
    }
    else if (!strcmp(voter, "NO")) {
        no++;//this adds to the no votes and if the users input is valid
    }
    else {
        printf("Invalid input please type either YES or NO\n");
        users[currentUser].voted = 0;//if the users input is invalid he will still be able to vote if he signs in again
    }
}


int main() {

    while (yes + no < 4) {//this loop will allow at most 4 users to login and vote
        if (login() == 1)
            vote();//if the user inputs a currect username and password in the login function that's called in the pervious line he will be able to vote
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
    }//the functions above checks the finalvote from users using majority. since the number of users is 4 the possible out comes are either a draw: 2,2 or 3,1 1,3 or 4,0 0,4 the last two are insignificant if either yes or no reach 3 the vote should end
}