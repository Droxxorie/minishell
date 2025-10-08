/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 16:05:34 by eraad             #+#    #+#             */
/*   Updated: 2025/10/08 16:58:51 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

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
