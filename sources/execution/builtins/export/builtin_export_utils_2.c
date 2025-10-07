/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils_2.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 15:39:15 by eraad             #+#    #+#             */
/*   Updated: 2025/10/07 16:48:43 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

long long parse_shlvl(const char *value)
{
	const char *ptr;
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

static int	check_event_expansion_arg(const char *arg)
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

static int	error_arg_handler(char *arg)
{
	if (arg)
		report_error3("export: `", arg, "': not a valid identifier");
	else
		report_error3("export: `", "", "': not a valid identifier");
	return (1);
}

static int	error_key_handler(char *key, char *arg)
{
	if (key)
	{
		report_error3("export: `", key, "': not a valid identifier");
		free(key);
	}
	else
		report_error3("export: `", arg, "': not a valid identifier");
	return (1);
}

int	process_export_args(t_data *data, char *arg, t_bool do_mutate)
{
	int		key_index;
	char	*key;

	if (!arg || arg[0] == '\0' || arg[0] == '=')
		return (error_arg_handler(arg));
	if (check_event_expansion_arg(arg))
		return (1);
	key = extract_env_key(arg);
	if (!key || key_is_valid(key) == FALSE)
		return (error_key_handler(key, arg));
	if (do_mutate == FALSE)
	{
		free(key);
		return (0);
	}
	key_index = env_index_of_key(data->env_copy, key);
	if (key_index != -1)
		env_update_value(data, arg, key_index);
	else
		env_add_from_arg(data, data->env_copy, key, arg);
	handle_export_arg(data, key, arg, key_index);
	return (0);
}
