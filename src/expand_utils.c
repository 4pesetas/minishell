#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

char	*append_char(char *result, char ch)
{
	char	c[2];
	char	*temp;

	c[0] = ch;
	c[1] = '\0';
	temp = ft_strjoin(result, c);
	free(result);
	return (temp);
}

char	*append_str(char *result, const char *s)
{
	char	*temp;

	temp = ft_strjoin(result, s);
	free(result);
	return (temp);
}

static char	*process_dollar(char *result, char *token, int *i, t_data *data)
{
	char	*expanded;
	char	*temp;

	expanded = expand_variable(token, i, data);
	if (!expanded)
	{
		free(result);
		return (NULL);
	}
	temp = append_str(result, expanded);
	free(expanded);
	return (temp);
}

char	*expand_token(char *token, t_data *data)
{
	char	*result;
	int		i;
	t_state	state;
	t_state	new;

	if (!token)
		return (NULL);
	result = ft_strdup("");
	if (!result)
		return (NULL);
	i = 0;
	state = NORMAL;
	while (token[i])
	{
		if (token[i] && (token[i] == '\'' || token[i] == '"'))
		{
			new = update_state(state, token[i]);
			if (new == state)
				result = append_char(result, token[i]);
			state = new;
		}
		else if (token[i] == '$' && state != IN_SINGLE)
		{
			result = process_dollar(result, token, &i, data);
			i--;
		}
		else
			result = append_char(result, token[i]);
		if (!result)
			return (NULL);
		i++;
	}
	return (result);
}

void	clean_tokens(t_token *head, t_data *data)
{
	t_token	*cur;
	char	*expanded;

	cur = head;
	while (cur)
	{
		expanded = expand_token(cur->value, data);
		free(cur->value);
		cur->value = expanded;
		cur = cur->next;
	}
}
