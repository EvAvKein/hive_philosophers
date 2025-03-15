/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 09:15:43 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/15 15:57:48 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	parse_philo_args(t_philo_args *data, int argc, char **argv)
{
	data->num_of_philos = ft_atoi_positive_strict(argv[1]);
	data->time_to_die = ft_atoi_positive_strict(argv[2]);
	data->time_to_eat = ft_atoi_positive_strict(argv[3]);
	data->time_to_sleep = ft_atoi_positive_strict(argv[4]);
	if (data->num_of_philos < 0 || data->time_to_die < 1
		|| data->time_to_eat < 1 || data->time_to_sleep < 1)
		return (false);
	if (argc == 6)
	{
		data->must_eat = ft_atoi_positive_strict(argv[5]);
		if (data->must_eat < 1)
			return (false);
	}
	else
		data->must_eat = -1;
	return (true);
}

static bool	prepare_feast(t_feast *feast, t_philo_args data)
{
	if (pthread_mutex_init(&feast->greeter, NULL))
	{
		write(STDERR_FILENO, "Feast off: Can't bring greeter :(\n", 35);
		return (false);
	}
	if (pthread_mutex_init(&feast->stenographer, NULL))
	{
		write(STDERR_FILENO, "Feast off: Can't bring stenographer :(\n", 40);
		return (pthread_mutex_destroy(&feast->greeter) && false);
	}
	feast->forks = malloc(sizeof(pthread_mutex_t) * (data.num_of_philos));
	feast->philo_threads = malloc(sizeof(pthread_t) * (data.num_of_philos));
	if (!feast->forks || !feast->philo_threads)
	{
		if (feast->forks)
			free(feast->forks);
		if (feast->philo_threads)
			free(feast->philo_threads);
		write(STDERR_FILENO, "Feast off: Not enough space for feast :(\n", 42);
		return (false);
	}
	memset(feast->philo_threads, '\0', sizeof(pthread_t) * data.num_of_philos);
	memset(feast->forks, '\0', sizeof(pthread_mutex_t) * data.num_of_philos);
	return (true);
}

static int	philosophers(t_philo_args data)
{
	t_feast	feast;

	feast = (t_feast){
		.status = COOKING, .forks = NULL, .philo_threads = NULL,
		.philos = NULL, .num_of_philos = data.num_of_philos,
		.time_to_die = data.time_to_die, .time_to_eat = data.time_to_eat,
		.time_to_sleep = data.time_to_sleep, .must_eat = data.must_eat
	};
	if (!prepare_feast(&feast, data))
		return (EXIT_FAILURE);
	if (!launch_feast(&feast, data))
		return (EXIT_FAILURE);
	wait_for_everyone(&feast);
	free(feast.philo_threads);
	feast.philo_threads = NULL;
	feast.grim_reaper = 0;
	end_feast(&feast, NULL);
	return (EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_philo_args	data;

	if (argc < 5 || argc > 6)
	{
		write(STDERR_FILENO, "\e[1mphilo program arguments:\e[0m\n\
 - number_of_philosophers\n - time_to_die\n\
 - time_to_eat\n - time_to_sleep\n\
 - number_of_times_each_philosopher_must_eat \e[2;37m(optional)\e[0;37m\n",
			177);
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
