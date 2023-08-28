/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_substitution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:35:09 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/28 20:51:34 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	*trim_single_char(char **in, char *tmp)
{
	char	*result;
	char	*character;

	result = 0;
	character = ft_substr(*in - 1, 0, 1);
	if (!character)
		return (result);
	result = ft_strjoin(tmp, character);
	free(character);
	return (result);
}

char	*handle_dollar_sign(char **in, char *tmp, char *quo, t_exec_info *info)
{
	char	*result;

	(*in)++;
	result = 0;
	if (**in == '?')
		result = sub_exit_code(in, tmp, info);
	else if (!**in)
		result = ft_strjoin(tmp, *in - 1);
	else if (is_env_var(**in))
		result = sub_env_var(in, tmp, quo);
	else
		result = trim_single_char(in, tmp);
	free(tmp);
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
			while (idx > 0)
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
