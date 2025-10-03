/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 18:48:49 by eraad             #+#    #+#             */
/*   Updated: 2025/10/01 17:37:04 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../../includes/minishell.h"

static int	update_env_var(t_data *data, const char *key, const char *value)
{
	t_env	*temp;

	temp = data->env_copy;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0 && temp->equal == '=')
		{
			free(temp->value);
			temp->value = ft_strdup(value);
		}
		temp = temp->next;
	}
	temp = data->export;
	while (temp)
	{
		if (ft_strcmp(temp->key, key) == 0 && temp->equal == '=')
		{
			free(temp->value);
			temp->value = ft_strdup(value);
		}
		temp = temp->next;
	}
	return (EXIT_SUCCESS);
}

static int	update_cd_env(t_data *data, const char *old_pwd,
		const char *new_pwd)
{
	if (!old_pwd || !new_pwd)
		return (EXIT_FAILURE);
	if (update_env_var(data, "OLDPWD", old_pwd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (update_env_var(data, "PWD", new_pwd) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

static char	*get_cd_target_path(t_data *data, t_command *node)
{
	char	*path;

	if (!node->args)
	{
		path = find_env_value(data, "HOME");
		if (!path)
		{
			report_error(data, "cd: HOME not set", -1);
			return (NULL);
		}
	}
	else if (ft_strcmp(node->args->content, "-") == 0)
	{
		path = find_env_value(data, "OLDPWD");
		if (!path)
		{
			report_error(data, "cd: OLDPWD not set", -1);
			return (NULL);
		}
		ft_putendl_fd(path, 1);
	}
	else
		path = node->args->content;
	return (path);
}

static t_bool	is_valid_cd_args(t_data *data, t_command *node)
{
	if (node->flags)
	{
		report_error(data, "cd: options are not supported", -1);
		return (FALSE);
	}
	if (node->args && node->args->next)
	{
		report_error(data, "cd: too many arguments", -1);
		return (FALSE);
	}
	return (TRUE);
}

int	execute_builtin_cd(t_data *data, t_command *node)
{
	char	old_pwd[PATH_MAX];
	char	new_pwd[PATH_MAX];
	char	*path;

	if (is_valid_cd_args(data, node) == FALSE)
		return (EXIT_FAILURE);
	if (!getcwd(old_pwd, sizeof(old_pwd)))
		return (report_error(data, "getcwd", -1), EXIT_FAILURE);
	path = get_cd_target_path(data, node);
	if (!path)
		return (EXIT_FAILURE);
	if (chdir(path) == -1)
		return (report_error(data, "chdir", -1), free(path), EXIT_FAILURE);
	if (!getcwd(new_pwd, sizeof(new_pwd)))
		return (report_error(data, "getcwd", -1), free(path), EXIT_FAILURE);
	if (cd_update_vars(data, old_pwd, new_pwd) == EXIT_FAILURE)
		return (free(path), EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
