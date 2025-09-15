#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

int	count_cmds(t_cmd *cmds)
{
	int		i;
	t_cmd	*cur;

	i = 0;
	cur = cmds;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (1);
}

void	close_all_pipes(int *pipefds, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		close(pipefds[i]);
		i++;
	}
}
