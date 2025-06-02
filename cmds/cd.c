#include "cmds.h"

int	cd(const char *path)
{
	if (chdir(path) != 0)
	{
		printf("Impossible d'entrer de ce dossier !");
		return (-1);
	}
	return (1);
}
