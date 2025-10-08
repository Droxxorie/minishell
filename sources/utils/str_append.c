/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_append.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:08:47 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 16:09:14 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_append_char(char **str, char c)
{
	size_t	len;
	char	*new_str;

	if (!str)
		return (EXIT_FAILURE);
	len = 0;
	if (*str)
		len = ft_strlen(*str);
	new_str = malloc(len + 2);
	if (!new_str)
	{
		report_error(NULL, "malloc", -1);
		return (EXIT_FAILURE);
	}
	if (*str)
		ft_memcpy(new_str, *str, len);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(*str);
	*str = new_str;
	return (EXIT_SUCCESS);
}
