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
		.id = data.id, .dead = false,
		.forks = {
			&data.feast->forks[data.id - 1],
			&data.feast->forks[data.id % data.args.num_of_philos]
		},
		.ate = 0, .last_satiated = {}
	};
	if (data.id == data.args.num_of_philos)
	{
		philo->forks[0] = &data.feast->forks[data.id % data.args.num_of_philos];
		philo->forks[1] = &data.feast->forks[data.id - 1];
	}
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
		// HANDLE INIT FAILURES
		pthread_mutex_init(&feast->forks[i].mutex, NULL);
		feast->forks[i++].available = true;
	}
	return (true);
}

bool launch_feast(t_feast *feast, t_philo_args data)
{
	if (!place_forks(feast))
		return (false);
	if (!welcome_philos(feast, data))
		return (false);
	if (!thread_all_philos(feast))
		return (false);
	gettimeofday(&feast->serve_time, NULL);
	feast->status = SERVED;
	return (true);
}
