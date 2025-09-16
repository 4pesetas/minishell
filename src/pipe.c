#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

pid_t	fork_and_exec(t_cmd *cmd, t_pipe_ctx *ctx, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		setup_child_pipes(ctx->pipefds, i, ctx->num_cmds);
		close_all_pipes(ctx->pipefds, 2 * (ctx->num_cmds - 1));
		if (cmd->redirs && handle_redirections(cmd) != 0)
			exit(1);;
		if (is_builtin_name(cmd->argv[0]))
			exit(handle_builtin(cmd->argv, ctx->data));
		exec_child_command(cmd, ctx->data);
		perror("exec");
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
		if (pipe(ctx->pipefds + 1 * 2) < 0)
		{
			perror("pipe");
			exit(1);
		}
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

static int	init_pipeline_ctx(t_cmd *cmds, t_data *data,
		t_pipe_ctx *ctx, pid_t **pids)
{
	int			num_cmds;
	int			*pipefds;

	num_cmds = count_cmds(cmds);
	if (num_cmds <= 0)
		return (0);
	pipefds = malloc(sizeof(int) * 2 * (num_cmds - 1));
	if (!pipefds)
		return (perror("malloc"), 1);
	*pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids)
	{
		free(pipefds);
		return (perror("malloc"), 1);
	}
	ctx->pipefds = pipefds;
	ctx->num_cmds = num_cmds;
	ctx->data = data;
	return (0);
}

int	execute_pipeline(t_cmd *cmds, t_data *data)
{
	int			i;
	pid_t		*pids;
	t_pipe_ctx	ctx;

	i = 0;
	if (init_pipeline_ctx(cmds, data, &ctx, &pids) != 0)
		return (1);
	fork_pipeline(cmds, &ctx, pids);
	close_all_pipes(ctx.pipefds, 2 * (ctx.num_cmds -1));
	while (i < ctx.num_cmds)
		waitpid(pids[i++], &data->last_status, 0);
	free(ctx.pipefds);
	free(pids);
	return (data->last_status);
}
