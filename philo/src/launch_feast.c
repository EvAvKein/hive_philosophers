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

static t_philo	*welcome_philo(t_philo_init_data data)
{
	t_philo	*philo;

	philo = malloc(sizeof(t_philo));
	if (!philo)
	{
		end_feast(data.feast, "Feast off: Philo malloc failed :(\n");
		return (NULL);
	}
	*philo = (t_philo){
		.feast = data.feast, .next = NULL,
		.id = data.id, .status = ABSENT,
		.forks = {
			&data.feast->forks[data.id - 1],
			&data.feast->forks[data.id % data.args.num_of_philos]
		},
		.time_to_die = data.args.time_to_die,
		.time_to_eat = data.args.time_to_eat,
		.time_to_sleep = data.args.time_to_sleep,
		.must_eat = data.args.must_eat, .ate = 0,
	};
	return (philo);
}

static bool	welcome_philos(t_feast *feast, t_philo_args data)
{
	t_philo		*append_philo;
	t_philo		*philo;
	int			i;
	
	i = 0;
	while (i < data.num_of_philos)
	{
		philo = welcome_philo((t_philo_init_data){
			.args = data, .feast = feast, .id = ++i});
		if (!philo)
			return (false);
		if (!feast->philos)
		{
			feast->philos = philo;
			append_philo = philo;
		}
		else 
		{
			append_philo->next = philo;
			append_philo = philo;
		}
	}
	philo->next = feast->philos;
	return (true);
}

static bool thread_all_philos(t_feast *feast)
{
	pthread_t	thread;
	t_philo		*philo;
	int			i;

	philo = feast->philos;	
	i = 0;
	while (i < feast->num_of_philos)
	{
		if (pthread_create(&thread, NULL, philo_routine, philo))
			return (end_feast(feast, "Feast off: Threading failed :(\n"));
		feast->threads[i++] = thread;
		philo = philo->next;
	}
	return (true);
}

static bool place_forks(t_feast *feast)
{
	int	i;

	i = 0;
	while (i < feast->num_of_philos)
	{
		feast->forks[i].available = true;
		pthread_mutex_init(&feast->forks[i++].mutex, NULL);
	}
	return (true);
}

bool launch_feast(t_feast *feast, t_philo_args data)
{
	if (!welcome_philos(feast, data))
		return (false);
	if (!thread_all_philos(feast))
		return (false);
	place_forks(feast);
	feast->status = SERVED;
	return (true);
}
