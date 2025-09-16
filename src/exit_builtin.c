#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

void	exit_builtin(char **args, t_data *data, char *input)
{
	int	i;

	i = 0;
	ft_printf("exit\n");
	while (args && args[i])
		free(args[i++]);
	free(args);
	free(input);
	i = 0;
	while (data->env[i])
		free(data->env[i++]);
	free(data->env);
	exit(0);
}
