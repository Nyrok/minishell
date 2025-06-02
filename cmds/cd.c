#include "cmds.h"

int	cd(const char *path)
{
	if (chdir(path) != 0)
	{
		printf("Impossible d'entrer de ce dossier !");
		return (-1);
	}
	printf("dd");
	return (1);
}

int	main(void)
{
	cd("test");
	cd("test");
}
