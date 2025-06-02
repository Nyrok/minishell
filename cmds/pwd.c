#include "cmds.h"

int	pwd()
{
	printf("%s\n", getenv("PWD"));
	return (1);
}