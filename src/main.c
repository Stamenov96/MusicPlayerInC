/* This tutorial will open a 640 x 480 screen,
** load an image and display it at specified coordinates.
** Pressing space will play a sound effect and pressing escape
** or closing the window will exit the program
*/

#include "main.h"
#include "input.h"
#include "console_thread.h"

extern void playSound(Mix_Chunk *sfx);
extern void pauseSound();
extern void resumeSound();
extern void updateScreen(void);
extern void init(char *);
extern void cleanup(void);
extern void getInput(int);
extern SDL_Surface *loadImage(char *);
extern Mix_Chunk *loadSound(char *);
extern Mix_Chunk *soundFile;

int main(int argc, char *argv[])
{
	int go;
	
	/* Start up SDL */
	
	init("Music Player in C");
	
	/* Call the cleanup function when the program exits */
	
	atexit(cleanup);
	
	go = 1;
	
	dexterImage = loadImage("gfx/dexter.jpg");
	
	/* If we get back a NULL image, just exit */
	
	if (dexterImage == NULL)
	{
		exit(0);
	}
	
	
	Communication* comm_head = (Communication*)malloc(sizeof(Communication));
	comm_head->command= COMMAND_NULL;
	comm_head->next= NULL;
	comm_head->parameter=NULL;
	
	pthread_t new_thread;
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_init(&mutex,NULL);
	
	Comm comm;
	comm.head = comm_head;
	comm.mutex = &mutex;
		
	pthread_create (&new_thread, NULL ,console,(void*)&comm);
	
	
	
	/* If we get back a NULL sound, just exit 
	
	if (soundFile == NULL)
	{
		exit(0);
	}
	 Loop indefinitely for messages */
	
	while (go == 1)
	{
		
		if(comm_head->next != NULL){	
			//printf("WAZAAAA\n");
			pthread_mutex_lock(&mutex);
			if(comm_head->next->parameter != NULL){
				soundFile = loadSound(comm_head->next->parameter);
			}
		getInput(comm_head->next->command);
		
		Communication* curr = comm_head->next;
		//comm_head=comm_head->next;
		
		comm_head->next = curr->next;
		
		printf("CHECK FREE BEFORE FREE");
		
		//printf("----------------------------^^^^^^^^^^^^^^^   %s\n",curr->parameter);
			
		if(curr->parameter != NULL){
			free(curr->parameter);}
		printf("----------------------------^^^^^^^^^^^^^^^   %s\n",curr->parameter);
	
			
		free(curr);
		
		printf("CHECK FREE AFTER FREE");
			
		
		}
		pthread_mutex_unlock(&mutex);
		
		updateScreen();
		
		/* Sleep briefly to stop sucking up all the CPU time */
		
		
		SDL_Delay(16);
	}
	pthread_join(new_thread,NULL);
	pthread_mutex_destroy(&mutex);
	/* Exit the program */
	
	exit(0);
}
