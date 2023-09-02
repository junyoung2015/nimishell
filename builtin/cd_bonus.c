/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_bonus.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 19:50:41 by sejinkim          #+#    #+#             */
/*   Updated: 2023/09/02 15:54:44 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_bonus.h"

t_bool	cd_to_home(t_exec_info *info)
{
	char	*home;

	home = get_env("HOME");
	if (!home)
	{
		write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
		info->exit_code = EXIT_FAILURE;
		return (FALSE);
	}
	return (try_chdir(home, info));
}

t_bool	cd_to_oldpwd(t_exec_info *info)
{
	char	*oldpwd;

	oldpwd = get_env("OLDPWD");
	if (!oldpwd)
	{
		write(STDERR_FILENO, "minishell: cd: OLDPWD not set\n", 30);
		info->exit_code = EXIT_FAILURE;
		return (FALSE);
	}
	return (try_chdir(oldpwd, info));
}

t_bool	cd_to_path_with_home(char *path, t_exec_info *info)
{
	char	*home;
	char	*full_path;
	t_bool	bool;

	home = get_env("HOME");
	if (!home)
	{
		write(STDERR_FILENO, "minishell: cd: HOME not set\n", 28);
		info->exit_code = EXIT_FAILURE;
		return (FALSE);
	}
	full_path = ft_strjoin(home, path + 1);
	if (!full_path)
	{
		err("minishell: malloc", info);
		return (FALSE);
	}
	bool = try_chdir(full_path, info);
	free(full_path);
	return (bool);
}

t_bool	cd(t_node *node, t_exec_info *info)
{
	char	*cwd;
	t_bool	status;

	status = TRUE;
	if (node->num_args == 1)
		status = cd_to_home(info);
	else if (node->num_args >= 2)
	{
		if (ft_strcmp(node->cmd_args[1], "~") == 0)
			status = cd_to_home(info);
		else if (ft_strcmp(node->cmd_args[1], "-") == 0)
			status = cd_to_oldpwd(info);
		else if (ft_strncmp(node->cmd_args[1], "~/", 2) == 0)
			status = cd_to_path_with_home(node->cmd_args[1], info);
		else
			status = try_chdir(node->cmd_args[1], info);
	}
	cwd = getcwd(0, 0);
	update_pwd(cwd, info);
	free(cwd);
	return (status);
}
