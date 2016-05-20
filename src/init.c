#include "init.h"

void init(char *title)
{
	/* Initialise SDL Video and Audio */
	
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0)
	{
		printf("Could not initialize SDL: %s\n", SDL_GetError());
		
		exit(1);
	}
	
	/* Open a 640 x 480 screen */
	
	screen = SDL_SetVideoMode(640, 480, 0, SDL_HWPALETTE|SDL_DOUBLEBUF);
	
	if (screen == NULL)
	{
		printf("Couldn't set screen mode to 640 x 480: %s\n", SDL_GetError());

		exit(1);
	}
	
	/* Set the audio rate to 22050, 16 bit stereo, 2 channels and a 4096 byte buffer */
	
	if (Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096) != 0)
	{
		printf("Could not open audio: %s\n", Mix_GetError());
		
		exit(1);
	}
	
	/* Set the screen title */
	
	SDL_WM_SetCaption(title, NULL);
}

void cleanup()
{
	/* Free the image */
	
	if (dexterImage != NULL)
	{
		SDL_FreeSurface(dexterImage);
	}
	
	/* Free the sound effect */
	
	if (dexterBark != NULL)
	{
		Mix_FreeChunk(dexterBark);
	}
	
	/* Close the mixer */
	
	Mix_CloseAudio();
	
	/* Shut down SDL */
	
	SDL_Quit();
}
