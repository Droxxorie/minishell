/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 16:10:00 by eraad             #+#    #+#             */
/*   Updated: 2025/10/02 12:25:19 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void swap_env_fields(t_env *a, t_env *b)
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

int safe_putstr_fd(char *s, int fd)
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

int safe_putchar_fd(char c, int fd)
{
	if (fd > -1)
	{
		if (write(fd, &c, 1) == -1)
			return (EXIT_FAILURE);
		return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}
