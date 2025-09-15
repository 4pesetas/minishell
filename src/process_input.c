#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

t_cmd	*get_cmds_from_input(char *input, t_data *data)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = tokenize_to_list(input);
	if (!tokens)
		return (NULL);
	clean_tokens(tokens, data);
	cmds = parse_tokens_to_cmds(tokens);
	free_tokens(tokens);
	return (cmds);
}

int	handle_builtin(char **args, t_data *data)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (data->last_status = echo_builtin(args, data));
	if (ft_strcmp(args[0], "cd") == 0)
		return (data->last_status = cd_builtin(args, data));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (data->last_status = pwd_builtin(args, data));
	if (ft_strcmp(args[0], "export") == 0)
		return (data->last_status = export_builtin(args, data));
	if (ft_strcmp(args[0], "unset") == 0)
		return (data->last_status = unset_builtin(args, data));
	if (ft_strcmp(args[0], "env") == 0)
		return (data->last_status = env_builtin(args, data));
	return (-1);
}

int	is_builtin_name(char *name)
{
	if (!name)
		return (0);
	return (ft_strcmp(name, "echo") == 0
		|| ft_strcmp(name, "cd") == 0
		|| ft_strcmp(name, "pwd") == 0
		|| ft_strcmp(name, "export") == 0
		|| ft_strcmp(name, "unset") == 0
		|| ft_strcmp(name, "env") == 0);
}

int	manage_builtin(t_cmd *cmds, t_data *data, char *input)
{
	int	saved_in;
	int	saved_out;

	if (ft_strcmp(cmds->argv[0], "exit") == 0)
		exit_builtin(cmds->argv, data, input);
	if (!is_builtin_name(cmds->argv[0]))
		return (0);
	if (cmds->redirs && apply_redirections(cmds, &saved_in,
			&saved_out) != 0)
		return (data->last_status = 1, 1);
	data->last_status = handle_builtin(cmds->argv, data);
	restore_stdio(saved_in, saved_out);
	free_cmds(cmds);
	free(input);
	return (1);
}

void	process_input(char *input, t_data *data)
{
	t_cmd	*cmds;
	int		status;

	if (!input || check_input_quotes(input))
		return ;
	cmds = get_cmds_from_input(input, data);
	if (!cmds)
		return (free(input));
	if (cmds->next == NULL && cmds->argv && cmds->argv[0])
	{
		if (manage_builtin(cmds, data, input))
			return ;
	}
	status = execute_cmds(cmds, data);
	if (status >= 0)
		data->last_status = status;
	free_cmds(cmds);
	free(input);
}
