/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 09:15:43 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/06 11:23:38 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_atoi_positive_strict(char *str)
{
	size_t	i;
	int		num;

	if (!str)
		return (-1);
	if (str[0] == '-')
	{
		if (str[1] == '0' && !str[2])
			return (0);
		else
			return (-1);
	}
	i = 0;
	num = 0;
	if ((!(*str >= '0' && *str <= '9') && !((str[0] == '+') && ++i)) || !str[i])
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = (num * 10) + (str[i++] - '0');
		if (num < 0)
			return (-1);
	}
	if (str[i])
		return (-1);
	return (num);
}

static bool	parse_philo_args(t_philo_args *data, int argc, char **argv)
{
	data->num_of_philos = ft_atoi_positive_strict(argv[1]);
	data->time_to_die = ft_atoi_positive_strict(argv[2]);
	data->time_to_eat = ft_atoi_positive_strict(argv[3]);
	data->time_to_sleep = ft_atoi_positive_strict(argv[4]);
	if (data->num_of_philos < 0 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0)
		return (0);
	if (argc == 6)
	{
		data->must_eat = ft_atoi_positive_strict(argv[5]);
		if (data->must_eat < 0)
			return (0);
	}
	else
		data->must_eat = -1;
	return (1);
}

static bool	prepare_feast(t_feast *feast, t_philo_args data)
{
	*feast = (t_feast){
		.status = COOKING, .philos = NULL, .num_of_philos = data.num_of_philos,
		.forks = malloc(sizeof(t_fork) * (data.num_of_philos + 1)),
		.threads = malloc(sizeof(pthread_t) * (data.num_of_philos + 1)),
		.time_to_die = data.time_to_die,
		.time_to_eat = data.time_to_eat,
		.time_to_sleep = data.time_to_sleep,
		.must_eat = data.must_eat
	};
	if (!feast->forks || !feast->threads)
	{
		if (feast->forks)
			free(feast->forks);
		if (feast->threads)
			free(feast->threads);
		write(STDERR_FILENO, "Feast off: A feast malloc failed :(\n", 37);
		return (false);
	}
	pthread_mutex_init(&feast->stenographer, NULL);
	memset(feast->threads, '\0',
		sizeof(pthread_t) * (data.num_of_philos + 1));
	memset(feast->forks, '\0',
		sizeof(t_fork) * (data.num_of_philos + 1));
	feast->num_of_philos = data.num_of_philos;
	return (true);
}

static int	philosophers(t_philo_args data)
{
	t_feast	feast;

	if (!prepare_feast(&feast, data))
		return (EXIT_FAILURE);
	if (!launch_feast(&feast, data))
		return (EXIT_FAILURE);
	wait_for_philos(&feast);
	free(feast.threads);
	feast.threads = NULL;
	end_feast(&feast, NULL);
	return (EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_philo_args	data;

	if (argc < 5 || argc > 6)
	{
		write(STDERR_FILENO, "./philo program arguments:\n\
 - number_of_philosophers\n - time_to_die\n\
 - time_to_eat\n - time_to_sleep\n\
 - number_of_times_each_philosopher_must_eat \033[37m(optional)\033[0m\n",
			166);
		return (EXIT_FAILURE);
	}
	if (!parse_philo_args(&data, argc, argv))
	{
		write(STDERR_FILENO, "Invalid philo arguments\
 - make sure they're all positive integers!\n", 68);
		return (EXIT_FAILURE);
	}
	if (!data.num_of_philos)
	{
		write(STDERR_FILENO, "No philosophers? :(\n", 21);
		return (EXIT_FAILURE);
	}
	return (philosophers(data));
}
