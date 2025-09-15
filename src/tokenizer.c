#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

void	append_token(t_token **head, char *value, int type)
{
	t_token *new_node;
	t_token	*temp;

	new_node = malloc(sizeof(t_token));
	if (!new_node)
		return ;
	new_node->value = ft_strdup(value);
	new_node->type = type;
	new_node->next = NULL;
	if (*head == NULL)
	{
		*head = new_node;
		return ;
	}
	temp = *head;
	while (temp->next != NULL)
	{
		temp = temp->next;
	}
	temp->next = new_node;
}

static int handle_operator(char *input, int *i, t_token **head)
{
	if (input[*i] == '|')
		append_token(head, "|", T_PIPE);
	else if (input[*i] == '>' && input[*i + 1] == '>')
	{
		append_token(head, ">>", T_APPEND);
		(*i)++;
	}
	else if (input[*i] == '>')
		append_token(head, ">", T_REDIR_OUT);
	else if (input[*i] == '<' && input[*i + 1] == '<')
	{
		append_token(head, "<<", T_HEREDOC);
		(*i)++;
	}
	else if (input[*i] == '<')
		append_token(head, "<", T_REDIR_IN);
	else
		return (0);
	(*i)++;
	return (1);
}

static void	handle_word(char *input, int *i, t_token **head)
{
	int		start;
	char	quote;
	char	*msg;
	char	*token_str;

	start = *i;
	while (input[*i] && !ft_is_space(input[*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>')
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			quote = input[*i];
			(*i)++;
			while (input[*i] && input[*i] != quote)
				(*i)++;
			if (input[*i] == '\0')
			{
				msg = "minishell: syntax error: unclosed quotes";
				rl_print_error_and_reset_prompt(msg);
				free(input);
				return ;
			}
			(*i)++;
			continue ;
		}
		(*i)++;
	}
	token_str = ft_substr(input, start, *i - start);
	append_token(head, token_str, T_WORD);
}

t_token	*tokenize_to_list(char *input)
{
	int		i;
	int		len;
	t_token	*head;

	i = 0;
	head = NULL;
	len = ft_strlen(input);
	while (i < len)
	{
		if (ft_is_space(input[i]))
		{
			i++;
			continue ;
		}
		if (handle_operator(input, &i, &head))
			continue ;
		handle_word(input, &i, &head);
	}
	return (head);
}
