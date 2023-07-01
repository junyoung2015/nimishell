/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/20 13:21:43 by jusohn            #+#    #+#             */
/*   Updated: 2023/05/01 19:55:29 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	free_split(char **ptr)
{
	int	i;

	i = 0;
	while (ptr && ptr[i])
		free(ptr[i++]);
	if (ptr)
		free(ptr);
	return (0);
}

static char	*ft_word_cpy(int *idx, char *temp, char *str, char c)
{
	int	i;
	int	len;

	len = *idx;
	while (str[len] && !(str[len] == c))
		len++;
	temp = (char *) ft_calloc((len - *idx + 1), sizeof(char));
	if (!temp)
		return (0);
	i = 0;
	len = *idx;
	while (str[len] && !(str[len] == c))
	{
		temp[i] = str[len];
		len++;
		i++;
	}
	*idx = --len;
	return (temp);
}

static char	**ft_assign_word(char **temp, char *str, char c)
{
	int	i;
	int	temp_i;
	int	flag;

	i = 0;
	temp_i = 0;
	flag = 0;
	while (str[i])
	{
		if (!flag && !(str[i] == c))
		{
			temp[temp_i] = ft_word_cpy(&i, temp[temp_i], str, c);
			if (!temp[temp_i])
			{
				free_split(temp);
				return (NULL);
			}
			flag = 1;
			temp_i++;
		}
		else if (str[i] == c)
			flag = 0;
		i++;
	}
	return (temp);
}

static int	ft_count_words(char *str, char c)
{
	int	i;
	int	len;
	int	flag;

	i = 0;
	len = 0;
	flag = 0;
	while (str[i])
	{
		if (str[i] == c)
			flag = 0;
		else if (!flag && !(str[i] == c))
		{
			flag = 1;
			len++;
		}
		i++;
	}
	return (len);
}

char	**ft_split(char const *str, char c)
{
	int		len;
	char	**temp;

	temp = 0;
	len = ft_count_words((char *) str, c);
	temp = (char **) ft_calloc((len + 1), sizeof(char *));
	if (!temp)
		return (NULL);
	return (ft_assign_word(temp, (char *) str, c));
}
