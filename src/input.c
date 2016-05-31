#include "input.h"
#include "console_thread.h"

extern void playSound(Mix_Chunk *sfx);
extern void pauseSound();
extern void resumeSound();

void getInput(int command)
{
	
	switch(command){
		
		case COMMAND_PLAY:
		    if( Mix_PlayingMusic() == 0 )
                {
					playSound(soundFile);
		
        		}	
			else
            {
				if( Mix_PausedMusic() == 1 )
				{
                  	Mix_ResumeMusic();
                }   
			}
		break;
			
		case COMMAND_PAUSE:
			if( Mix_PausedMusic() == 1 )
			{
                Mix_ResumeMusic();
            }else{   
				Mix_PauseMusic();
			}
		break;
					
		case COMMAND_STOP:
			Mix_HaltMusic();
		break;
		
		case COMMAND_QUIT:
				exit(0);
					
		default:
		break;
				}		
		
		
		
		}
