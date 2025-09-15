#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

t_state	toggle_quote(char c, t_state state)
{
	if (c == 39)
	{
		if (state == IN_SINGLE)
			return (NORMAL);
		else if (state == NORMAL)
			return (IN_SINGLE);
	}
	else if (c == 34)
	{
		if (state == IN_DOUBLE)
			return (NORMAL);
		else if (state == NORMAL)
			return (IN_DOUBLE);
	}
	return (state);
}

void	add_token(char ***tokens, char *new_token)
{
	char	**new_array;
	int		count;
	int		i;

	count = 0;
	if (*tokens)
	{
		while ((*tokens)[count])
			count++;
	}
	new_array = malloc(sizeof (char *) * (count +2));
	if (!new_array)
		return ;
	i = 0;
	while (i < count)
	{
		new_array[i] = (*tokens)[i];
		i++;
	}
	new_array[i] = new_token;
	new_array[i + 1] = NULL;
	free(*tokens);
	*tokens = new_array;
}

void	finish_space_token(char ***tokens, char *input, int start, int i)
{
	char	*str;

	str = ft_substr(input, start, i - start);
	if (i > start)
		add_token(tokens, str);
}
