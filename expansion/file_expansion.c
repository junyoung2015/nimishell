/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_expansion.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:38:38 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/28 20:59:13 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief   Find all files that match the pattern
 * 
 * @param files		List of files in current direcotry
 * @param pattern	Pattern of the files to search for
 * @return char**	List of files that match the pattern
 */
char	**find_matching_files(t_search *info, char **pat)
{
	t_size	i;
	char	**result;
	char	*trimmed;
	char	*tmp;
	t_cmp	cmp;

	i = 0;
	result = 0;
	while (pat[i])
		match_except_last(info, pat, &i, &cmp);
	if (i && !(is_wildcard(*(pat[i - 1])) && ft_strlen(pat[i - 1]) == 1))
	{
		trimmed = pat[i - 1];
		if (is_quote(*(pat[i - 1])))
		{
			tmp = pat[i - 1];
			trimmed = trim(&tmp, cmp, 0);
		}
		match_pattern_last(info, trimmed, ft_strlen(trimmed));
	}
	ft_arrfree(pat);
	if (!info->files)
		return (0);
	ft_arrcat(&result, info->files, 0);
	return (result);
}

t_size	file_not_found(char ***result, t_size len, char *cmd_arg)
{
	char	**new;

	new = (char **)ft_calloc(2, sizeof(char *));
	if (!new)
		return (0);
	new[0] = ft_strdup(cmd_arg);
	len = ft_arrcat(result, new, len);
	return (len);
}

t_size	handle_wildcard_expansion(t_size len, char *cmd_arg, char ***result)
{
	char		**new;
	t_search	*info;

	new = get_search_pattern(cmd_arg, 0);
	if (!new)
		return (0);
	info = init_search_info(new[0]);
	if (!info)
		return (0);
	info->files = find_matching_files(info, new);
	if (!info->files || (*(info->files) && !(*(info->files))[0]))
		len = file_not_found(result, len, cmd_arg);
	else
		len = ft_arrcat(result, info->files, len);
	free_search_info(info);
	if (!result)
		return (0);
	return (len);
}

/**
 * @brief   Expand the wildcard inside cmd_args. Also expand the string inside
 *        quotes if needed.
 * 
 * @param node      node to expand
 * @return char**   expanded string
 */
char	**str_expansion(t_node *node)
{
	t_size		idx;
	t_size		len;
	char		**result;

	idx = 0;
	len = 0;
	result = 0;
	if (!node || !node->cmd_args)
		return (0);
	while (idx < node->num_args)
	{
		if (is_wildcard_expansion(node->cmd_args[idx]))
			len = handle_wildcard_expansion(len, node->cmd_args[idx], &result);
		else
			len = file_not_found(&result, len, node->cmd_args[idx]);
		idx++;
	}
	node->num_args = len;
	ft_arrfree(node->cmd_args);
	return (result);
}
