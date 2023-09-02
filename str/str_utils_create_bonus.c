/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils_create_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/02 15:53:55 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/02 15:53:55 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

char	*ft_strtrim(char const *s1, char const *s2)
{
	t_size	idx;

	if (!s1 || !s2)
		return (NULL);
	while (*s1 && ft_strchr(s2, *s1))
		s1++;
	idx = ft_strlen(s1);
	while (idx > 0 && ft_strchr(s2, s1[idx]))
		idx--;
	return (ft_substr(s1, 0, idx + 1));
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
