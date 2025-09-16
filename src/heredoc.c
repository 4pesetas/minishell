#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

static void	sigint_handler_heredoc(int sig)
{
	(void)sig;

	g_sig = 1;
	write(1, "\n", 1);
}

int	swap_stdin_with_fd(int fd)
{
	int	saved_in;

	saved_in = dup(STDIN_FILENO);
	if (saved_in < 0)
	{
		perror("dup");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2");
		close(saved_in);
		return (-1);
	}
	close(fd);
	return (saved_in);
}

void	restore_stdin(int saved_in)
{
	if (saved_in >= 0)
	{
		dup2(saved_in, STDIN_FILENO);
		close(saved_in);
	}
}

int	handle_heredoc(char *delim)
{
	int		pipefd[2];
	int		saved_in;
	struct sigaction sa_old, sa_new;

	if (pipe(pipefd) < 0)
	{
		perror("pipe");
		return (1);
	}
	sa_new.sa_handler = sigint_handler_heredoc;
	sigemptyset(&sa_new.sa_mask);
	sa_new.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_new, &sa_old);
	if (write_heredoc_input(pipefd[1], delim) != 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		sigaction(SIGINT, &sa_old, NULL);
		return (1);
	}
	saved_in = swap_stdin_with_fd(pipefd[0]);
	if (saved_in < 0)
		return (sigaction(SIGINT, &sa_old, NULL), 1);
	sigaction(SIGINT, &sa_old, NULL);
	return (0);
}
