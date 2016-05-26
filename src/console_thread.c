#include "console_thread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct Playlist
{
	struct Playlist* next;
	struct Playlist* prev;
	char* filename;
}Playlist;

void send_command(int command, const char* param, Communication* comm_head, pthread_mutex_t* mutex)
{
	pthread_mutex_lock(mutex);
	Communication* curr_comm = comm_head;
	
	while((curr_comm->next) != NULL)
	{
		curr_comm = curr_comm->next;
	}
	
	Communication* comm = (Communication*)malloc(sizeof(Communication));
	comm->command = command;
	comm->next = NULL;
	curr_comm->next = comm;
	
	if(param == NULL)
	{
		comm->parameter = NULL;
	}
	else
	{
		comm->parameter = (char*)malloc(strlen(param));
		strcpy(comm->parameter, param);
	}
	pthread_mutex_unlock(mutex);
}

void kill_playlist(Playlist* head, Playlist* tail)
{
	Playlist* curr = tail;
	while(curr != head)
	{
		tail = curr->prev;
		if(curr->filename != NULL)
			free(curr->filename);
		free(curr);
		curr = tail;
		tail->next = NULL;
	}
}

void* console(void* arg)
{
	// PLAYLIST INIT
	Playlist* head = (Playlist*)malloc(sizeof(Playlist));
	Playlist* tail = head;
	head->next = NULL;
	head->prev = NULL;
	head->filename = NULL;
	Playlist* curr = head;
	Playlist* iter;

	// THREAD INIT
	Comm* comm = (Comm*)arg;
	Communication* comm_head = comm->head;
	pthread_mutex_t* mutex = comm->mutex;

	const char delimiters[] = {' ', '\n', '\r', '\t', '\f', '\v','\0'};
	const char* commands[] = {"play", "pause", "stop", "next", "prev", "add", "remove", "list"};
	
	char quit = 0;
	//char playing = 0;
	//char stopped = 1;
	char buffer[1024];
	char* token;
	int i;
	int command_index;
	
	while(!quit)
	{
		//RESET + INPUT
		command_index = COMMAND_NULL;
		fgets (buffer, 1024, stdin);
		token = strtok(buffer, delimiters);
		
		//GET COMMAND
		for(i = 0; i < COMMAND_NULL; i++)
		{
		
			if(strcmp(token, commands[i]) == 0)
			{
				command_index = i;
				break;
			}
		}
		
		token = strtok(NULL, delimiters);
		
		//PLAY
		//printf("COMMAN INDEXX ==== %d\n",command_index);
		if(command_index == COMMAND_PLAY)
		{

			if(token == NULL)
			{
				send_command(COMMAND_PLAY, NULL, comm_head, mutex);
			}
			else
			{
				kill_playlist(head, tail);
				Playlist* new_song = (Playlist*)malloc(sizeof(Playlist));
				new_song->prev = head;
				new_song->next = NULL;
				new_song->filename = (char*)malloc(strlen(token)+1);
				strcpy(new_song->filename, token);
				head->next = new_song;
				tail = new_song;
				curr = tail;
				send_command(COMMAND_PLAY, curr->filename, comm_head, mutex);
			}
		}
		
		//PAUSE
		if(command_index == COMMAND_PAUSE)
		{
			send_command(COMMAND_PAUSE, NULL, comm_head, mutex);
		}
		
		//STOP
		if(command_index == COMMAND_STOP)
		{
			send_command(COMMAND_STOP, NULL, comm_head, mutex);
		}
		
		//NEXT
		if(command_index == COMMAND_NEXT)
		{
			if(curr->next == NULL)
			{
				printf("No next song.\n");
			}
			else
			{
				curr = curr->next;
				send_command(COMMAND_STOP, NULL, comm_head, mutex);
				send_command(COMMAND_PLAY, curr->filename, comm_head, mutex);
			}
		}
		
		//PREV
		if(command_index == COMMAND_PREV)
		{
			if(curr->prev == head)
			{
				printf("No previous song.\n");
			}
			else
			{
				curr = curr->prev;
				send_command(COMMAND_STOP, NULL, comm_head, mutex);
				send_command(COMMAND_PLAY, curr->filename, comm_head, mutex);
			}
		}
		
		//ADD
		if(command_index == COMMAND_ADD)
		{
			if(token == NULL)
			{
				printf("No filename given.\n");
			}
			else
			{
				Playlist* new_song = (Playlist*)malloc(sizeof(Playlist));
				new_song->filename = (char*)malloc(strlen(token) + 1);
				strcpy(new_song->filename, token);
				
				new_song->next = curr->next;
				new_song->prev = curr;
				curr->next = new_song;
				
				if(curr != tail)
				{
					new_song->next->prev = new_song;
				}
				else
				{
					tail = new_song;
				}
			}
		}
		
		//REMOVE
		if(command_index == COMMAND_REMOVE)
		{
			if(curr != tail)
			{
				(curr->next)->prev = curr->prev;
			}
			if(curr != head)
			{
				(curr->prev)->next = curr->next;
			}
			Playlist* help = curr->next;
			if(curr->filename != NULL)
				free(curr->filename);
			free(curr);
			curr = help;
		}
		
		//LIST
		printf("000000000  %d\n",command_index);
		if(command_index == COMMAND_LIST)
		{
			printf("BLYAT\n");
			iter = head->next;
			
			for(i = 1; iter != NULL; i++)
			{
				
			printf("CYKAAA\n");
				printf("%d. %s", i, iter->filename);
				if(iter == curr)
				{
					printf(" (Current Song)\n");
				}
				else
				{
					printf("\n");
				}
				iter = iter->next;
			}
		}
		
		//QUIT
		if(command_index == COMMAND_QUIT)
		{
			send_command(COMMAND_QUIT, NULL, comm_head);
			quit = 1;
		}
		
		//WRONG COMMAND
		if(command_index == COMMAND_NULL)
		{
			printf("This is a wrong command.\n");
		}
	}
	pthread_exit(0);
	return NULL;
}
