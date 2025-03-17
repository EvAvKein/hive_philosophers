/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_feast.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 15:07:53 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/01 16:44:12 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	place_forks(t_feast *feast)
{
	int	i;

	i = 0;
	while (i < feast->num_of_philos)
	{
		if (pthread_mutex_init(&feast->forks[i], NULL))
		{
			while (i)
				pthread_mutex_destroy(&feast->forks[--i]);
			free(feast->forks);
			feast->forks = NULL;
			return (end_feast(feast, "Feast off: Can't get enough forks :(\n"));
		}
		i++;
	}
	return (true);
}

static t_philo	*welcome_philo(t_feast *feast, t_philo_args args, int id)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	*philo = (t_philo){
		.feast = feast,
		.id = id, .next = NULL,
		.hands = {
		(t_philo_hand){.fork = &feast->forks[id - 1],
		.gripping = false},
		(t_philo_hand){.fork = &feast->forks[id % args.num_of_philos],
		.gripping = false}},
		.ate = 0, .last_satiated = 0
	};
	if (id == args.num_of_philos)
	{
		philo->hands[0].fork = &feast->forks[id % args.num_of_philos];
		philo->hands[1].fork = &feast->forks[id - 1];
	}
	return (philo);
}

static bool	welcome_philos(t_feast *feast, t_philo_args data)
{
	t_philo		*append_philo;
	t_philo		*philo;
	int			i;

	i = 0;
	feast->philos = welcome_philo(feast, data, ++i);
	if (!feast->philos)
		return (end_feast(feast, "Feast off: No space for even 1 philo :(\n"));
	append_philo = feast->philos;
	while (i < data.num_of_philos)
	{
		philo = welcome_philo(feast, data, ++i);
		if (!philo)
			return (end_feast(feast, "Feast off: No space for philos :(\n"));
		append_philo->next = philo;
		append_philo = philo;
	}
	append_philo->next = feast->philos;
	return (true);
}

static bool	greet_everyone(t_feast *feast)
{
	pthread_t	thread;
	t_philo		*philo;
	int			i;

	if (pthread_create(&thread, NULL, grim_reaper_routine, feast))
	{
		pthread_mutex_unlock(&feast->staff.greeter);
		return (end_feast(feast, "Feast off: Couldn't greet the reaper :(\n"));
	}
	feast->grim_reaper = thread;
	philo = feast->philos;
	i = 0;
	while (i < feast->num_of_philos)
	{
		if (pthread_create(&thread, NULL, philo_routine, philo))
		{
			pthread_mutex_unlock(&feast->staff.greeter);
			return (end_feast(feast, "Feast off: Couldn't greet a philo :(\n"));
		}
		feast->philo_threads[i++] = thread;
		philo = philo->next;
	}
	return (true);
}

bool	launch_feast(t_feast *feast, t_philo_args data)
{
	if (!place_forks(feast))
		return (false);
	if (!welcome_philos(feast, data))
		return (false);
	pthread_mutex_lock(&feast->staff.greeter);
	if (!greet_everyone(feast))
		return (false);
	gettimeofday(&feast->serve_time, NULL);
	feast->status = CRAVINGS;
	pthread_mutex_unlock(&feast->staff.greeter);
	return (true);
}
