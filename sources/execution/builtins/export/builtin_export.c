/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/01 12:12:50 by eraad             #+#    #+#             */
/*   Updated: 2025/10/02 13:03:15 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

t_bool	key_is_valid(const char *key)
{
	int	i;

	if (!key || (!ft_isalpha(key[0]) && key[0] != '_'))
		return (FALSE);
	i = 1;
	while (key[i])
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (FALSE);
		i++;
	}
	return (TRUE);
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

static void	process_export_args(t_data *data, char *arg)
{
	int		key_index;
	char	*key;

	if (!data || !arg)
		return ;
	if (check_event_expansion_arg(arg))
		return ;
	key = extract_env_key(arg);
	if (!key || key_is_valid(key) == FALSE)
	{
		if (key)
		{
			report_error3("export: `", key, "': not a valid identifier");
			free(key);
		}
		else
			report_error3("export: `", arg, "': not a valid identifier");
		return ;
	}
	key_index = env_index_of_key(data->env_copy, key);
	if (key_index != -1)
		env_update_value(data, arg, key_index);
	else
		env_add_from_arg(data, data->env_copy, key, arg);
	handle_export_arg(data, key, arg, key_index);
}

static void	print_export_list(t_data *data, int fd)
{
	t_env	*sorted;

	sorted = data->export;
	while (sorted)
	{
		if (safe_putstr_fd("declare -x ", fd) == EXIT_FAILURE)
			return (report_error(data, "export: write", -1));
		if (safe_putstr_fd(sorted->key, fd) == EXIT_FAILURE)
			return (report_error(data, "export: write", -1));
		if (sorted->equal && sorted->value)
			if (safe_putstr_fd("=\"", fd) == EXIT_FAILURE)
				return (report_error(data, "export: write", -1));
		if (sorted->value)
			if (safe_putstr_fd(sorted->value, fd) == EXIT_FAILURE)
				return (report_error(data, "export: write", -1));
		if (sorted->equal)
			if (safe_putchar_fd('\"', fd) == EXIT_FAILURE)
				return (report_error(data, "export: write", -1));
		if (safe_putchar_fd('\n', fd) == EXIT_FAILURE)
			return (report_error(data, "export: write", -1));
		sorted = sorted->next;
	}
}

int	execute_builtin_export(t_data *data, char **argv, int fd)
{
	int		i;
	t_bool	error;

	if (!data->export)
		init_export_list(data);
	if (!argv[1])
	{
		print_export_list(data, fd);
		return (EXIT_SUCCESS);
	}
	i = 0;
	error = FALSE;
	while (argv[++i])
	{
		if (argv[i][0] == '\0' || ft_strcmp(argv[i], "\"\"") == 0)
		{
			report_error(data, "export: invalid identifier", -1);
			error = TRUE;
		}
		if (!data->pipes->nb)
			process_export_args(data, argv[i]);
	}
	if (error == TRUE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
