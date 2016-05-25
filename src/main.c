/* This tutorial will open a 640 x 480 screen,
** load an image and display it at specified coordinates.
** Pressing space will play a sound effect and pressing escape
** or closing the window will exit the program
*/

#include "main.h"
#include "console_thread.h"
#include "input.h"

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
		
	pthread_create (&new_thread, NULL ,console,(void*)comm_head);
	
	
	
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
			if(comm_head->next->parameter != NULL){
				soundFile = loadSound(comm_head->next->parameter);
			}
		getInput(comm_head->next->command);
		comm_head=comm_head->next;
		}
		
		updateScreen();
		
		/* Sleep briefly to stop sucking up all the CPU time */
		
		
		SDL_Delay(16);
	}
	
	/* Exit the program */
	
	exit(0);
}
