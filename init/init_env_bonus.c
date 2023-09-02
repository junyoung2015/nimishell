/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 00:09:36 by sejinkim          #+#    #+#             */
/*   Updated: 2023/09/02 15:56:45 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

char	*handle_shlvl(char *env_var)
{
	t_size	i;
	char	*tmp;
	char	*shlvl;

	tmp = ft_strchr(env_var, '=');
	if (!tmp)
		return (ft_strdup(SHLVL_INIT));
	tmp++;
	i = 0;
	while (tmp[i] && is_number(tmp[i]))
		i++;
	if (i == ft_strlen(tmp))
	{
		shlvl = ft_itoa(ft_atoi(tmp) + 1);
		if (!shlvl)
			exit_err_msg(1, 0, MALLOC_ERR, 0);
		tmp = ft_strjoin(SHLVL, shlvl);
		free(shlvl);
		if (!tmp)
			exit_err_msg(1, 0, MALLOC_ERR, 0);
	}
	else
		tmp = ft_strdup(SHLVL_INIT);
	return (tmp);
}

void	init_env(char **envp, t_sh_info *info)
{
	char	*tmp;
	t_bool	flag;

	flag = FALSE;
	while (envp && envp[info->env_cnt])
	{
		if (!strncmp(envp[info->env_cnt], OLDPWD, 7))
		{
			flag = TRUE;
			info->env_cnt++;
			continue ;
		}
		if (!strncmp(envp[info->env_cnt], SHLVL, 6))
			tmp = handle_shlvl(envp[info->env_cnt]);
		else
			tmp = ft_strdup(envp[info->env_cnt]);
		if (!tmp)
			exit_err_msg(1, 0, MALLOC_ERR, 0);
		info->env_cnt = ft_arr_append_back(&g_env, tmp, info->env_cnt);
		if (!g_env)
			exit_err_msg(1, 0, MALLOC_ERR, 0);
	}
	if (!flag)
		info->env_cnt++;
}
