#include"stdio.h"
#include"conio.h"
#include"stdbool.h"
#include"stdlib.h"
#include"Windows.h"
#include"serverUtils.h"

void clear();
void wait();
void main_screen();
void draw_title();
void login();
void signup();
bool validate_username(char *username);
bool validate_password(char *password);
void user_screen(int userid);
void view_history(int userid);
void book_tickets(int userid);
void changecolor(char fgcolor, char bgcolor);
void show_availability(int bid, char*date, char*available_seats, int userid, int no_of_seats);
void payment_screen(int userid);
void reset_colors();


void clear()
{
	system("cls");
}

void wait(const char *message)
{
	fflush(stdin);
	printf("\n\n!%s\n", message);
	printf("Enter any key to continue: ");
	system("pause");
	fflush(stdin);
}

void main_screen(){
	int choice;
	draw_title();
	printf("1. Login\n");
	printf("2. Signup\n");
	printf("3. Exit\n\nEnter your choice: ");
	scanf("%d",&choice);
    
	if (choice == 1) login();
	else if (choice == 2) signup();
	else if (choice == 3) { close(); exit(0); }
	else wait("Invalid Choice");
}

void login(){
	draw_title();
	char username[22], password[20];


	printf("username: ");
	//reading username
	scanf("%s", username);
	fflush(stdin);

	
	printf("password: ");
	scanf("%s", password);
	fflush(stdin);

	int userid=login_request(username, password);
	if ( userid == -2) {
		wait("Username doesn't exist");
		login();
	}
	else if (userid == -1) {
		wait("Wrong password");
		login();
	}
	else
		user_screen(userid);

	free(password);
	user_screen(7);
}

void signup(){
	draw_title();

	char username[22], password[20];
	
	printf("username: ");
	//reading username
	scanf("%s", username);
	fflush(stdin);

	bool is_valid_username = validate_username(username);
	
	if (!is_valid_username) {
		wait("Invalid username");
		signup();
	}

	printf("password: ");
	scanf("%s", password);
	fflush(stdin);

	bool is_valid_password = validate_password(password);

	if (!is_valid_password) {
		wait("Invalid password");
		signup();
	}
	
	if (register_request(username, password) == -1)
		wait("Username already taken!");
	else
		wait("Successfully registered");
	main_screen();
}

void draw_title(){
	clear();
	reset_colors();
	printf("\t\t\t\t ************************************* \n");
	printf("\t\t\t\t*************** RED BUS ***************\n");
	printf("\t\t\t\t ************************************* \n\n\n");
}

bool validate_username(char *username){
	for (int i = 0; username[i] != '\0'; i++) {
		if (i > 18)
			return false;
		if (username[i] == ' ' || username[i] == '\t')
			return false;
	}

	return true;
}

bool validate_password(char *password) {
	int i = 0;
	for (i = 0; password[i] != '\0' ; i++);
	return (i >= 5) && (i <= 18);
}


void user_screen(int userid){
	int choice;
	draw_title();
	printf("1. Book Ticket\n");
	printf("2. View History\n");
	printf("3. Logout\n\nEnter your choice: ");
	scanf("%d", &choice);

	if (choice == 1) book_tickets(userid);
	else if (choice == 2) view_history(userid);
	else if (choice == 3) main_screen();
	else wait("Invalid Choice");
}

void book_tickets(int userid){
	draw_title();
	char source[20], dest[20], date[11];
	int no_of_seats = 0;
	printf(" Source: ");
	scanf("%s",source);
	printf(" Destination: ");
	scanf("%s",dest);
	printf(" Date(dd/mm/yyyy): ");
	scanf("%s",date);
	printf(" No of seats: ");
	scanf("%d", &no_of_seats);
	draw_title();
	Bus_response *available_buses = get_buses(source,dest,date);
	if (available_buses == NULL)
		printf("No Routes Found!");
	else
	{
		printf("BusId\tType\tTime\tDate\tPrice\n");
		char div[16] = "---------------";
		for (int i = 0; i<available_buses->length; i++)
		{
			printf("%d\t%s\t%s\t%s\t%d\n", available_buses->bus_details[i]->bid, available_buses->bus_details[i]->type, available_buses->bus_details[i]->time, available_buses->bus_details[i]->date, available_buses->bus_details[i]->price);
		}
		printf("\n Enter bus id:");
		int bid;
		scanf("%d", &bid);
		clear();

		int available_count = 0;
		char *available_seats = get_seats_server(bid, date);
		if (available_seats == NULL)
			printf("\nNo seats found!\n");
		else
		{
			for (int i = 0; available_seats[i] != '\0'; i++)
				available_count += (available_seats[i] == '1');

			if (available_count < no_of_seats) {
				wait("Bus has less number of available seats");
				book_tickets(userid);
			}
			show_availability(bid, date, available_seats, userid, no_of_seats);
		}
	}
	user_screen(userid);
}
void changecolor(char fgcolor, char bgcolor)
{
	int col;
	if (fgcolor == 'g' && bgcolor == 'g')
	{
		col = 10 + 10 * 16;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		SetConsoleTextAttribute(hConsole, col);
	}
	if (fgcolor == 'r' && bgcolor == 'r')
	{
		col = 12 + 12 * 16;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		SetConsoleTextAttribute(hConsole, col);
	}
	if (fgcolor == 'w' && bgcolor == 'w')
	{
		col = 15 + 15 * 16;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		SetConsoleTextAttribute(hConsole, col);
	}
	if (fgcolor == 'r' && bgcolor == 'w')
	{
		col = 12 + 15 * 16;
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		SetConsoleTextAttribute(hConsole, col);
	}



}
void show_availability(int bid, char*date, char*available_seats, int userid, int no_of_seats)
{
	draw_title();

	int c = 0, seatno = 0;
	for (int i = 0; i<10; i++)
	{
		printf("\t\t\t\t");
		for (int j = 0; j < 2; j++)
		{
			if (available_seats[c++] == '0')
			{
				changecolor('r', 'r');
				printf("hi");
			}
			else
			{
				changecolor('g', 'g');
				printf("hi");
			}
			changecolor('w', 'w');
			printf("h");
		}
		changecolor('w', 'w');
		printf("\n");
		changecolor('r', 'w');
		printf("\t\t\t\t%d    %d\n", seatno + 1,seatno + 2);
		seatno += 2;
	}
	
	printf("Enter the seat numbers:\n");
	int max_seats = strlen(available_seats);
	int *seats=(int *)malloc(no_of_seats * sizeof(int));
	for (int i = 0; i < no_of_seats; i++)  {
		printf("seat %d no: ", i + 1);
		scanf("%d", &seats[i]);

		if (seats[i] > max_seats || available_seats[seats[i] - 1] == '0') {
			wait("Seat already booked! enter another seat number");
			i--;
		}
		else
			available_seats[seats[i] - 1] = '0';
	}

	
	book_ticket(userid, bid, date,no_of_seats,seats);
	payment_screen(userid);
	free(seats);
}

void payment_screen(int userid) {
	int c_num, cvv_num;
	draw_title();
	printf("Enter card number: ");
	scanf(" %d",&c_num);
	printf("Enter cvv number: ");
	scanf(" %d", &cvv_num);
	wait("Your payment was succeessfull.\nEnjoy the ride.......");
	user_screen(userid);
}

void reset_colors()
{
	system("color FC");
}

void view_history(int userid){
	draw_title();
	Ticket_response* response = get_tickets(userid);
	if (response == NULL)
		printf("\nNo tickets found!\n");
	else
	{
		printf("BusId\tDate\t\tTicketId\tTime\n");
		printf("------------------------------------------------------------\n\n");
		for (int i = 0; i < response->length; i++)
		{
			printf("%d\t%s\t%d\t%s\n", response->ticket_details[i]->bid, response->ticket_details[i]->date, response->ticket_details[i]->tid, response->ticket_details[i]->time);
		}
	}
	wait("");
	user_screen(userid);
}
