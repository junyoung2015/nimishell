
#include "minishell.h"

t_bool	is_squote(char ch)
{
	return (ch == '\'');
}

t_bool	is_dquote(char ch)
{
	return (ch == '"');
}

t_bool	is_quote(char ch)
{
	return (is_squote(ch) || is_dquote(ch));
}
