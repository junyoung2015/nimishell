#include "minishell.h"

void	ft_arrfree(char **arr)
{
	t_size	idx;

	idx = 0;
	while (arr && arr[idx])
		free(arr[idx++]);
	free(arr);
}

t_size	ft_arrlen(char **arr)
{
	t_size	len;

	len = 0;
	while (arr && arr[len])
		len++;
	return (len);
}

t_size	ft_arrcat(char ***arr, char **new_arr, t_size size)
{
	t_size	new;
	t_ssize	idx;

	if (!new_arr)
		return (size);
	idx = -1;
	new = size;
	while (new_arr[new - size])
		new++;
	char **tmp = (char **)ft_calloc((new + 1), sizeof(char *));
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

t_size	ft_arr_append(char ***arr, char *str, t_size size)
{
	t_size	idx;

	idx = 0;
	if (!str)
		return (size);
	char **tmp = (char **)ft_calloc((size + 2), sizeof(char *));
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
