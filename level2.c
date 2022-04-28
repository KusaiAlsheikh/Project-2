#include <stdbool.h>//this allowed me to use bool variables in login
#include <stdio.h>//this is a standard library 
#include <string.h>//the command strcmp is taken from here
#include <stdlib.h>

#pragma warning(disable : 4996)

typedef enum { // enum type for votes
	HASNT_VOTED = 0,
	YES,
	NO,
	ABSTAIN,
} voting_nu_t;

typedef struct {
	char username[50];
	char password[50];
	voting_nu_t voting_nu;
} user_t; // user struct that stores the username, password and vote result

typedef struct {
	user_t* users;
	size_t count;
} List;

int currentUser = -1;//this variable is used to assign a vote for the user currently logged in

bool user_loading(const char* fileName, List* u_list) {
	if (u_list->users) // freeing up user list if not empty
		free(u_list->users);
	u_list->count = 0;

	FILE* file;
	errno_t res;

	if (res = fopen_s(&file, fileName, "r")) {
		fprintf(stderr, "Cannot open file. Error no: %d\n", res);
		return 0;
	}

	do {
		user_t buffer = { "", "", HASNT_VOTED }; // temporary buffer to store user data
		int n;
		if ((n = fscanf(file, "%s%s%d", buffer.username, buffer.password, &buffer.voting_nu))==3) {
			if (u_list->users = realloc(u_list->users, ++u_list->count * sizeof(user_t))); // reallocting user list to a bigger size to store the new user
			memcpy(&u_list->users[u_list->count - 1], &buffer, sizeof(user_t)); // copying buffer to the latest allocted user
		}
		else if (n != EOF) {
			fputs("Couldn't find a match.\n", stderr);
			fclose(file);
			return 0;
		}
		else
			break;
	} while (1);

	fclose(file);

	return 1;
}


bool user_s(const char* fileName, List* u_list) { // function to save user data such as usernames, passwords and votes
	FILE* file; // file pointer
	errno_t res; // errno for debugging

	if (res = fopen_s(&file, fileName, "w")) {
		fprintf(stderr, "Cannot open file. Error no: %d\n", res); // write error to stderr
		return EXIT_FAILURE;
	}

	for (size_t i = 0; i < u_list->count; i++) {
		if (fprintf(file, "%s %s %d\n", u_list->users[i].username, u_list->users[i].password, u_list->users[i].voting_nu) < 0) { // writing formatted user data to the file
			fputs("Failed to write the user data to the file.\n", stderr);
			fclose(file);
			return 0;
		}
	}
	fclose(file);
	return 1;
}

void voting_b(List* user_list, int* yes, int* no, int* abstain) { // function to build vote sums from users
	*yes = 0;
	*no = 0;
	*abstain = 0;
	for (size_t i = 0; i < user_list->count; i++) {

		if (user_list->users[i].voting_nu == NO) {
			(*no)++;
		}
		else if (user_list->users[i].voting_nu == YES) {
			(*yes)++;
		}
		else if (user_list->users[i].voting_nu == ABSTAIN) {
			(*abstain)++;
		}
	}
}

bool login(List* list) {//this function is bool, it returns the value of true when a username and password match,or false when a match is not found.
	user_t buffer = { "", "" };//A buffer is defined to saved inputs using struct
	printf("Please enter a username: ");
	scanf_s("%s", &buffer.username, sizeof(buffer.username));//here an input is asked and saved to buffer struct username
	printf("Please enter a password: ");
	scanf_s("%s", &buffer.password, sizeof(buffer.password)); //here the password is asked for and saved into buffer struct password
	for (size_t i = 0; i < list->count; i++) {
		if (strcmp(buffer.username, list->users[i].username) == 0 && strcmp(buffer.password, list->users[i].password) == 0) {//here the values saved into buffer are compared to the array we defined earlier
			currentUser = i;
			return 1;//if username and password are both correct login is given as true
		}
	}
	printf("User not found, username or pcassword incorrect.\n");
	return 0;//if username or password are incorrect login returns as false
}

int main() {
	const char* fileName = "UserInfo.txt"; // name of the file that stores user data
	List u_L = { NULL, 0 };

	user_loading(fileName, &u_L); // loading users from file

	if (!u_L.count) {
		fputs("No user found.\n", stderr);
		return EXIT_FAILURE;
	}

	int yes = 0, no = 0, abstain = 0;
	voting_b(&u_L, &yes, &no, &abstain);

	while (yes < ((u_L.count - abstain) / 2 + 1) && no < ((u_L.count - abstain) / 2 + 1) && yes + no != u_L.count - abstain) { // checking whether the voting has ended or the result is obvious
		// pointer to the logged in user for easier access
		login(&u_L);

		while (currentUser != -1) { // looping until user logs out
			puts("1- Change your password");
			puts("2- Vote");
			puts("3- Log out");

			printf("Select one of these actions above: ");
			int action = 0;
			int i;
			scanf_s("%d", &action);

			switch (action) {
			case 1:
				printf("Enter your new password: "); // requesting new password
				scanf_s("%31s", u_L.users[currentUser].password, 32);
				if (user_s(fileName, &u_L)) // saving new password with users
					puts("Your password has been successfully changed.\n");
				break;
			case 2:
				u_L.users[currentUser].voting_nu = HASNT_VOTED;
				while (u_L.users[currentUser].voting_nu == HASNT_VOTED) { // looping until valid input
					puts("\n1- Yes");
					puts("2- No");
					puts("3- Abstain");
					printf("Vote for ending the class: "); // requesting vote
					scanf_s("%d", &u_L.users[currentUser].voting_nu);
					if (u_L.users[currentUser].voting_nu > ABSTAIN || u_L.users[currentUser].voting_nu < YES) {
						u_L.users[currentUser].voting_nu = HASNT_VOTED;
						puts("Please select a valid option.\n");
					}
					else
						break;
				}
				user_s(fileName, &u_L); // saving votes with users
				puts("Your vote has been saved.\n");
				break;
			case 3:
				currentUser = -1;
				break;
			}
			voting_b(&u_L, &yes, &no, &abstain);
		}
	}

	if (yes + no != u_L.count - abstain) // checking if the result is obvious
		puts("Result is obvious..");

	printf("Yes: %d\nNo: %d\nAbsent: %d\n", yes, no, abstain); // printing results
	printf("Result for the vote: ");

	if (yes == no) {
		printf("DRAW");
	}
	else if (yes > no) {
		printf("Yes was the winning vote");
	}
	else {
		printf("No was the winning vote");
	}printf("\n");

	for (size_t i = 0; i < u_L.count; i++) // resetting votes to be able to vote again on start
		u_L.users[i].voting_nu = HASNT_VOTED;

	user_s(fileName, &u_L); // saving reseted votes

	free(u_L.users);

	return 0;
}