#include "minishell.h"

t_size	handle_wildcard(char ***result, char **start, char **end, t_size size)
{
	char	*tmp;

	*end = *start;
	tmp = ft_strdup("*");
	size = ft_arr_append(result, tmp, size);
	(*end)++;
	return (size);
}

t_size	handle_quotes(char ***result, char **start, char **end, t_size size)
{
	char	*substr;
	t_cmp	cmp;

	cmp = is_squote;
	if (is_dquote(**start))
		cmp = is_dquote;
	*end = *start + 1;
	while (**end && !cmp(**end))
		(*end)++;
	substr = ft_substr(*start, 0, *end - *start + 1);
	if (!substr)
		return (0);
	size = ft_arr_append(result, substr, size);
	(*end)++;
	return (size);
}

t_size	handle_normal(char ***result, char **start, char **end, t_size size)
{
	char	*tmp;

	*end = *start + 1;
	while (**end && !is_wsplit(**end))
		(*end)++;
	tmp = ft_substr(*start, 0, *end - *start);
	size = ft_arr_append(result, tmp, size);
	return (size);
}

char	**split_pattern(char *cmd_arg)
{
	char	*start;
	char	*end;
	char	**result;
	t_size	size;

	size = 0;
	result = 0;
	start = cmd_arg;
	end = start;
	while (start && *start)
	{
		if (is_wsplit(*start))
		{
			if (is_wildcard(*start))
				size = handle_wildcard(&result, &start, &end, size);
			else
				size = handle_quotes(&result, &start, &end, size);
		}
		else
			size = handle_normal(&result, &start, &end, size);
		if (!size)
			return (0);
		start = end;
	}
	return (result);
}
