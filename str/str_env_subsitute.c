
#include "minishell.h"

typedef	char	*(*t_env_fn)(char **, t_token_state *);

/**
 * @brief Check whether 'ch' is a valid character for an environment variable.
 * 
 * @param ch		character to check
 * @return t_bool	TRUE if 'ch' is a valid character for an environment variable,
 * 					FALSE otherwise.
 */
t_bool	is_env_var(char ch)
{
	return (ft_isalnum(ch) || ch == '_');
}

t_bool	is_dollar(char ch)
{
	return (ch == '$');
}

char	*ft_getenv(char *env_var)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(env_var);
	while (g_info.env && g_info.env[i])
	{
		if (ft_strncmp(g_info.env[i], env_var, len) == 0)
			return (g_info.env[i] + len + 1);
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

char	*process_normal(char **input, t_token_state *state)
{
	char	*start;
	char	*tmp;
	char	*env_var;
	char	*result;

	tmp = 0;
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
	if (*input > start)
	{
		env_var = ft_substr(start, 0, *input - start);
		if (!env_var)
			return (result);
		result = ft_strjoin(tmp, substitute(env_var));
		free(env_var);
	}
	free(tmp);
	// if (!result)
	// 	return (0);
	return (result);
}

char	*process_squote(char **input, t_token_state *state)
{
	char	*start;
	char	*result;

	start = *input;
	(*input)++;
	while(**input && !is_squote(**input))
		(*input)++;
	result = ft_substr(start, 0, *input - start + 1);
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
char	*process_dquote(char **input, t_token_state *state)
{
	char	*start;
	char	*tmp;
	char	*env_var;
	char	*result;

	tmp = 0;
	start = *input;
	while(**input && !is_dquote(**input) && !is_dollar(**input))
		(*input)++;
	if (*input > start)
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
	}
	return (result);
}

// char	*process_meta(char **input, t_token_state *state)
// {

// }

// char	*process_wspace(char **input, t_token_state *state)
// {

// }

/**
 * @brief	Look for $ in the cmg_args, while checking if it's quoted or not.
 * 			Keep join the string using ft_strjoin. If it is a valid env var,
 *			substitute it with the value of the environment variable and join.
 * @param cmd_arg	argument to check
 * @return
 */
char	*check_env_var(char *cmd_arg)
{
	t_bool			is_squote;
	char			*result;
	char			*substr;
	char			*tmp;
	t_token_state	state;
	const t_env_fn	env_fn[] = {
		process_normal,
		process_squote,
		process_dquote,
		// process_meta,
		// process_wspace,
	};

	result = 0;
	is_squote = FALSE;
	state = update_state(*cmd_arg);
	while (*cmd_arg && state != END)
	{
		tmp = result;
		substr = env_fn[state](&cmd_arg, &state);
		if (!substr)
			return (result);
		result = ft_strjoin(result, substr);
		free(tmp);
		state = update_state(*cmd_arg);
		if (META_CH <= state && state < END)
			break ;
		// cmd_arg++;
	}
	return (result);
}

/**
 * @brief	Find and substitute environment variables in the node->cmd_args.
 * 			If the string is not quoted or double-quoted, search for $ and
 * 			substitute the environment variable ONLY.
 * 			If the string is single-quoted, do nothing.
 *
 * @param node		node to substitute env var
 * @return char**	substituted cmd_args
 */
char	**check_and_substitute_env(t_node *node)
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
			// free result up to now
			return (0);
		}
		idx++;
	}
	return (result);
}
