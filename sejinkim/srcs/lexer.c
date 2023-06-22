/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejinkim <sejinkim@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/21 19:50:41 by sejinkim          #+#    #+#             */
/*   Updated: 2023/06/22 10:41:19 by sejinkim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*_str_dup_(char *str, size_t len)
{
	char	*dup;
	size_t	i;

	dup = malloc(sizeof(char) * (len + 1));
	i = 0;
	while (i < len)
	{
		dup[i] = str[i];
		i++;
	}
	dup[i] = 0;
	return (dup);
}

size_t	_len_(char *str, char flag)
{
	char	c;
	size_t	len;

	c = flag;
	len = 1;
	if (!c)
	{
		c = ' ';
		len = 0;
	}
	else if (c == '(')
		c = ')';
	while (str[len] && str[len] != c)
		len++;
	if (!str[len] && (flag == '\"' || flag == '\''))
		return (0);
	if (c != ' ')
		len++;
	return (len);
}

t_node	*_new_node_(t_node *tail, char *str, size_t len)
{
	t_node	*new;

	new = malloc(sizeof(t_node));
	new->left = NULL;
	new->right = NULL;
	new->val = _str_dup_(str, len);
	if (tail)
		tail->right = new;
	return (new);
}

void	lexer(char *str, t_node **head)
{
	char	flag;
	size_t	len;
	t_node	*tail;

	tail = NULL;
	while (*str)
	{
		while (*str == ' ')
			str++;
		if (!*str)
			return ;
		flag = *str * (*str == '\"' || *str == '\'' || *str == '(');
		len = _len_(str, flag);
		if (!len)
			_syntax_err_(*head);
		tail = _new_node_(tail, str, len);
		if (!*head)
			*head = tail;
		str += len;
	}
}
