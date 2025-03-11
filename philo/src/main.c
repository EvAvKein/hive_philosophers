/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 09:15:43 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/11 11:11:13 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static bool	init_locks(t_feast *feast)
{
	if (pthread_mutex_init(&feast->status_check, NULL))
	{
		write(STDERR_FILENO, "Feast off: Can't create status check :(\n", 41);
		return (false);
	}
	if (pthread_mutex_init(&feast->stenographer, NULL))
	{
		write(STDERR_FILENO, "Feast off: Can't bring stenographer :(\n", 40);
		pthread_mutex_destroy(&feast->status_check);
		return (false);
	}
	return (true);
}

static bool	prepare_feast(t_feast *feast, t_philo_args data)
{
	*feast = (t_feast){
		.status = COOKING, .philos = NULL, .num_of_philos = data.num_of_philos,
		.forks = NULL, .philo_threads = NULL,
		.time_to_die = data.time_to_die, .time_to_eat = data.time_to_eat,
		.time_to_sleep = data.time_to_sleep, .must_eat = data.must_eat,
	};
	if (!init_locks(feast))
		return (false);
	feast->forks = malloc(sizeof(pthread_mutex_t) * (data.num_of_philos + 1));
	feast->philo_threads = malloc(sizeof(pthread_t) * (data.num_of_philos + 1));
	if (!feast->forks || !feast->philo_threads)
	{
		if (feast->forks)
			free(feast->forks);
		if (feast->philo_threads)
			free(feast->philo_threads);
		write(STDERR_FILENO, "Feast off: A feast malloc failed :(\n", 37);
		return (false);
	}
	memset(feast->philo_threads, '\0', sizeof(pthread_t) * data.num_of_philos);
	memset(feast->forks, '\0', sizeof(pthread_mutex_t) * data.num_of_philos);
	return (true);
}

static int	philosophers(t_philo_args data)
{
	t_feast	feast;

	if (!prepare_feast(&feast, data))
		return (EXIT_FAILURE);
	if (!launch_feast(&feast, data))
		return (EXIT_FAILURE);
	wait_for_everyone(&feast);
	free(feast.philo_threads);
	feast.philo_threads = NULL;
	feast.grim_reaper_thread = 0;
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
