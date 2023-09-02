/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logo_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:09:57 by jusohn            #+#    #+#             */
/*   Updated: 2023/09/02 15:56:48 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell_bonus.h"

/**
 * @brief clear screen in bash, before printing the logo of our minishell,
 *      'nimishell'.
 * 
 */
void	clear_screen(void)
{
	const char	*clear_screen_ansi = "\e[1;1H\e[2J";

	write(STD_OUT, clear_screen_ansi, ft_strlen(clear_screen_ansi));
	write(STD_OUT, "\n\n\n", 3);
}

// green	\033\[32m
// red		\033\[31m
// yellow	\033\[33m
// blue		\033\[34m
// default	\033\[0m
/**
 * @brief print the logo of our minishell, 'nimishell'.
 * 
 */
void	print_logo(void)
{
	clear_screen();
	write(STD_OUT, "              \033\[33mo8o                     \033\[34mo8\
o\033\[0m           oooo                  oooo  oooo\n", 100);
	write(STD_OUT, "              \033\[33m`\"'                     \033\[34m`\
\"'           \033\[0m`888                  `888  `888\n", 100);
	write(STD_OUT, " \033\[31mooo. .oo.\033\[33m   oooo  \033\[32mooo. .oo.  .\
oo.   \033\[34moooo   \033\[0m.oooo.o  888 .oo.    .ooooo.   888   888\n", 110);
	write(STD_OUT, " \033\[31m`888P\"Y88b\033\[33m  `888  \033\[32m`888P\"Y88b\
P\"Y88b  \033\[34m`888  \033\[0md88(  \"8  888P\"Y88b  d88' `88b  888   888\n", 110);
	write(STD_OUT, "  \033\[31m888   888\033\[33m   888   \033\[32m888   888  \
 888   \033\[34m888  \033\[0m`\"Y88b.   888   888  888ooo888  888   888\n", 110);
	write(STD_OUT, "  \033\[31m888   888\033\[33m   888   \033\[32m888   888  \
 888   \033\[34m888  \033\[0mo.  )88b  888   888  888    .o  888   888\n", 110);
	write(STD_OUT, " \033\[31mo888o o888o\033\[33m o888o \033\[32mo888o o888o \
o888o \033\[34mo888o \033\[0m8\"\"888P' o888o o888o `Y8bod8P' o888o o888o\n", 110);
	write(STD_OUT, "\n\n\n", 3);
	write(STD_OUT, "\t\t\t\t\t\t\t- by sejinkim, jusohn\n", 29);
	write(STD_OUT, "\n\n\n", 3);
}
