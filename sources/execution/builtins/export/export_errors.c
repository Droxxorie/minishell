/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:05:34 by eraad             #+#    #+#             */
/*   Updated: 2025/10/19 17:45:30 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

int	export_invalid_option(t_data *data, const char *arg)
{
	if (arg && arg[0] == '-')
	{
		report_error(data, "export: option not supported", -1);
		return (2);
	}
	return (0);
}

int	error_key_handler(char *key, char *arg)
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

int	error_arg_handler(char *arg)
{
	if (arg)
		report_error3("export: `", arg, "': not a valid identifier");
	else
		report_error3("export: `", "", "': not a valid identifier");
	return (1);
}
