/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:59:17 by eraad             #+#    #+#             */
/*   Updated: 2025/10/12 16:08:24 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	env_unlink_key_helper(t_env **head, t_env **cur, t_env **prev)
{
	t_env	*to_free;

	to_free = *cur;
	if (*prev)
		(*prev)->next = (*cur)->next;
	else
		*head = (*cur)->next;
	*cur = (*cur)->next;
	free(to_free->key);
	free(to_free->value);
	free(to_free);
}

static void	env_unlink_key(t_env **head, const char *key)
{
	t_env	*cur;
	t_env	*prev;

	if (!head || !*head || !key)
		return ;
	prev = NULL;
	cur = *head;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			env_unlink_key_helper(head, &cur, &prev);
			continue ;
		}
		prev = cur;
		cur = cur->next;
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
