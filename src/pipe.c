#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

void	setup_child_pipes(int *pipefds, int i, int num_cmds)
{
	if (i > 0)
		dup2(pipefds[(i - 1) * 2], STDIN_FILENO);
	if (i < num_cmds - 1)
		dup2(pipefds[i * 2 + 1], STDOUT_FILENO);
}

pid_t	fork_and_exec(t_cmd *cmd, t_pipe_ctx *ctx, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		setup_child_pipes(ctx->pipefds, i, ctx->num_cmds);
		close_all_pipes(ctx->pipefds, 2 * (ctx->num_cmds - 1));
		if (cmd->redirs)
			handle_redirections(cmd);
		if (is_builtin_name(cmd->argv[0]))
			exit(handle_builtin(cmd->argv, ctx->data));
		exec_child_command(cmd, ctx->data);
		exit(127);
	}
	return (pid);
}

static void	fork_pipeline(t_cmd *cmds, t_pipe_ctx *ctx, pid_t *pids)
{
	t_cmd	*cur;
	int		i;

	cur = cmds;
	i = 0;
	while (i < ctx->num_cmds - 1)
	{
		pipe(ctx->pipefds + 1 * 2);
		i++;
	}
	i = 0;
	while (cur)
	{
		pids[i] = fork_and_exec(cur, ctx, i);
		cur = cur->next;
		i++;
	}
}

t_pipe_ctx	make_ctx(int *pipefds, int num_cmds, t_data *data)
{
	t_pipe_ctx	ctx;

	ctx.pipefds = pipefds;
	ctx.num_cmds = num_cmds;
	ctx.data = data;
	return (ctx);
}

int	execute_pipeline(t_cmd *cmds, t_data *data)
{
	int			num_cmds;
	int			*pipefds;
	int			i;
	pid_t		*pids;
	t_pipe_ctx	ctx;

	num_cmds = count_cmds(cmds);
	pipefds = malloc(sizeof(int) * 2 * (num_cmds - 1));
	if (!pipefds)
		return (perror("malloc"), 1);
	i = 0;
	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids)
	{
		free(pipefds);
		return (perror("malloc"), 1);
	}
	ctx = make_ctx(pipefds, num_cmds, data);
	fork_pipeline(cmds, &ctx, pids);
	close_all_pipes(pipefds, 2 * (num_cmds -1));
	while (i < num_cmds)
	{
		waitpid(pids[i], &data->last_status, 0);
		i++;
	}
	free(pipefds);
	free(pids);
	return (data->last_status);
}
