/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eraad <eraad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 15:57:37 by eraad             #+#    #+#             */
/*   Updated: 2025/10/09 04:12:03 by eraad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

static void	exit_numeric_error(t_data *data, char *arg)
{
	safe_putstr_fd("minishell: ", STDERR_FILENO);
	safe_putstr_fd("exit: ", STDERR_FILENO);
	safe_putstr_fd(arg, STDERR_FILENO);
	safe_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	exit_minishell(data, 2);
}

static int	exit_too_many_args(t_data *data)
{
	report_error(data, "exit: too many arguments", 1);
	return (EXIT_FAILURE);
}

static t_bool	is_valid_number(const char *str)
{
	int					sign;
	unsigned long long	result;

	if (!str || *str == '\0')
		return (FALSE);
	while (ft_iswhitespace((unsigned char)*str))
		str++;
	sign = 1;
	if (*str == '+' || *str == '-')
		if (*(str++) == '-')
			sign = -1;
	result = 0;
	while (ft_isdigit((unsigned char)*str))
	{
		result = result * 10ULL + (unsigned long long)(*(str++) - '0');
		if ((result > (unsigned long long)LLONG_MAX && sign == 1)
			|| (result > (unsigned long long)LLONG_MAX + 1ULL && sign == -1))
			return (FALSE);
	}
	if (*str != '\0' || !ft_isdigit(*(str - 1)))
		return (FALSE);
	return (TRUE);
}

static int	execute_builtin_exit_helper(t_data *data, char **argv,
		t_bool in_pipeline)
{
	unsigned char	status;

	if (is_valid_number(argv[1]) == FALSE)
	{
		if (!in_pipeline)
			exit_numeric_error(data, argv[1]);
		data->exit_status = 2;
		return (EXIT_FAILURE);
	}
	if (argv[2])
	{
		if (!in_pipeline)
			return (exit_too_many_args(data));
		data->exit_status = 1;
		return (EXIT_FAILURE);
	}
	status = (unsigned char)(ft_atoll(argv[1]));
	if (!in_pipeline)
		exit_minishell(data, status);
	data->exit_status = status;
	return (EXIT_SUCCESS);
}

int	execute_builtin_exit(t_data *data, char **argv)
{
	t_bool	in_pipeline;
	int		argc;

	if (!data || !argv || !argv[0])
		return (EXIT_FAILURE);
	in_pipeline = (data->pipes && data->pipes->nb > 0);
	argc = 0;
	while (argv[argc])
		argc++;
	if (argc == 1)
	{
		if (!in_pipeline)
			exit_minishell(data, EXIT_SUCCESS);
		return (EXIT_SUCCESS);
	}
	if (execute_builtin_exit_helper(data, argv, in_pipeline) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (data->exit_status == 0)
		return (EXIT_SUCCESS);
	return (EXIT_FAILURE);
}
