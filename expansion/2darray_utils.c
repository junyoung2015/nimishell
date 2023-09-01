/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   2darray_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:33:49 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/30 15:34:55 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Free 2d array
 * 
 * @param arr	2d array to free
 */
void	ft_arrfree(char **arr)
{
	t_size	idx;

	idx = 0;
	if (!arr)
		return ;
	while (arr && arr[idx])
		free(arr[idx++]);
	free(arr);
}

/**
 * @brief	Get the size of a 2d array
 * 
 * @param arr		2d array to get size of
 * @return t_size	size of the 2d array
 */
t_size	ft_arrlen(char **arr)
{
	t_size	len;

	len = 0;
	if (!arr || !*arr)
		return (0);
	while (arr && arr[len])
		len++;
	return (len);
}

/**
 * @brief Concatenate two 2d arrays
 * 
 * @param arr		2d array to concatenate to
 * @param new_arr	2d array to concatenate from
 * @param size		size of the 2d array
 * @return t_size	new size of the 2d array
 */
t_size	ft_arrcat(char ***arr, char **new_arr, t_size size)
{
	char	**tmp;
	t_size	new;
	t_size	idx;

	if (!new_arr)
		return (size);
	idx = -1;
	new = size;
	while (new_arr[new - size])
		new++;
	tmp = (char **)ft_calloc((new + 1), sizeof(char *));
	if (!tmp)
		return (0);
	while (++idx < size)
		tmp[idx] = (*arr)[idx];
	while (size < new)
	{
		tmp[size] = new_arr[size - idx];
		size++;
	}
	free(new_arr);
	if (*arr)
		free(*arr);
	*arr = tmp;
	return (new);
}

/**
 * @brief	Append string element to 2d array
 * 
 * @param arr		2d array to append to
 * @param str		string to append
 * @param size		size of the 2d array
 * @return t_size	new size of the 2d array
 */
t_size	ft_arr_append_back(char ***arr, char *str, t_size size)
{
	char	**tmp;
	t_size	idx;

	idx = 0;
	if (!str)
		return (size);
	tmp = (char **)ft_calloc((size + 2), sizeof(char *));
	if (!tmp)
		return (0);
	while (idx < size)
	{
		tmp[idx] = (*arr)[idx];
		idx++;
	}
	tmp[size] = str;
	if (*arr)
		free(*arr);
	*arr = tmp;
	return (size + 1);
}

/**
 * @brief	Append string element to 2d array
 * 
 * @param arr		2d array to append to
 * @param str		string to append
 * @param size		size of the 2d array
 * @return t_size	new size of the 2d array
 */
t_size	ft_arr_append_front(char ***arr, char *str, t_size size)
{
	char	**tmp;
	t_size	idx;

	idx = 0;
	if (!str)
		return (size);
	tmp = (char **)ft_calloc((size + 2), sizeof(char *));
	if (!tmp)
		return (0);
	tmp[idx++] = str;
	while (idx <= size)
	{
		tmp[idx] = (*arr)[idx];
		idx++;
	}
	if (*arr)
		free(*arr);
	*arr = tmp;
	return (size + 1);
}
