/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 16:10:00 by eraad             #+#    #+#             */
/*   Updated: 2025/10/05 19:14:39 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_minilist	*minilist_last(t_minilist *list)
{
	if (!list)
		return (NULL);
	while (list->next)
		list = list->next;
	return (list);
}

void	minilist_add_back(t_minilist **list, t_minilist *new_node)
{
	t_minilist	*last;

	if (!list || !new_node)
		return ;
	if (!*list)
	{
		*list = new_node;
		return ;
	}
	last = minilist_last(*list);
	last->next = new_node;
}

void	swap_env_fields(t_env *a, t_env *b)
{
	char	*temp_key;
	char	*temp_value;
	char	temp_equal;

	if (!a || !b)
		return ;
	temp_key = a->key;
	temp_value = a->value;
	temp_equal = a->equal;
	a->key = b->key;
	a->value = b->value;
	a->equal = b->equal;
	b->key = temp_key;
	b->value = temp_value;
	b->equal = temp_equal;
}

int	safe_putstr_fd(char *s, int fd)
{
	if (!s)
		return (EXIT_FAILURE);
	while (*s)
	{
		if (safe_putchar_fd(*s++, fd) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	safe_putchar_fd(char c, int fd)
{
	if (fd > -1)
	{
		if (write(fd, &c, 1) == -1)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
