/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jusohn <jusohn@student.42seoul.kr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/28 20:09:57 by jusohn            #+#    #+#             */
/*   Updated: 2023/08/28 20:10:21 by jusohn           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

/**
 * @brief print the logo of our minishell, 'nimishell'.
 * 
 */
void	print_logo(void)
{
	clear_screen();
	write(STD_OUT, "              o8o                     o\
8o           oooo                  oooo  oooo\n", 85);
	write(STD_OUT, "              `\"'                     `\"\
'           `888                  `888  `888\n", 85);
	write(STD_OUT, " ooo. .oo.   oooo  ooo. .oo.  .oo.   oo\
oo   .oooo.o  888 .oo.    .ooooo.   888   888\n", 85);
	write(STD_OUT, " `888P\"Y88b  `888  `888P\"Y88bP\"Y88b \
 `888  d88(  \"8  888P\"Y88b  d88' `88b  888   888\n", 85);
	write(STD_OUT, "  888   888   888   888   888   888   8\
88  `\"Y88b.   888   888  888ooo888  888   888\n", 85);
	write(STD_OUT, "  888   888   888   888   888   888   8\
88  o.  )88b  888   888  888    .o  888   888\n", 85);
	write(STD_OUT, " o888o o888o o888o o888o o888o o888o o8\
88o 8\"\"888P' o888o o888o `Y8bod8P' o888o o888o\n", 85);
	write(STD_OUT, "\n\n\n", 3);
	write(STD_OUT, "\t\t\t\t\t\t\t- by sejinkim, jusohn\n", 29);
	write(STD_OUT, "\n\n\n", 3);
}
