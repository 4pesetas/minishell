#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

static int	open_redir(t_redir *redir, int *fd)
{
	*fd = -1;
	if (redir->type == T_REDIR_OUT)
		*fd = open(redir->target, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	else if (redir->type == T_APPEND)
		*fd = open(redir->target, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (redir->type == T_REDIR_IN)
		*fd = open(redir->target, O_RDONLY);
	else if (redir->type == T_HEREDOC)
	{
		if (handle_heredoc(redir->target) != 0)
			return (1);
	}
	if (*fd < 0 && redir->type != T_HEREDOC)
		return (perror("minishell: open"), 1);
	return (0);
}

int	handle_redirections(t_cmd *cmd)
{
	t_redir	*redir;
	int		fd;

	redir = cmd->redirs;
	while (redir != NULL)
	{
		if (open_redir(redir, &fd) != 0)
			return (-1);
		if (fd >= 0)
		{
			if (redir->type == T_REDIR_OUT || redir->type == T_APPEND)
				dup2(fd, STDOUT_FILENO);
			else if (redir->type == T_REDIR_IN)
				dup2(fd, STDIN_FILENO);
			close(fd);
		}
		redir = redir->next;
	}
	return (0);
}

int	apply_redirections(t_cmd *cmd, int *saved_in, int *saved_out)
{
	if (!cmd)
		return (0);
	*saved_in = dup(STDIN_FILENO);
	*saved_out = dup(STDOUT_FILENO);
	if (*saved_in < 0 || *saved_out < 0)
	{
		perror("dup");
		if (*saved_in >= 0)
			close(*saved_in);
		if (*saved_out >= 0)
			close(*saved_out);
		return (1);
	}
	if (handle_redirections(cmd) != 0)
	{
		dup2(*saved_in, STDIN_FILENO);
		dup2(*saved_out, STDOUT_FILENO);
		close(*saved_in);
		close(*saved_out);
		return (1);
	}
	return (0);
}
