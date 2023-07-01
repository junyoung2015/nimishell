/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils_create.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 13:59:34 by jusohn            #+#    #+#             */
/*   Updated: 2023/06/15 13:59:56 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strtrim(char *str, char c)
{
	char	*result;
	t_size	start;
	t_size	end;
	t_size	idx;

	if (!(str && *str))
		return (0);
	start = 0;
	end = ft_strlen(str);
	while (str[start] == c)
		start++;
	while (str[end] == c)
		end--;
	result = (char *) ft_calloc((end - start + 1), sizeof(char));
	if (!result)
		return (0);
	idx = 0;
	while (start < end)
		result[idx++] = str[start++];
	return (result);
}

char	*ft_strdup(char *src)
{
	char	*dup;
	t_size	len;
	t_size	idx;

	idx = 0;
	len = ft_strlen(src);
	dup = (char *) ft_calloc(len + 1, sizeof(char));
	if (!dup)
		return (NULL);
	while (idx <= len)
	{
		dup[idx] = src[idx];
		idx++;
	}
	return (dup);
}
