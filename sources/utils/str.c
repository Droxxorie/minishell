/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:08:19 by eraad             #+#    #+#             */
/*   Updated: 2025/10/23 10:44:25 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*strjoin_free_left(char *left, char *right)
{
	char	*joined;

	if (!left && !right)
		return (NULL);
	if (!left)
		return (ft_strdup(right));
	if (!right)
		return (left);
	joined = ft_strjoin(left, right);
	free(left);
	return (joined);
}

char	*strjoin_free_both(char *left, char *right)
{
	char	*joined;

	if (!left && !right)
		return (NULL);
	if (!left)
		return (right);
	if (!right)
		return (left);
	joined = ft_strjoin(left, right);
	free(left);
	free(right);
	return (joined);
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
