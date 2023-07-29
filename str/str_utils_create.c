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

char	*ft_substr(char const *s, t_size start, t_size len)
{
	char	*substr;
	t_size	i;
	t_size	len_src;

	i = 0;
	len_src = ft_strlen(s);
	if (!s || start >= len_src)
	{
		substr = (char *) ft_calloc(1, sizeof(char));
		if (!substr)
			return (NULL);
		return (substr);
	}
	else if (len_src < len + start)
		len = len_src - start;
	substr = (char *) ft_calloc(len + 1, sizeof(char));
	if (!substr)
		return (NULL);
	while (s[start] && 0 < len--)
		substr[i++] = s[start++];
	return (substr);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	len_s1;
	size_t	len_s2;

	if (s1 == 0)
		len_s1 = 0;
	else
		len_s1 = ft_strlen(s1);
	if (s2 == 0)
		len_s2 = 0;
	else
		len_s2 = ft_strlen(s2);
	result = (char *) ft_calloc((len_s1 + len_s2 + 1), sizeof(char));
	if (!result)
		return (0);
	while (s1 && *s1)
		*(result++) = *(s1++);
	while (s2 && *s2)
		*(result++) = *(s2++);
	return (result - len_s1 - len_s2);
}
