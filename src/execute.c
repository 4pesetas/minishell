#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

void	exec_child_command(t_cmd *cmds, t_data *data)
{
	char				*full_path;
	struct sigaction	sa;

	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	if (cmds->redirs && handle_redirections(cmds) != 0)
		exit (1);
	if (ft_strchr(cmds->argv[0], '/'))
		execve(cmds->argv[0], cmds->argv, data->env);
	full_path = find_in_path(cmds->argv[0], data);
	if (!full_path)
	{
		ft_printf("minishell: %s: command not found\n", cmds->argv[0]);
		exit(127);
	}
	execve(full_path, cmds->argv, data->env);
	perror("execve");
	exit(127);
}

int	execute_cmds(t_cmd *cmds, t_data *data)
{
	pid_t	pid;
	int		status;

	if (!cmds || !cmds->argv ||!cmds->argv[0])
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
		exec_child_command(cmds, data);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
