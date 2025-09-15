#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

t_state	update_state(t_state state, char ch)
{
	if (ch == '\'')
	{
		if (state == NORMAL)
			state = (IN_SINGLE);
		else if (state == IN_SINGLE)
			state = (NORMAL);
		else if (state == IN_DOUBLE)
			state = (IN_DOUBLE);
	}
	else if (ch == '"')
	{
		if (state == NORMAL)
			state = (IN_DOUBLE);
		else if (state == IN_DOUBLE)
			state = (NORMAL);
		else if (state == IN_SINGLE)
			state = (IN_SINGLE);
	}
	return (state);
}
