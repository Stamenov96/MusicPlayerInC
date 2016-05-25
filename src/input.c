#include "input.h"

extern void playSound(Mix_Chunk *sfx);
extern void pauseSound();
extern void resumeSound();

void getInput()
{
	SDL_Event event;
	
	/* Loop through waiting messages and process them */
	
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			/*
			** Closing the Window or pressing Escape will exit the program
			** Pressing Space will play the sound effect
			*/
			
			case SDL_QUIT:
				exit(0);
			break;
			
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_SPACE:
						playSound(soundFile);
					break;
					
					case SDLK_UP:
						pauseSound(soundFile);
					break;
					
					case SDLK_DOWN:
						resumeSound(soundFile);
					break;
					
					case SDLK_ESCAPE:
						exit(0);
					break;
					
					default:
					break;
				}
			break;
		}
	}
}
