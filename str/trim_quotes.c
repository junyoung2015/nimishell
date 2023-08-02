#include "minishell.h"

// TODO: 밑의 typedef 를 env_sub 에 있는 typedef 와 minishell.h에 합쳐 넣기
typedef	char	*(*t_trim_fn)(char **);

/**
 * @brief Get substring of cmd_arg until the next quote.
 * 
 * @param cmd_arg	arg to get substring
 * @return char*	substring of cmd_arg until the next quote
 */
char	*trim_normal(char **cmd_arg)
{
	char	*tmp;
	char	*start;
	char	*result;

	tmp = 0;
	result = 0;
	start = *cmd_arg;
	while (**cmd_arg && !is_quote(**cmd_arg))
		(*cmd_arg)++;
	if (*cmd_arg > start)
	{
		tmp = ft_substr(start, 0, *cmd_arg - start);
		if (!tmp)
			return (0);
		result = tmp;
		// result = ft_strjoin(result, tmp);
		// free(tmp);
	}
	else
		result = ft_strdup("");
	return (result);
}

/**
 * @brief Remove outer single quotes from the cmd_arg
 * 
 * @param cmd_arg	arg to remove single quotes
 * @return char*	new string without single quotes
 */
char	*trim_squote(char **cmd_arg)
{
	char	*tmp;
	char	*start;
	char	*result;

	tmp = 0;
	result = 0;
	start = ++(*cmd_arg);
	while (**cmd_arg && !is_squote(**cmd_arg))
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
	if (**cmd_arg)
		(*cmd_arg)++;
	return (result);
}

/**
 * @brief Remove outer double quotes from the cmd_arg
 * 
 * @param cmd_arg	arg to remove double quotes
 * @return char*	new string without double quotes
 */
char	*trim_dquote(char **cmd_arg)
{
	char	*tmp;
	char	*start;
	char	*result;

	tmp = 0;
	result = 0;
	start = ++(*cmd_arg);
	while (**cmd_arg && !is_dquote(**cmd_arg))
		(*cmd_arg)++;
	if (*cmd_arg > start)
	{
		tmp = ft_substr(start, 0, *cmd_arg - start);
		if (!tmp)
			return (0);
		result = tmp;
		if (**cmd_arg)
			(*cmd_arg)++;
	}
	else
		result = ft_strdup("");
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
	t_token_state	state;
	const t_trim_fn	state_fn[] = { 
		trim_normal,
		trim_squote,
		trim_dquote,
	};

	result = 0;
	state = update_state(*cmd_arg);
	while (*cmd_arg)
	{
		tmp = result;
		trimmed = state_fn[state](&cmd_arg);
		if (!trimmed)
			return (0);
		result = ft_strjoin(result, trimmed);
		state = update_state(*cmd_arg);
		free(tmp);
		free(trimmed);
		if (META_CH <= state && state <= END)
			break ;
	}
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
	if (!node->cmd_args)
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
		free(node->cmd_args[idx]);
		idx++;
	}
	free(node->cmd_args);
	return (result);
}
