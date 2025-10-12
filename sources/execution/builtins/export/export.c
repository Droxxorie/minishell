/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:59:54 by eraad             #+#    #+#             */
/*   Updated: 2025/10/12 16:08:57 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

void	export_add_key(t_data *data, const char *key)
{
	char	*dup_key;

	if (!data || !key || key[0] == '\0')
		return ;
	dup_key = ft_strdup(key);
	if (!dup_key)
	{
		report_error(data, "strdup", -1);
		return ;
	}
	if (add_var(data, &data->export, dup_key, NULL) == EXIT_FAILURE)
	{
		free(dup_key);
		return ;
	}
}

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
	int		had_error;
	t_bool	do_mutate;

	if (!data->export)
		init_export_list(data);
	if (!argv[1])
	{
		print_export_list(data, fd);
		return (0);
	}
	had_error = 0;
	do_mutate = !(data->pipes && data->pipes->nb);
	i = 1;
	while (argv[i])
	{
		if (process_export_args(data, argv[i], do_mutate) == 1)
			had_error = 1;
		i++;
	}
	if (had_error)
		return (1);
	return (0);
}
