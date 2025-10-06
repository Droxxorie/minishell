/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/15 15:49:21 by eraad             #+#    #+#             */
/*   Updated: 2025/10/05 23:51:11 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	close_fds_from(int start_fd)
{
	int	fd;
	int	max_fd;

	max_fd = 1024;
	fd = start_fd;
	while (fd < max_fd)
	{
		close(fd);
		fd++;
	}
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && (s1[i] == s2[i]))
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

long long	ft_atoll(const char *str)
{
	int			i;
	long long	n;
	long long	result;

	if (!str)
		return (0);
	i = 0;
	n = 1;
	result = 0;
	while (ft_iswhitespace(str[i]))
		i++;
	if (str[i] == '-')
	{
		n = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9' && str[i] != '\0')
	{
		result *= 10;
		result += str[i] - 48;
		i++;
	}
	return (result * n);
}

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
