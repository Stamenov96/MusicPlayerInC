#include "audio.h"

Mix_Music *loadSound(char *name)
{
	/* Load the sound specified by the filename */
	
	Mix_Music *sfx = Mix_LoadMUS(name);

	if (sfx == NULL)
	{
		printf("Failed to load sound %s\n", name);
	}
	
	return sfx;
}

void playSound(Mix_Music *sfx)
{
	/* Play the sound on the first free channel and only play it once */
	
	Mix_PlayMusic(sfx, -1);
}

void pauseSound()
{
	
		Mix_PauseMusic();
}

void resumeSound()
{
		Mix_ResumeMusic();
	
}

