/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:59:17 by eraad             #+#    #+#             */
/*   Updated: 2025/10/10 12:41:24 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	env_unlink_key(t_env **head, const char *key)
{
	t_env	*current;
	t_env	*previous;

	if (!head || !*head || !key)
		return ;
	previous = NULL;
	current = *head;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (previous)
				previous->next = current->next;
			else
				*head = current->next;
			free(current->key);
			if (current->value)
				free(current->value);
			free(current);
			return ;
		}
		previous = current;
		current = current->next;
	}
}

int	execute_builtin_unset(t_data *data, char **argv)
{
	int			i;
	const char	*key;

	if (!data || !argv)
		return (EXIT_FAILURE);
	i = 1;
	while (argv[i])
	{
		key = argv[i];
		if (key[0] == '\0' || key_is_valid(key) == FALSE)
		{
			i++;
			continue ;
		}
		env_unlink_key(&data->env_copy, key);
		env_unlink_key(&data->export, key);
		i++;
	}
	return (EXIT_SUCCESS);
}
