#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

int	ft_is_n(char *args)
{
	int	i;

	if (!args || args[0] != '-' || args[1] != 'n')
		return (0);
	i = 2;
	while (args[i])
	{
		if (args[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}
