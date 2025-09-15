#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include "minishell.h"
#include "libft/libft.h"

void rl_print_error_and_reset_prompt(char *msg)
{
	ft_putendl_fd(msg, 2);              /* print error on stderr */
	rl_replace_line("", 0);             /* clear current input line */
	rl_on_new_line();                   /* move to new line */
	rl_redisplay();                     /* force prompt redraw */
}
