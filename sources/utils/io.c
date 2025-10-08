/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:09:29 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 16:09:40 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
