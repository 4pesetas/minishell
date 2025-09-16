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
	return (i);
}

void	setup_child_pipes(int *pipefds, int i, int num_cmds)
{
	if (i > 0 && dup2(pipefds[(i - 1) * 2], STDIN_FILENO) < 0)
		perror("dup2");
	if (i < num_cmds - 1 && dup2(pipefds[i * 2 + 1], STDOUT_FILENO) < 0)
		perror("dup2");
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
