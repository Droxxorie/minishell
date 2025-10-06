/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   externals_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 11:55:37 by eraad             #+#    #+#             */
/*   Updated: 2025/10/05 23:03:50 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	parent_close_after_fork(int *fds, int index, int n_cmds)
{
	int max_index;
	
	if (!fds || n_cmds <= 1)
		return ;
	max_index = (n_cmds - 1) * 2;
	if (index - 2 >= 0 && fds[index - 2] != -1)
		close(fds[index - 2]);
	if (index + 1 < max_index && fds[index + 1] != -1)
		close(fds[index + 1]);
}

int	compute_n_cmds(t_data *data)
{
	if (data && data->pipes)
		return (data->pipes->nb + 1);
	return (1);
}

static int	env_list_size(const t_env *env)
{
	int	n;

	n = 0;
	while (env)
	{
		if (env->equal)
			n++;
		env = env->next;
	}
	return (n);
}

static char	*build_env_entry(const t_env *env)
{
	char	*env_str;
	size_t	len_key;
	size_t	len_value;

	if (!env || !env->key)
		return (NULL);
	len_key = ft_strlen(env->key);
	if (env->value)
		len_value = ft_strlen(env->value);
	else
		len_value = 0;
	env_str = (char *)malloc(len_key + 1 + len_value + 1);
	if (!env_str)
		return (report_error(NULL, "malloc", -1), NULL);
	ft_memcpy(env_str, env->key, len_key);
	env_str[len_key] = '=';
	if (len_value)
		ft_memcpy(env_str + len_key + 1, env->value, len_value);
	env_str[len_key + 1 + len_value] = '\0';
	return (env_str);
}

char	**env_list_to_array(const t_env *env)
{
	char	**envp;
	int		size;
	int		i;

	size = env_list_size(env);
	envp = (char **)malloc((size + 1) * sizeof(char *));
	if (!envp)
		return (report_error(NULL, "malloc", -1), NULL);
	i = 0;
	while (env)
	{
		if (env->equal)
		{
			envp[i] = build_env_entry(env);
			if (!envp[i])
			{
				free_char_array(envp);
				return (NULL);
			}
			i++;
		}
		env = env->next;
	}
	envp[i] = NULL;
	return (envp);
}
