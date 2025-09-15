#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

int	check_input_quotes(char *input)
{
	if (are_unclosed_quotes(input))
	{
		ft_putendl_fd("minishell: syntax error: unclosed quotes", 2);
		free(input);
		return (1);
	}
	return (0);
}

void	restore_stdio(int saved_in, int saved_out)
{
	if (saved_in >= 0)
	{
		dup2(saved_in, STDIN_FILENO);
		close(saved_in);
	}
	if (saved_out >= 0)
	{
		dup2(saved_out, STDOUT_FILENO);
		close(saved_out);
	}
}
