#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

int	write_heredoc_input(char *delim, int fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
			break ;
		ft_putendl_fd(line, fd);
		free(line);
	}
	free(line);
	return (0);
}

int	handle_heredoc(char *delim)
{
	int	pipefd[2];

	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (1);
	}
	if (write_heredoc_input(delim, pipefd[1]))
	{
		return (1);
	}
	close(pipefd[1]);
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	return (0);
}
