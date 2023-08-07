#include "builtin.h"

t_bool	is_in_env(char *key, char *env)
{
	size_t	i;

	i = 0;
	while (key && key[i] != '=')
	{
		if (key[i] != env[i])
			return (FALSE);
		i++;
	}
	if (env[i] == '=')
		return (TRUE);
	return (FALSE);
}

void	append_last(t_node *node, t_exec_info *info)
{
	char	**env;
	size_t	i;

	env = ft_calloc(g_info.env_cnt + 2, sizeof(char *));
	if (!env)
	{
		err("error: malloc", info);
		return ;
	}
	i = 0;
	while (g_info.env && g_info.env[i])
	{
		env[i] = g_info.env[i];
		g_info.env[i++] = NULL;
	}
	env[i++] = node->cmd_args[1];
	node->cmd_args[1] = NULL;
	env[i] = NULL;
	free(g_info.env);
	g_info.env = env;
}

void	print_env()
{
	size_t	i;
	size_t	len;

	i = 0;
	while (g_info.env && g_info.env[i])
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		len = 0;
		while (g_info.env[i][len] != '=')
			len++;
		write(STDOUT_FILENO, g_info.env[i], ++len);
		write(STDOUT_FILENO, "\"", 1);
		write(STDOUT_FILENO, g_info.env[i] + len, ft_strlen(g_info.env[i]) - len);
		write(STDOUT_FILENO, "\"\n", 2);
		i++;
	}
}

void	add_env(t_node *node, t_exec_info *info)
{
	size_t	i;

	i = 0;
	while (g_info.env && g_info.env[i])
	{
		if (is_in_env(node->cmd_args[1], g_info.env[i]))
		{
			free(g_info.env[i]);	
			g_info.env[i] = node->cmd_args[1];
			node->cmd_args[1] = NULL;
			return ;
		}
		i++;
	}
	append_last(node, info);
	if (!info->exit_code)
		g_info.env_cnt += 1;
}

void	export(t_node *node, t_exec_info *info)
{

	if (!node->cmd_args[1])
		print_env();
	else if (node->cmd_args[1][0] == '=')
	{
		write(STDERR_FILENO, "error: export: not a valid identifier\n", 38);
		info->exit_code = EXIT_FAILURE;
	}
	else
		add_env(node, info);
}
