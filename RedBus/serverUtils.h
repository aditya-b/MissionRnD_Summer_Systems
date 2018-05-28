#include "dbUtils.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>

/*Global Variables for ticket and bus cache*/
bus bus_cache[100];
int bus_cache_length = 0;

void close()
{
	flush();
}
/*Fuction to process login request*/
int login_request(char *uname, char *password)
{
	return authenticate(uname, password); //Post login request to DB
}

/*Function to process new user registration*/
int register_request(char *name,char *pass)
{
	return registerUser(name,pass); //Post register request to DB
}

/*Function to process ticket history*/
Ticket_response* get_tickets(int uid)
{
	Ticket_response *response = (Ticket_response*)malloc(sizeof(Ticket_response)); 
	Ticket_response *cache_from_server = get_tickets_db(uid); //Post username to retrieve ticket history from DB
	if (cache_from_server == NULL)
		return NULL;
	int length = cache_from_server->length;
	response->ticket_details = (tickets**)malloc(sizeof(tickets)*length);
	int i;
	for (i = 0; i < length; i++)
	{
		response->ticket_details[i] = (tickets*)malloc(sizeof(tickets));
		response->ticket_details[i]->tid = cache_from_server->ticket_details[i]->tid;
		response->ticket_details[i]->uid = cache_from_server->ticket_details[i]->uid;
		response->ticket_details[i]->bid = cache_from_server->ticket_details[i]->bid;
		char *dummy = cache_from_server->ticket_details[i]->date;
		response->ticket_details[i]->date = (char*)malloc(sizeof(char)*strlen(dummy));
		strcpy(response->ticket_details[i]->date, dummy);
		dummy = cache_from_server->ticket_details[i]->uname;
		response->ticket_details[i]->uname = (char*)malloc(sizeof(char)*strlen(dummy));
		strcpy(response->ticket_details[i]->uname, dummy);
		dummy = cache_from_server->ticket_details[i]->time;
		response->ticket_details[i]->time = (char*)malloc(sizeof(char)*strlen(dummy));
		strcpy(response->ticket_details[i]->time, dummy);
	}
	response->length = length;
	return response;
}

/*Function to check if the requested SRC,DEST combination exists in cache.*/
Bus_response* get_buses_from_cache(char *src, char *dest)
{
	int i = 0;
	int x = 0;
	Bus_response *response = (Bus_response*)malloc(sizeof(Bus_response));
	response->bus_details = (bus**)malloc(sizeof(bus)*bus_cache_length);
	for (i = 0; i < bus_cache_length; i++)
	{
		bus dummy = bus_cache[i];
		if (!_stricmp(dummy.source, src) && !_stricmp(dummy.destination, dest))
		{
			response->bus_details[x] = (bus*)malloc(sizeof(bus));
			response->bus_details[x]->bid = dummy.bid;
			response->bus_details[x]->price = dummy.price;
			strcpy(response->bus_details[x]->type , dummy.type);
			strcpy(response->bus_details[x]->time, dummy.time);
			strcpy(response->bus_details[x]->source, src);
			strcpy(response->bus_details[x]->destination, dest);
			x++;
		}
	}
	if (x == 0)
		return NULL;
	response->bus_details = (bus**)realloc(response->bus_details,sizeof(bus)*x);
	response->length = x;
	return response;

}

/*Function to process buses request (client-API) */
Bus_response* get_buses(char *src, char *dest, char *date)
{
	Bus_response *response=(Bus_response*)malloc(sizeof(Bus_response));
	int len = 0;
	Bus_response *cache_from_server= get_buses_from_cache(src, dest);
	if (cache_from_server == NULL)
	{
		cache_from_server = getAvailableBuses(src, dest, date);
		if (cache_from_server == NULL)
			return NULL;
		int max = bus_cache_length;
		len = (*cache_from_server).length;
		if (len == 0)
		{
			return NULL;
		}
		else
		{
			bus_cache_length += len;
			int x, y;
			for (x = max, y = 0; y < len; x++, y++) //Update Cache
			{
				bus_cache[x % 100].bid = cache_from_server->bus_details[y]->bid;
				char *dummy;
				dummy = cache_from_server->bus_details[y]->type;
				bus_cache[x % 100].type = (char*)malloc(sizeof(char)*strlen(dummy));
				strcpy(bus_cache[x % 100].type, dummy);
				dummy = cache_from_server->bus_details[y]->time;
				bus_cache[x % 100].time = (char*)malloc(sizeof(char)*strlen(dummy));
				strcpy(bus_cache[x % 100].time, cache_from_server->bus_details[y]->time);
				dummy = cache_from_server->bus_details[y]->date;
				bus_cache[x % 100].date = (char*)malloc(sizeof(char)*strlen(dummy));
				strcpy(bus_cache[x % 100].date, cache_from_server->bus_details[y]->date);
				bus_cache[x % 100].source = (char*)malloc(sizeof(char)*strlen(src));
				strcpy(bus_cache[x % 100].source, src);
				bus_cache[x % 100].price = cache_from_server->bus_details[y]->price;
				bus_cache[x % 100].destination = (char*)malloc(sizeof(char)*strlen(dest));
				strcpy(bus_cache[x % 100].destination, dest);
			}
			response->bus_details = (bus**)malloc(sizeof(bus)*len); //Generate response
			for (x = 0; x < len; x++)
			{
				response->bus_details[x] = (bus*)malloc(sizeof(bus));
				char *dummy;
				response->bus_details[x]->bid = cache_from_server->bus_details[x]->bid;
				dummy = cache_from_server->bus_details[x]->type;
				response->bus_details[x]->type = (char*)malloc(sizeof(char)*strlen(dummy));
				strcpy(response->bus_details[x]->type, dummy);
				dummy = cache_from_server->bus_details[x]->time;
				response->bus_details[x]->time = (char*)malloc(sizeof(char)*strlen(dummy));
				strcpy(response->bus_details[x]->time, cache_from_server->bus_details[x]->time);
				response->bus_details[x]->source = (char*)malloc(sizeof(char)*strlen(src));
				strcpy(response->bus_details[x]->source, src);
				dummy = cache_from_server->bus_details[x]->date;
				response->bus_details[x]->date = (char*)malloc(sizeof(char)*strlen(dummy));
				strcpy(response->bus_details[x]->date, cache_from_server->bus_details[x]->date);
				response->bus_details[x]->price = cache_from_server->bus_details[x]->price;
				response->bus_details[x]->destination = (char*)malloc(sizeof(char)*strlen(dest));
				strcpy(response->bus_details[x]->destination, dest);
			}
			response->length = len;
			return response;
		}
	}
	else
		return cache_from_server;
}

/*Function to get seats based on the specified date,source and destination*/
char* get_seats_server(int bid, char *date)
{
	char *seats = get_seats(bid, date);
	if (seats == NULL)
		return NULL;
	else
	{
		char *seat_map = (char*)malloc(sizeof(char)*strlen(seats));
		strcpy(seat_map, seats);
		return seat_map;
	}
}

/*Function to book tickets*/
int book_ticket(int uid,int bid, char *date, int seats, int *seat_nos)
{
	return update_seats(uid,bid, date, seat_nos, seats);
}