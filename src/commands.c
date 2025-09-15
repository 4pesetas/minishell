#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

t_redir	*create_redir(t_toktype type, char *target)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->target = ft_strdup(target);
	redir->next = NULL;
	return (redir);
}

void	append_redir(t_redir **head, t_redir *new_redir)
{
	t_redir *temp;
	if (!*head)
	{
		*head = new_redir;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_redir;
}

t_cmd	*create_cmd(char **argv, int argc, t_redir *redirs)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argv = argv;
	cmd->argc = argc;
	cmd->redirs = redirs;
	cmd->next = NULL;
	return (cmd);
}

void	append_cmd(t_cmd **head, t_cmd *new_cmd)
{
	t_cmd	*temp;
	if (!*head)
	{
		*head = new_cmd;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new_cmd;
}

t_cmd	*parse_tokens_to_cmds(t_token *token_head)
{
	t_cmd 		*head_cmd;
	t_redir 	*current_redirs;
	char		**current_argv;
	int			argc;
	t_token		*token;
	t_toktype	rtype;

	head_cmd = NULL;
	current_redirs = NULL;
	current_argv = NULL;
	argc = 0;
	token = token_head;
	while (token)
	{
		if (token->type == T_WORD)
		{
			current_argv = ft_realloc_argv(current_argv, argc, token->value);
			argc++;
		}
		else if (token->type == T_REDIR_IN || token->type == T_REDIR_OUT
			|| token->type == T_HEREDOC || token->type == T_APPEND)
		{
			rtype = token->type;
			token = token->next;
			if (token && token->type == T_WORD)
				append_redir(&current_redirs, create_redir(rtype, token->value));
			else
			{
				ft_putendl_fd("minishell: syntax error near redirection", 2);
                return (NULL);
			}
		}
		else if (token->type == T_PIPE)
		{
			append_cmd(&head_cmd, create_cmd(current_argv, argc, current_redirs));
			current_argv = NULL;
			argc = 0;
			current_redirs = NULL;
		}
		token = token->next;
	}
	if (argc > 0 || current_redirs)
		append_cmd(&head_cmd, create_cmd(current_argv, argc, current_redirs));
	return (head_cmd);

}
