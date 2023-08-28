/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/29 00:09:36 by sejinkim          #+#    #+#             */
/*   Updated: 2023/08/29 00:17:46 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	plus_shllv(char *str)
{
	t_size	i;

	i = 0;
	while (str[i])
		i++;
	while (str[--i] == '9')
		str[i] = '0';
	str[i] += 1;
}

void	dup_env(char **envp, t_sh_info *info, t_size oldpwd, t_size shlvl)
{
	t_size	i;
	t_size	j;

	g_env = ft_calloc((info->env_cnt + 1), sizeof(char *));
	if (!g_env)
		exit_err_msg(1, 0, MALLOC_ERR, 0);
	i = 0;
	j = 0;
	while (i < info->env_cnt)
	{
		if (i == oldpwd)
			j++;
		else
		{
			g_env[i] = ft_strdup(envp[i+j]);
			if (!g_env[i])
				exit_err_msg(1, 0, MALLOC_ERR, 0);
			if (i+j == shlvl)
				plus_shllv(g_env[i]);
		}
		i++;
	}
	g_env[i] = NULL;
}

void	init_env(char **envp, t_sh_info *info)
{
	t_bool	flag;
	t_size	oldpwd_idx;
	t_size	shlvl_idx;

	info->env_cnt = 0;
	flag = FALSE;
	while (envp && envp[info->env_cnt])
	{
		if (!strncmp(envp[info->env_cnt], "OLDPWD=", 7))
		{
			oldpwd_idx = info->env_cnt;
			flag = TRUE;
		}
		else if (!strncmp(envp[info->env_cnt], "SHLVL=", 6))
			shlvl_idx = info->env_cnt;
		info->env_cnt++;
	}
	if (flag)
		info->env_cnt--;
	dup_env(envp, info, oldpwd_idx, shlvl_idx);
}