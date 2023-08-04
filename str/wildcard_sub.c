#include "minishell.h"

// t_bool	is_wildcard(char ch)
// {
// 	return (ch == '*');
// }

// t_bool	is_wsplit(char ch)
// {
// 	return (is_quote(ch) || is_wildcard(ch));
// }

// t_bool	is_wildcard_expansion(char *cmd_arg)
// {
//     char            *tmp;
// 	t_bool			in_quotes;
// 	in_quotes = 0;

// 	tmp = cmd_arg;
// 	while (*tmp)
// 	{
// 		if (is_squote(*tmp))
// 			in_quotes = !in_quotes;  // flip the value if we encounter a quote
// 		if (is_wildcard(*tmp) && !in_quotes)  // only return true if we're not inside quotes
// 			return (TRUE);
// 		tmp++;
// 	}
//     return (FALSE);
// }

// t_size append_str(char ***arr, char *str, t_size size)
// {
// 	if (!str)
// 		return (size);
//     char **tmp = (char **)ft_calloc((size + 2), sizeof(char *));
//     if (!tmp)
//         return (0);
//     for (t_size i = 0; i < size; i++)
//         tmp[i] = (*arr)[i];
//     tmp[size] = str;
// 	if (*arr)
// 		free(*arr);
//     *arr = tmp;
//     return (size + 1);
// }

// char    **match_pattern(char *cmd_arg)
// {
//     DIR             *dir;
//     struct dirent   *entry;
//     char            *tmp;
//     char            **result;
//     char            *before;
//     char            *after;
//     t_size          size;

// 	size = 0;
// 	result = 0;
//     dir = opendir(".");
//     if (!dir)
//         return (0);
//     entry = readdir(dir);
//     tmp = ft_strchr(cmd_arg, '*');
//     if (!tmp)
//     {
//         result = (char **)ft_calloc(2, sizeof(char *));
// 		if (!result)
// 			return (0);
//         result[0] = ft_strdup(cmd_arg);
//         return (result);
//     }
//     before = ft_substr(cmd_arg, 0, tmp - cmd_arg);
//     tmp++;
//     after = ft_substr(tmp, 0, ft_strlen(tmp));
//     while (entry)
//     {
//         if (entry->d_name[0] != '.' &&
// 			ft_strncmp(entry->d_name, before, ft_strlen(before) == 0) &&
//             ft_strncmp(after, entry->d_name + ft_strlen(entry->d_name) - ft_strlen(after), ft_strlen(after)) == 0)
//         {
//             size = append_str(&result, ft_strdup(entry->d_name), size);
//             if (!size)
//             {
//                 free(before);
//                 free(after);
// 				closedir(dir);
//                 return (0);
//             }
//         }
//         entry = readdir(dir);
//     }
//     closedir(dir);
//     free(before);
//     free(after);
// 	if (!result)
// 	{
// 		result = (char **)ft_calloc(2, sizeof(char *));
// 		if (!result)
// 			return (0);
// 		result[0] = ft_strdup(cmd_arg);
// 	}
//     return (result);
// }

// t_size join_arr(char ***arr, char **new_arr, t_size size)
// {
//     t_size	new;
// 	t_size	idx;

// 	if (!new_arr)
// 		return (size);
// 	idx = -1;
// 	new = size;
//     while (new_arr[new - size])
//         new++;
//     char **tmp = (char **)ft_calloc((new + 1), sizeof(char *));
//     if (!tmp)
//         return (0);
// 	while (++idx < size)
// 		tmp[idx] = (*arr)[idx];
// 	while (size < new)
// 	{
// 		tmp[size] = new_arr[size - idx];
// 		size++;
// 	}
// 	free(*arr);
//     free(new_arr);
//     *arr = tmp;
//     return (new);
// }

// char    **wildcard_substitution(t_node *node)
// {
//     char    **result;
//     char    **new_result;
//     t_size  idx;
// 	t_size	size;

//     if (!node || !node->cmd_args)
//         return (0);
//     result = (char **)ft_calloc(1, sizeof(char *));
//     if (!result)
//         return (node->cmd_args);
//     idx = 0;
// 	size = 0;
//     while (node->cmd_args[idx])
//     {
//         new_result = match_pattern(node->cmd_args[idx]);
//         if (!new_result)
//         {
// 			idx = -1;
// 			while (++idx < size)
// 				free(result[idx]);
//             free(result);
//             return (node->cmd_args);
//         }
//         size = join_arr(&result, new_result, size);
// 		printf("size: %llu\n", size);
//         if (!size)
//         {
// 			idx = -1;
// 			while (++idx < size)
// 				free(result[idx]);
//             free(result);
//             return (node->cmd_args);
//         }
//         idx++;
//     }
// 	node->num_args = size;
// 	idx = -1;
// 	while (++idx < size)
// 		free(node->cmd_args[idx]);
// 	free(node->cmd_args);
// 	for (t_size i = 0; i < size; i++)
// 		printf("%s\n", result[i]);
//     return (result);
// }
