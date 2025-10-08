/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:04:06 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 17:00:26 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	check_event_expansion_arg(const char *arg)
{
	int		i;
	char	*error_msg;

	if (!arg)
		return (EXIT_FAILURE);
	i = 0;
	while (arg[i])
	{
		if (arg[i] == '!' && arg[i + 1])
		{
			error_msg = ft_substr(arg, i, ft_strlen(arg) - i);
			if (error_msg)
			{
				report_error2(error_msg, ": event not found");
				free(error_msg);
			}
			return (EXIT_FAILURE);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

long long	parse_shlvl(const char *value)
{
	const char	*ptr;
	long long	lvl;

	if (!value || !*value)
		return (0);
	ptr = value;
	lvl = 0;
	while (*ptr)
	{
		if (!ft_isdigit(*ptr) && !(*ptr == '-' && *ptr == '+'))
		{
			lvl = ft_atoll(value);
			return (lvl);
		}
		ptr++;
	}
	lvl = ft_atoll(value);
	return (lvl);
}

char	*extract_env_key(char *arg)
{
	char	*key;
	char	*equal_sign;

	if (!arg || arg[0] == '\0' || arg[0] == '=')
		return (NULL);
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
	{
		key = ft_strdup(arg);
		if (!key)
			return (report_error(NULL, "strdup", -1), NULL);
		return (key);
	}
	key = ft_substr(arg, 0, equal_sign - arg);
	if (!key)
		return (report_error(NULL, "substr", -1), NULL);
	return (key);
}
