#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

int	print_variable(char *token, int i, char **env)
{
	char	*name;
	char	*value;
	int		start;

	start = i + 1;
	if (!token[start])
		return (i + 1);
	if (!ft_isalnum(token[start]) && token[start] != '_')
		return (i + 2);
	while (token[start] && (ft_isalnum(token[start]) || token[start] == '_'))
		start++;
	name = ft_substr(token, i + 1, start - (i + 1));
	if (!name)
		return (start);
	value = get_env_value(env, name);
	if (value)
		ft_printf("%s", value);
	free(name);
	return (start);
}

void	print_expanded(char *token, t_data *data)
{
	int		i;
	t_state	state;

	i = 0;
	state = NORMAL;
	while (token[i])
	{
		state = toggle_quote(token[i], state);
		if ((token[i] == 39 && state == IN_SINGLE) || (token[i] == 34
				&& state == IN_DOUBLE))
			i++;
		else if (token[i] == '$' && token[i + 1] && state != IN_SINGLE)
		{
			if (token[i + 1] == '?')
			{
				ft_printf("%d", data->last_status);
				i += 2;
			}
			else
				i = print_variable(token, i, data->env);
		}
		else
			ft_printf("%c", token[i++]);
	}
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
