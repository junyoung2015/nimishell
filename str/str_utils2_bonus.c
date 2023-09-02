/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils2_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/15 13:31:23 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/02 15:58:24 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

char	*ft_strchr(const char *s, int c)
{
	t_size	i;
	char	*tmp;

	i = 0;
	tmp = (char *) s;
	while (tmp[i])
	{
		if (tmp[i] == (char) c)
			return (tmp + i);
		i++;
	}
	if (tmp[i] == (char) c)
		return (tmp + i);
	return (NULL);
}

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	char	*tmp;

	i = 0;
	tmp = (char *) s;
	while (tmp[i])
		i++;
	while (i > 0)
	{
		if (tmp[i] == (char) c)
			return (tmp + i);
		i--;
	}
	if (tmp[i] == (char) c)
		return (tmp + i);
	return (0);
}

char	*ft_strnstr(const char *haystack, const char *needle, t_size n)
{
	size_t	i;
	size_t	j;
	size_t	size;
	char	*src;
	char	*dst;

	i = -1;
	size = n;
	src = (char *) haystack;
	dst = (char *) needle;
	if (!*dst)
		return (src);
	while (src[++i] && 0 < n)
	{
		j = 0;
		if (src[i] == dst[j])
		{
			while (0 < n && i + j < size && dst[j] && src[i + j] == dst[j])
				j++;
			if (dst[j] == '\0')
				return (src + i);
		}
		n--;
	}
	return (0);
}
