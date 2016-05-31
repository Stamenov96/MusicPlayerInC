#include "init.h"


void init(char *title)
{
	/* Initialise SDL Audio */
	
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
	{
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		
		exit(1);
	}
	
	
	/* Set the audio rate to 22050, 16 bit stereo, 2 channels and a 4096 byte buffer */
	
	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) != 0)
	{
		printf("Could not open audio: %s\n", Mix_GetError());
		
		exit(1);
	}
	
}

void cleanup()
{
	/* Free the sound effect */
	
	if (soundFile != NULL)
	{
		Mix_FreeChunk(soundFile);
	}
	
	/* Close the mixer */
	
	Mix_CloseAudio();
	
	/* Shut down SDL */
	
	
	SDL_Quit();
	
}
