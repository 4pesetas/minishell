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

int	echo_builtin(char **args, t_data *data)
{
	int		i;
	int		newline;

	(void)data;
	i = 1;
	newline = 1;
	while (args[i] && ft_is_n(args[i]))
	{
		newline = 0;
		i++;
	}
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}
