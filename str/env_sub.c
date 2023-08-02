
#include "minishell.h"

// TODO: 밑의 typedef 를 trim_quotes 에 있는 typedef 와 minishell.h에 합쳐 넣기
typedef	char	*(*t_process_fn)(char **);

/**
 * @brief Check whether 'ch' is a valid character for an environment variable.
 * 
 * @param ch		character to check
 * @return t_bool	TRUE if 'ch' is a valid character for an environment variable,
 * 					FALSE otherwise.
 */
t_bool	is_env_var(char ch)
{
	return (is_alnum(ch) || ch == '_');
}

t_bool	is_dollar(char ch)
{
	return (ch == '$');
}

char	*ft_getenv(char *env_var)
{
	t_size	i;
	char	*start;
	char	*end;
	char	*tmp;

	i = 0;
	while (g_info.env && g_info.env[i])
	{
		start = g_info.env[i];
		end = ft_strchr(start, '=');
		if (!end)
			return (0);
		tmp = ft_substr(start, 0, end - start);
		if (tmp)
		{
			if (ft_strcmp(tmp, env_var) == 0)
			{
				free(tmp);
				return (end + 1);
			}
		}
		free(tmp);
		i++;
	}
	return (0);
}

char	*substitute(char *env_var)
{
	char	*result;
	char	*value;

	result = 0;
	value = ft_getenv(env_var);
	if (value)
		result = ft_strdup(value);
	return (result);
}

char	*process_normal(char **input)
{
	char	*start;
	char	*tmp;
	char	*env_var;
	char	*result;

	tmp = 0;
	env_var = 0;
	start = *input;
	while(**input && !is_dollar(**input))
		(*input)++;
	if (*input > start)
	{
		tmp = ft_substr(start, 0, *input - start);
		if (!tmp)
			return (0);
	}
	result = tmp;
	if (!**input || !is_dollar(**input))
		return (tmp);
	start = ++(*input);
	while(**input && is_env_var(**input))
		(*input)++;
	if (**input == '?')
	{
		result = ft_strjoin(tmp, ft_itoa(g_info.exit_status));
		(*input)++;
	}
	else if (is_dollar(*((*input) - 1)) && **input && !is_env_var(**input)) // do I need to hcek \0 here?
	{
		env_var = ft_substr(*input - 1, 0, 1);
		if (!env_var)
			return (result);
		result = ft_strjoin(tmp, env_var);
		// (*input)++;
		// free(env_var);
	}
	else if (*input > start)
	{
		env_var = ft_substr(start, 0, *input - start);
		if (!env_var)
			return (result);
		result = ft_strjoin(tmp, substitute(env_var));
		// free(env_var);
	}
	else
	{
		env_var = ft_substr(*input - 1, 0, 1);
		if (!env_var)
			return (result);
		result = ft_strjoin(tmp, env_var);
		// (*input)++;
		// free(env_var);
	}
	free(env_var);
	free(tmp);
	return (result);
}

char	*process_squote(char **input)
{
	char	*start;
	char	*result;

	start = *input;
	(*input)++;
	while(**input && !is_squote(**input))
		(*input)++;
	if (is_squote(**input))
		(*input)++;
	result = ft_substr(start, 0, *input - start + is_squote(**input));
	if (!result)
		return (0);
	return (result);
}

/**
 * @brief Process double-quoted string, substituting environment variables.
 * 
 * @param input 
 * @param state 
 * @return char* 
 */
char	*process_dquote(char **input)
{
	char	*start;
	char	*tmp;
	char	*env_var;
	char	*result;

	tmp = 0;
	start = *input;
	result = 0;
	(*input)++;
	while(**input && !is_dquote(**input) && !is_dollar(**input))
		(*input)++;
	if (*input > start || is_dquote(**input))
	{
		tmp = ft_substr(start, 0, *input - start + is_dquote(**input));
		if (!tmp)
			return (0);
		if (is_dollar(**input))
		{
			(*input)++;
			if (**input == '?')
			{
				result = ft_strjoin(tmp, ft_itoa(g_info.exit_status));
				(*input)++;
				// free(tmp);
				// return (result);
			}
			else if (!**input)
			{
				result = ft_strjoin(tmp, *input - 1);
				// free(tmp);
				// return (result);
			}
			else if (is_env_var(**input))
			{
				start = *input;
				while(**input && is_env_var(**input))
					(*input)++;
				env_var = ft_substr(start, 0, *input - start);
				if (!env_var)
					return (tmp);
				result = ft_strjoin(tmp, substitute(env_var));
				free(env_var);
				// free(tmp);
				// return (result);
			}
			else
			{
				result = ft_strjoin(tmp, *input - 1);
				// free(tmp);
				// return (result);
			}
			free(tmp);
			return (result);
		}
		else if (is_dquote(**input))
		{
			(*input)++;
			return (tmp);
		}
		else if (!**input)
		{
			result = tmp;
			// free(tmp);
			// return (result);
		}
	}
	return (result);
}

/**
 * @brief	Look for $ in the cmg_args, while checking if it's quoted or not.
 * 			Keep join the string using ft_strjoin. If it is a valid env var,
 *			substitute it with the value of the environment variable and join.
 * @param cmd_arg	argument to check
 * @return
 */
char	*check_env_var(char *cmd_arg)
{
	char			*result;
	char			*substr;
	char			*tmp;
	t_token_state	state;
	const t_process_fn	state_fn[] = { 
		process_normal,
		process_squote,
		process_dquote,
	};

	result = 0;
	state = update_state(*cmd_arg);
	while (*cmd_arg && state != END)
	{
		tmp = result;
		substr = state_fn[state](&cmd_arg);
		if (!substr)
			return (result);
		result = ft_strjoin(result, substr);
		free(tmp);
		free(substr);
		state = update_state(*cmd_arg);
		if (META_CH <= state && state < END)
			break ;
	}
	return (result);
}

/**
 * @brief	Find and substitute environment variables in the node->cmd_args.
 * 			If the string is not quoted or double-quoted, search for $ and
 * 			substitute the environment variable ONLY.
 * 			If the string is single-quoted, do nothing.
 *
 * @param node		node to substitute appropriate cmd_args with env var
 * @return char**	substituted cmd_args
 */
char	**env_substitution(t_node *node)
{
	char    **result;
	t_size  idx;

	idx = 0;
	if (!node->cmd_args)
		return (0);
	result = (char **)ft_calloc(node->num_args + 1, sizeof(char *));
	if (!result)
		return (0);
	while (idx < node->num_args)
	{
		result[idx] = check_env_var(node->cmd_args[idx]);
		if (!result[idx])
		{
			while(--idx != 0)
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
