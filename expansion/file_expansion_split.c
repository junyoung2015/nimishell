#include "minishell.h"

t_bool	is_wildcard(char ch)
{
	return (ch == '*');
}

t_bool	is_wsplit(char ch)
{
	return (is_quote(ch) || is_wildcard(ch));
}

t_bool	is_wildcard_expansion(char *cmd_arg)
{
	char	*tmp;
	t_bool	in_quotes;
	
	tmp = cmd_arg;
	in_quotes = FALSE;
	while (*tmp)
	{
		if (is_squote(*tmp))
			in_quotes = !in_quotes;
		if (is_wildcard(*tmp) && !in_quotes)
			return (TRUE);
		tmp++;
	}
	return (FALSE);
}
