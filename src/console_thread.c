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

void send_command(int command, const char* param, Communication* comm_head)
{
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
}

void kill_playlist(Playlist* head, Playlist* tail)
{
	Playlist* curr;
	while(curr != head)
	{
		curr = tail;
		tail = curr->prev;
		free(curr->filename);
		free(curr);
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
	Communication* comm_head = (Communication*)arg;

	const char delimiters[] = {' ', '\n', '\r', '\t', '\f', '\v'};
	const char* commands[] = {"play", "pause", "stop", "next", "prev", "add", "remove", "list"};
	
	char quit = 0;
	char playing = 0;
	char stopped = 1;
	char buffer[1024];
	char* token;
	int i;
	int command_index;
	
	while(!quit)
	{
		//RESET + INPUT
		command_index = COMMAND_NULL;
		scanf("%s", buffer);
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
		if(command_index == COMMAND_PLAY)
		{
			if(token == NULL)
			{
				if(head == tail)
				{
					printf("Nothing to play.\n");
				}
				else
				{
					if(playing)
					{
						printf("Already playing.\n");
					}
					else
					{
						send_command(COMMAND_PLAY, curr->filename, comm_head);
						playing = 1;
						stopped = 0;
					}
				}
			}
			else
			{
				kill_playlist(head, tail);
				Playlist* new_song = (Playlist*)malloc(sizeof(Playlist));
				new_song->prev = head;
				new_song->next = NULL;
				new_song->filename = (char*)malloc(strlen(token)+1);
				strcpy(new_song->filename, token);
				tail = new_song;
				curr = tail;
				send_command(COMMAND_PLAY, curr->filename, comm_head);
				playing = 1;
			}
		}
		
		//PAUSE
		if(command_index == COMMAND_PAUSE)
		{
			if(playing)
			{
				send_command(COMMAND_PAUSE, NULL, comm_head);
				playing = 0;
			}
			else
			{
				printf("Nothing is playing at the moment.\n");
			}
		}
		
		//STOP
		if(command_index == COMMAND_STOP)
		{
			if(stopped)
			{
				printf("Already stopped.\n");
			}
			else
			{
				send_command(COMMAND_STOP, NULL, comm_head);
				stopped = 1;
				playing = 0;
			}
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
				send_command(COMMAND_PLAY, curr->filename, comm_head);
			}
		}
		
		//PREV
		if(command_index == COMMAND_PREV)
		{
			if(curr->prev == NULL)
			{
				printf("No previous song.\n");
			}
			else
			{
				curr = curr->prev;
				send_command(COMMAND_PLAY, curr->filename, comm_head);
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
				new_song->next = curr->next;
				new_song->prev = curr;
				new_song->filename = (char*)malloc(strlen(token) + 1);
				strcpy(new_song->filename, token);
				curr->next = new_song;
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
			free(curr->filename);
			free(curr);
			curr = help;
		}
		
		//LIST
		if(command_index == COMMAND_LIST)
		{
			iter = head->next;
			for(i = 1; iter != tail; i++)
			{
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
