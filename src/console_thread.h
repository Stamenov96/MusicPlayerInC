#ifndef CONSOLE_THREAD_HH_
#define CONSOLE_THREAD_HH_

typedef struct Communication
{
	int command;
	struct Communication* next;
	char* parameter;
}Communication;

enum Commands
{
	COMMAND_PLAY = 0,
	COMMAND_PAUSE,
	COMMAND_STOP,
	COMMAND_NEXT,
	COMMAND_PREV,
	COMMAND_ADD,
	COMMAND_REMOVE,
	COMMAND_LIST,
	COMMAND_QUIT,
	COMMAND_NULL
};
#endif
