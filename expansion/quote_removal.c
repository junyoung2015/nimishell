#include "minishell.h"

// TODO: 밑의 typedef 를 env_sub 에 있는 typedef 와 minishell.h에 합쳐 넣기
typedef	char	*(*t_trim_fn)(char **, t_cmp);

char	*trim(char	**cmd_arg, t_cmp cmp)
{
	char	*tmp;
	char	*start;
	char	*result;

	tmp = 0;
	result = 0;
	start = *cmd_arg;
	if (cmp(**cmd_arg))
		start = ++(*cmd_arg);
	while (**cmd_arg && !cmp(**cmd_arg))
		(*cmd_arg)++;
	if (*cmd_arg > start)
	{
		tmp = ft_substr(start, 0, *cmd_arg - start);
		if (!tmp)
			return (0);
		result = tmp;
	}
	else
		result = ft_strdup("");
	if (**cmd_arg && cmp != is_quote && cmp(**cmd_arg))
		(*cmd_arg)++;
	return (result);
}

/**
 * @brief
 * 
 * @param cmd_arg 
 */
char	*trim_outer_quotes(char *cmd_arg)
{
	char			*trimmed;
	char			*result;
	char			*tmp;
	t_state	state;
	const t_cmp		cmp[] = {
		is_quote,
		is_squote,
		is_dquote,
	};

	result = 0;
	state = update_state(*cmd_arg);
	while (*cmd_arg)
	{
		tmp = result;
		trimmed = trim(&cmd_arg, cmp[state]);
		if (!trimmed)
			return (0);
		result = ft_strjoin(result, trimmed);
		state = update_state(*cmd_arg);
		free(tmp);
		free(trimmed);
		if (!result)
			return (0);
		if (META_CH <= state && state <= END)
			break ;
	}
	if (!result)
		return (ft_strdup(""));
	return (result);
}

/**
 * @brief Remove the outer quotes from node->cmd_args.
 * 
 * @param node		node to remove quotes
 * @return char**	new arr of cmd_args, without quotes
 */
char    **remove_quotes(t_node *node)
{
	char	**result;
	t_size	idx;

	idx = 0;
	if (!node || !node->cmd_args)
		return (0);
	result = (char **)ft_calloc(node->num_args + 1, sizeof(char *));
	if (!result)
		return (0);
	while (idx < node->num_args)
	{
		result[idx] = trim_outer_quotes(node->cmd_args[idx]);
		if (!result[idx])
		{
			while(idx-- > 0)
				free(result[idx]);
			free(result);
			return (0);
		}
		idx++;
	}
	ft_arrfree(node->cmd_args);
	return (result);
}
