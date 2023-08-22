
#include "executor.h"

t_bool	is_number(int c)
{
	return ('0' <= c && c <= '9');
}

t_bool	is_alpha(int c)
{
	return (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'));
}
t_bool	is_alnum(int c)
{
	return (is_number(c) || is_alpha(c));
}

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
	while (g_env && g_env[i])
	{
		start = g_env[i];
		if (start)
			end = ft_strchr(start, '=');
		else
			return (0);
		if (!end)
			return (0);
		tmp = ft_substr(start, 0, end - start);
		if (tmp && ft_strcmp(tmp, env_var) == 0)
		{
			free(tmp);
			return (start);
		}
		free(tmp);
		i++;
	}
	return (0);
}

char	*wrap_env_var(char *env_var, char *quote)
{
	char	*tmp;
	char	*wrapper;

	tmp = env_var;
	if (is_squote(*env_var))
		wrapper = "\"";
	else
		wrapper = "'";
	env_var = ft_strjoin(wrapper, env_var);
	free(tmp);
	tmp = env_var;
	env_var = ft_strjoin(env_var, wrapper);
	free(tmp);
	if (quote)
	{
		tmp = env_var;
		env_var = ft_strjoin(env_var, quote);
		free(tmp);
		tmp = env_var;
		env_var = ft_strjoin(quote, env_var);
		free(tmp);
	}
	return (env_var);
}

char	*substitute(char *env_var, char *quote)
{
	char	*result;
	char	*key;
	char	*value;

	result = 0;
	key = ft_getenv(env_var);
	if (!key)
		return (0);
	value = ft_strchr(key, '=');
	if (value)
	{
		result = ft_strtrim(value + 1, " ");
		result = wrap_env_var(result, quote);
	}
	return (result);
}

char	*sub_exit_code(char **input, char *tmp, t_exec_info *info)
{
	char	*substituted;
	char	*result;

	result = 0;
	if (**input == '?')
	{
		substituted = ft_itoa(info->prev_exit_code);
		result = ft_strjoin(tmp, substituted);
		(*input)++;
		free(substituted);
	}
	return (result);
}

char	*sub_env_var(char **input, char *tmp, char *quote)
{
	char	*substituted;
	char	*result;
	char	*env_var;
	char	*start;

	result = 0;
	start = *input;
	while(**input && is_env_var(**input))
		(*input)++;
	env_var = ft_substr(start, 0, *input - start);
	if (!env_var)
		return (0);
	substituted = substitute(env_var, quote);
	result = ft_strjoin(tmp, substituted);
	free(substituted);
	free(env_var);
	return (result);
}

char	*trim_single_char(char **input, char *tmp)
{
	char	*result;
	char	*character;

	result = 0;
	character = ft_substr(*input - 1, 0, 1);
	if (!character)
		return (result);
	result = ft_strjoin(tmp, character);
	free(character);
	return (result);
}

char	*handle_dollar_sign(char **input, char *tmp, char *quote, t_exec_info *info)
{
	char	*result;

	(*input)++;
	result = 0;
	if (**input == '?')
		result = sub_exit_code(input, tmp, info);
	else if (!**input)
		result = ft_strjoin(tmp, *input - 1);
	else if (is_env_var(**input))
		result = sub_env_var(input, tmp, quote);
	else
		result = trim_single_char(input, tmp);
	free(tmp);
	return (result);
}

char	*env_str(char **input, t_exec_info *info)
{
	char	*start;
	char	*tmp;
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
	result = handle_dollar_sign(input, tmp, 0, info);
	return (result);
}

char	*env_squote(char **input, t_exec_info *info)
{
	char	*start;
	char	*result;

	(void)info;
	start = *input;
	(*input)++;
	while(**input && !is_squote(**input))
		(*input)++;
	result = ft_substr(start, 0, *input - start + is_squote(**input));
	if (!result)
		return (0);
	if (is_squote(**input))
		(*input)++;
	return (result);
}

/**
 * @brief Process double-quoted string, substituting environment variables.
 * 
 * @param input 
 * @param state 
 * @return char* 
 */
char	*env_dquote(char **input, t_exec_info *info)
{
	char	*start;
	char	*end;
	char	*tmp;
	char	*prev;
	char	*result;

	tmp = 0;
	result = 0;
	start = (*input)++;
	end = *input;
	while (*end && !is_dquote(*end))
		end++;
	while(**input && start < end)
	{
		while(**input && !is_dquote(**input) && !is_dollar(**input))
			(*input)++;
		if (*input > start || is_dquote(**input) || is_dollar(**input))
		{
			prev = result;
			tmp = ft_substr(start, 0, *input - start);
			if (!tmp)
				return (0);
			result = tmp;
			tmp = ft_strjoin(prev, tmp);
			free(prev);
			free(result);
			result = tmp;
			if (tmp && is_dollar(**input))
				result = handle_dollar_sign(input, tmp, "\"", info);
		}
		start = *input;
	}
	tmp = result;
	result = ft_strjoin(result, "\"");
	free(tmp);
	(*input)++;
	return (result);
}

/**
 * @brief	Look for $ in the cmg_args, while checking if it's quoted or not.
 * 			Keep join the string using ft_strjoin. If it is a valid env var,
 *			substitute it with the value of the environment variable and join.
 * @param cmd_arg	argument to check
 * @return
 */
char	*check_env_var(char *cmd_arg, t_exec_info *info)
{
	char			*result;
	char			*substr;
	char			*tmp;
	t_state			state;
	const t_env_fn	state_fn[] = {env_str, env_squote, env_dquote, env_str};

	result = 0;
	state = update_state(*cmd_arg);
	if (state == END)
		return (ft_strdup(cmd_arg));
	while (*cmd_arg && state != END)
	{
		tmp = result;
		substr = state_fn[state](&cmd_arg, info);
		if (!substr)
			return (result);
		result = ft_strjoin(result, substr);
		free(substr);
		free(tmp);
		state = update_state(*cmd_arg);
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
char	**env_substitution(t_node *node, t_exec_info *info)
{
	char	**result;
	t_size	idx;

	idx = -1;
	if (!node || !node->cmd_args)
		return (0);
	result = (char **)ft_calloc(node->num_args + 1, sizeof(char *));
	if (!result)
		return (0);
	while (++idx < node->num_args)
	{
		result[idx] = check_env_var(node->cmd_args[idx], info);
		if (!result[idx])
		{
			while(idx > 0)
			{
				idx--;
				free(result[idx]);
			}
			free(result);
			return (0);
		}
	}
	ft_arrfree(node->cmd_args);
	return (result);
}
