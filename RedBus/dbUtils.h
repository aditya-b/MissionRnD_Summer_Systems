#include<stdio.h>
#include<stdlib.h>
#include<string.h>


//User struct
struct user
{
	int uid;
	char *uname, *password;
};
struct user **user_details;

//Bus struct
struct bus
{
	int bid, price;
	char *time, *source, *destination, *type, *date;
};
struct bus **bus_attributes;

//Seat struct
struct seats
{
	int bid;
	char *date, *seat_string;
	int total_seats;
};
struct seats **seat_details;

//Tickets Struct
struct tickets
{
	int tid, uid, bid;
	char *uname, *date, *time;
};
struct tickets **ticket_details;

//Bus Response struct
struct Bus_response
{
	int length;
	struct bus **bus_details;
}*bus_response;

//Ticket Response struct
struct Ticket_response
{
	int length;
	struct tickets **ticket_details;
}*ticket_response;

int user_count = 0, bus_count = 0, seats_count = 0, tickets_count = 0;

/*
imp0rting data int0 structs
*/
//No.of lines in csv
int countLines(const char *filename) {
	FILE *fp;
	fp = fopen(filename, "r");
	if (fp == NULL)
		return -1;
	char c;
	int count = 0;
	while (c = fgetc(fp)) {
		if (c == EOF) {
			count++;
			break;
		}
		else if (c == '\n')
			count++;
	}
	return count;
}

//Tokenizing based ',' into arrays of strings
char **tokenize(char *line) {
	char **tokens = (char **)malloc(sizeof(char*) * 10);
	for (int e = 0; e < 10; e++)
		tokens[e] = (char *)malloc(1024);
	int i = 0, c = 0;
	while (*line != '\0') {
		if (*line == ',' || *line == '\n') {
			tokens[c++][i] = '\0';
			i = 0;
		}
		else
			tokens[c][i++] = *line;
		line++;
	}
	tokens[c][i] = '\0';
	return tokens;
}

//Convert string into int
int strToInt(char *s) {
	int n = 0;
	while (*s != '\0') {
		if (*s<'0' || *s>'9')
			return -1;
		n = n * 10 + *s - '0';
		s++;
	}
	return n;
}

//Importing into structs(user,bus..)
void importData() {
	int l;
	FILE *fp;
	char record[1024], **tokens;
	l = countLines("user.csv");
	user_details = (struct user **)malloc(sizeof(struct user*)*l);
	fp = fopen("user.csv", "r");
	if (fp == NULL || l == -1)
	{
		return;
	}
	while (fgets(record, 1024, fp)) {
		user_details[user_count] = (struct user*)malloc(sizeof(struct user));
		tokens = tokenize(record);
		user_details[user_count]->uid = strToInt(tokens[0]);
		user_details[user_count]->uname = tokens[1];
		user_details[user_count]->password = tokens[2];
		user_count++;
	}
	fclose(fp);

	l = countLines("bus.csv");
	bus_attributes = (struct bus **)malloc(sizeof(struct bus*)*l);
	fp = fopen("bus.csv", "r");
	if (fp == NULL || l == -1)
	{
		return;
	}
	while (fgets(record, 1024, fp)) {
		bus_attributes[bus_count] = (struct bus*)malloc(sizeof(struct bus));
		tokens = tokenize(record);
		bus_attributes[bus_count]->bid = strToInt(tokens[0]);
		bus_attributes[bus_count]->time = tokens[1];
		bus_attributes[bus_count]->source = tokens[2];
		bus_attributes[bus_count]->destination = tokens[3];
		bus_attributes[bus_count]->type = tokens[4];
		bus_attributes[bus_count]->price = strToInt(tokens[5]);
		bus_attributes[bus_count]->date = tokens[6];
		bus_count++;
	}
	fclose(fp);

	l = countLines("tickets.csv");
	ticket_details = (struct tickets **)malloc(sizeof(struct tickets*)*l);
	fp = fopen("tickets.csv", "r");
	if (fp == NULL || l == -1)
	{
		return;
	}
	while (fgets(record, 1024, fp)) {
		ticket_details[tickets_count] = (struct tickets*)malloc(sizeof(tickets));
		tokens = tokenize(record);
		ticket_details[tickets_count]->tid = strToInt(tokens[0]);
		ticket_details[tickets_count]->uid = strToInt(tokens[1]);
		ticket_details[tickets_count]->bid = strToInt(tokens[2]);
		ticket_details[tickets_count]->uname = tokens[3];
		ticket_details[tickets_count]->time = tokens[4];
		ticket_details[tickets_count]->date = tokens[5];
		tickets_count++;
	}
	fclose(fp);

	l = countLines("seats.csv");
	seat_details = (struct seats **)malloc(sizeof(struct seats*)*l);
	fp = fopen("seats.csv", "r");
	if (fp == NULL || l == -1)
	{
		return;
	}
	while (fgets(record, 1024, fp)) {
		seat_details[seats_count] = (struct seats*)malloc(sizeof(seats));
		tokens = tokenize(record);
		seat_details[seats_count]->bid = strToInt(tokens[0]);
		seat_details[seats_count]->date = tokens[1];
		seat_details[seats_count]->seat_string = tokens[2];
		seat_details[seats_count]->total_seats = strToInt(tokens[3]);
		seats_count++;
	}
	fclose(fp);
}




//overwriting the files
void flush()
{
	FILE *fp;
	fp = fopen("user.csv", "w");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < user_count; i++)
	{
		fprintf(fp, "%d,%s,%s\n", user_details[i]->uid, user_details[i]->uname, user_details[i]->password);
	}
	fclose(fp);
	fp = fopen("bus.csv", "w");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < bus_count; i++)
	{
		fprintf(fp, "%d,%s,%s,%s,%s,%d,%s\n", bus_attributes[i]->bid, bus_attributes[i]->time, bus_attributes[i]->source, bus_attributes[i]->destination, bus_attributes[i]->type, bus_attributes[i]->price, bus_attributes[i]->date);
	}
	fclose(fp);
	fp = fopen("seats.csv", "w");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < seats_count; i++)
	{
		fprintf(fp, "%d,%s,%s,%d\n", seat_details[i]->bid, seat_details[i]->date, seat_details[i]->seat_string, seat_details[i]->total_seats);
	}
	fclose(fp);
	fp = fopen("tickets.csv", "w");
	if (fp == NULL)
	{
		return;
	}
	for (int i = 0; i < tickets_count; i++)
	{
		fprintf(fp, "%d,%d,%d,%s,%s,%s\n", ticket_details[i]->tid, ticket_details[i]->uid, ticket_details[i]->bid, ticket_details[i]->uname, ticket_details[i]->time, ticket_details[i]->date);
	}
	fclose(fp);
}
/*
int authenticate(char *u_name,char *password)
--->return 0 when password incorrect
--->return -1 when not found
--->return 1 when found correct
*/

/*
get_avaliablie_seats()

*/
//
/*char *getToken(FILE *fp, int *flag) {
char c = getc(fp);
char *str = (char *)malloc(sizeof(char) * 20);
int i = 0;
while (c != EOF && c != ',' && c != '\n') {
str[i++] = c;
c = getc(fp);
}
str[i] = '\0';
if (c == EOF)
*flag = 1;
return str;
}
*/
//
/*int toInt(char *string) {
int number = 0, i = 0;
while (*(string + i) != '\0') {
number = number * 10 + (*(string + i) - '0');
i++;
}
return number;
}
*/


//Doubles size when not sufficient
struct bus **adjustSize(struct bus **buses, int *size, int count) {
	if (count >= *size) {
		*size = *size * 2;
		struct bus **busesList = (struct bus **)malloc(sizeof(struct bus *) * *size);
		for (int k = 0; k < count; k++) {
			busesList[k] = buses[k];
		}
		buses = busesList;
	}
	return buses;
}

//
/*struct bus **loadBuses(int *count) {
FILE *fp = fopen("bus.csv", "r");
int size = 10, flag = 0;
bus **buses = (bus **)malloc(sizeof(bus*) * size);
while (true) {
bus *b = (bus*)malloc(sizeof(bus));
b->bid = toInt(getToken(fp, &flag));
b->time = getToken(fp, &flag);
b->source = getToken(fp, &flag);
b->destination = getToken(fp, &flag);
b->type = getToken(fp, &flag);
b->price = toInt(getToken(fp, &flag));
b->date = getToken(fp, &flag);
if (*count > size)
buses = adjustSize(buses, &size, *count);
buses[*count] = b;
*count = *count + 1;
if (flag == 1)
break;
}
return buses;
}*/
//comparing string1 and string2
bool areEqual(char *string1, char *string2) {
	if (string1 == NULL || string2 == NULL)
		return false;
	int index = 0;
	while (string1[index] != '\0' && string2[index] != '\0') {
		if (string1[index] != string2[index])
			return false;
		index++;
	}
	if (string1[index] != '\0' || string2[index] != '\0')
		return false;
	return true;
}
//checks whether source destination and date matches
bool isMatched(struct bus *buses, char *src, char *dest, char *date) {
	return (areEqual(buses->source, src) && areEqual(buses->destination, dest) && areEqual(buses->date, date));
}

//returns avaliable buses
struct Bus_response *getAvailableBuses(char *src, char *dest, char *date) {
	if (src == NULL || dest == NULL || date == NULL)
		return NULL;
	struct bus **buses = bus_attributes;
	int size = 10, length = 0;
	struct bus **availableBuses = (struct bus **)malloc(sizeof(struct bus*) * size);
	for (int i = 0; i < bus_count; i++) {
		availableBuses[i] = (struct bus*)malloc(sizeof(struct bus));
		if (isMatched(buses[i], src, dest, date)) {
			if (length >= size)
				availableBuses = adjustSize(availableBuses, &size, length);
			availableBuses[length++] = buses[i];
		}
	}
	if(length==0)
		return NULL;
	struct Bus_response *busResponse = (struct Bus_response *)malloc(sizeof(Bus_response));
	busResponse->length = length;
	busResponse->bus_details = availableBuses;
	return busResponse;
}

/*
char * get_seats(int bid,str date);
returns a string with avaliable seats and booked seats
else NULL
*/
char *get_seats(int bid, char *date)
{
	if (bid <= 0 || date == NULL)
		return NULL;
	int i;
	for (i = 0; i < seats_count; i++)
	{
		if (seat_details[i]->bid == bid && strcmp(seat_details[i]->date, date) == 0)
			return seat_details[i]->seat_string;
	}
	return NULL;
}

/*
authenicate
returns -2 :Errvr case
returns uid :crrt
returns -1: passwrd incrrt
*/
int authenticate(char *uname, char *pass)
{
	if (uname == NULL || pass == NULL)
		return -2;
	int i;

	for (i = 0; i < user_count; i++)
	{
		//if user is found
		if (strcmp(user_details[i]->uname, uname) == 0)
		{
			//if password is correct
			if (strcmp(user_details[i]->password, pass) == 0)
				return i + 1;

			//if password is incorrect
			return -1;
		}

	}

	//user not found
	return -2;

}


//Checks whether user exits 0r n0t
int usernameExists(char *username)
{
	int i;
	for (i = 0; i < user_count; i++)
	{
		if (strcmp(user_details[i]->uname, username) == 0)
			return 1;

	}
	return 0;
}

/*
registerUser:
returns -1 :Error
returns 0:user registered
*/
int registerUser(char *username, char *password)
{
	if (username == NULL || password == NULL)
		return -1;

	//check if user exists 
	if (usernameExists(username))
		return -1;


	//register user
	user_details = (struct user**)realloc(user_details, sizeof(struct user*)*(user_count + 1));
	user_details[user_count] = (struct user*)malloc(sizeof(struct user));
	user_details[user_count]->uid = user_count + 1;
	user_details[user_count]->uname = (char*)malloc(sizeof(char) * 1024);
	user_details[user_count]->password = (char*)malloc(sizeof(char) * 1024);
	strcpy(user_details[user_count]->uname, username);
	strcpy(user_details[user_count]->password, password);
	user_count++;
	//printf("%d  %s   %s\n", user_details[user_count - 1]->uid, user_details[user_count - 1]->uname, user_details[user_count - 1]->password);
	return 0;
}



int update_transcation_history(int uid, int bid)
{
	if (uid <= 0 || bid <= 0)
		return -1;
	ticket_details = (struct tickets**)realloc(ticket_details, sizeof(tickets)*(tickets_count + 1));
	ticket_details[tickets_count] = (struct tickets*)malloc(sizeof(tickets));
	ticket_details[tickets_count]->bid = bid;
	ticket_details[tickets_count]->tid = tickets_count + 1;
	ticket_details[tickets_count]->uid = uid;

	ticket_details[tickets_count]->uname = (char*)malloc(sizeof(char) * 1024);
	ticket_details[tickets_count]->date = (char*)malloc(sizeof(char) * 1024);
	ticket_details[tickets_count]->time = (char*)malloc(sizeof(char) * 1024);
	strcpy(ticket_details[tickets_count]->uname, user_details[uid - 1]->uname);
	strcpy(ticket_details[tickets_count]->date, bus_attributes[bid - 1]->date);
	strcpy(ticket_details[tickets_count]->time, bus_attributes[bid - 1]->time);
	tickets_count++;
	printf("Done\n");
	return tickets_count;

}
/*
int update_seats(int bid,char *date,int* seats,int no_of_seats_booked)
updates  avaliable seats and returns tickkets_count
returns -1 when uid||bid<=0
calls update_transactin_history
*/
int update_seats(int uid, int bid, char *date, int *seats, int numberofseatsbooked)
{
	int i, j, flag = 0;
	for (i = 0; i < seats_count; i++)
	{
		if (seat_details[i]->bid == bid && strcmp(seat_details[i]->date, date) == 0)
		{
			flag = 1;
			for (j = 0; j < numberofseatsbooked; j++)
			{
				if (seat_details[i]->seat_string[seats[j]-1] == '1')
					seat_details[i]->seat_string[seats[j]-1] = '0';
				else
					return 0;
			}
			break;
		}
	}
	if (flag == 0)
		return -1;
	return update_transcation_history(uid, bid);
}

/*
Ticket_response* get_tickets(int uid)
returns Transcation_histrory
returns NULL when uid<=0
*/
Ticket_response* get_tickets_db(int uid)
{
	if (uid <= 0 || ticket_details == NULL)
		return NULL;
	int len = 0, size = 10;
	ticket_response = (Ticket_response*)malloc(sizeof(Ticket_response));
	ticket_response->ticket_details = (tickets **)malloc(sizeof(tickets) * size);
	for (int i = 0; i < tickets_count; i++) {
		if (ticket_details[i]->uid == uid) {
			if (len >= size) {
				ticket_response->ticket_details = (tickets **)realloc(ticket_response->ticket_details, sizeof(tickets) * 2 * size);
				size *= 2;
			}
			ticket_response->ticket_details[len++] = ticket_details[i];
		}
	}
	if (len == 0)
		return NULL;
	ticket_response->length = len;
	ticket_response->ticket_details = (tickets **)realloc(ticket_response->ticket_details, sizeof(tickets) * len);
	return ticket_response;

	//	ticket_response->ticket_details
}
