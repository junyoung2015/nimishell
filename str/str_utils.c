/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/15 13:58:00 by jusohn            #+#    #+#             */
/*   Updated: 2023/06/15 23:07:35 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	t_size	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
}

int	ft_strncmp(const char *s1, const char *s2, t_size n)
{
	t_size	i;

	i = 0;
	while (s1[i] && s1[i] == s2[i] && i < n)
		i++;
	if (i == n)
		return (0);
	return ((unsigned char) s1[i] - (unsigned char) s2[i]);
}

t_size	ft_strlen(const char *str)
{
	t_size	idx;

	if (!str || !*str)
		return (0);
	idx = -1;
	while (str[++idx])
		;
	return (idx);
}

t_size	ft_strlcpy(char *dst, const char *src, t_size dstsize)
{
	t_size	idx;
	t_size	cp_size;

	idx = 0;
	cp_size = dstsize;
	while (cp_size > 1 && src[idx])
	{
		dst[idx] = src[idx];
		idx++;
		cp_size--;
	}
	if (dstsize != 0)
		dst[idx] = '\0';
	return (ft_strlen(src));
}

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
