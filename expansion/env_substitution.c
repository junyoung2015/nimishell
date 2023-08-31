/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_substitution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:35:09 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/30 18:04:07 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"

char	**trim_single_char(char **in, char *tmp)
{
	char	**result;
	char	*character;

	result = 0;
	result = ft_calloc(2, sizeof(char **));
	if (!result)
		return (0);
	character = ft_substr(*in - 1, 0, 1);
	if (!character)
		return (result);
	result[0] = ft_strjoin(tmp, character);
	free(character);
	return (result);
}

char	**handle_dollar_sign(char **in, char *tmp, char *quo, t_exec_info *info)
{
	char	**result;

	(*in)++;
	result = 0;
	if (**in == '?')
		result = sub_exit_code(in, tmp, info);
	else if (result && !**in)
	{
		result[0] = ft_strjoin(tmp, *in - 1);
		free(tmp);
	}
	else if (is_env_var(**in))
		result = sub_env_var(in, tmp, quo);
	else
		result = trim_single_char(in, tmp);
	// free(tmp);	
	return (result);
}

/**
 * @brief	Look for $ in the cmg_args, while checking if it's quoted or not.
 * 			Keep join the string using ft_strjoin. If it is a valid env var,
 *			substitute it with the value of the environment variable and join.
 * @param cmd_arg	argument to check
 * @return
 */
char	**check_env_var(char *cmd_arg, t_exec_info *info)
{
	char			*tmp;
	char			**result;
	char			**substrs;
	t_size			len;
	t_state			state;
	const t_env_fn	state_fn[] = {env_str, env_squote, env_dquote, env_str};

	len = 0;
	result = 0;
	state = update_state(*cmd_arg);
	if (state == END)
	{
		result = ft_calloc(2, sizeof(char **));
		if (!result)
			return (0);
		*result = ft_strdup(cmd_arg);
		return (result);
	}
	while (*cmd_arg && state != END)
	{
		substrs = state_fn[state](&cmd_arg, info);
		if (ft_arrlen(substrs) > 1 || !len)
			len = ft_arrcat(&result, substrs, len);
		else
		{
			tmp = result[len - 1];
			result[len - 1] = ft_strjoin(result[len - 1], substrs[0]);
			free(tmp);
			// free(substrs[0]);
			ft_arrfree(substrs);
		}
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
	t_size	idx;
	t_size	len;
	char	**tmp;
	char	**result;

	len = 0;
	idx = -1;
	if (!node || !node->cmd_args)
		return (0);
	result = (char **)ft_calloc(node->num_args + 1, sizeof(char *));
	if (!result)
		return (0);
	while (++idx < node->num_args)
	{
		tmp = check_env_var(node->cmd_args[idx], info);
		if (!tmp)
		{
			ft_arrfree(result);
			return (0);
		}
		len = ft_arrcat(&result, tmp, len);
	}
	ft_arrfree(node->cmd_args);
	node->num_args = len;
	return (result);
}
