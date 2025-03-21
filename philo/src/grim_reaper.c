/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grim_reaper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:12:42 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/17 15:49:12 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	reap(t_feast *feast, t_philo *philo)
{
	feast->status = CANCELLED;
	philo_log(philo, CLR_DIED "died" CLR_RESET, true);
	pthread_mutex_lock(&feast->staff.fork_coordinator);
	if (philo->hands[0].gripping)
	{
		pthread_mutex_unlock(philo->hands[0].fork);
		philo->hands[0].gripping = false;
	}
	if (philo->hands[1].gripping)
	{
		pthread_mutex_unlock(philo->hands[1].fork);
		philo->hands[1].gripping = false;
	}
	pthread_mutex_unlock(&feast->staff.fork_coordinator);
}

static void	grim_loop(t_feast *feast, t_philo *philo, int satiated)
{
	while (1)
	{
		if (feast->status == CANCELLED)
			return ;
		if (feast->must_eat >= 0 && philo->ate == feast->must_eat)
			satiated++;
		else
			satiated = 0;
		if (satiated == feast->num_of_philos)
		{
			feast->status = CANCELLED;
			return ;
		}
		if ((ms_since(feast->serve_time) - philo->last_satiated)
			>= feast->time_to_die)
		{
			reap(feast, philo);
			return ;
		}
		philo = philo->next;
	}
}

void	*grim_reaper_routine(void *arg)
{
	t_feast	*feast;
	t_philo	*philo;
	int		satiated;

	feast = (t_feast *)arg;
	philo = feast->philos;
	pthread_mutex_lock(&feast->staff.greeter);
	if (feast->status == CANCELLED)
	{
		pthread_mutex_unlock(&feast->staff.greeter);
		return (NULL);
	}
	pthread_mutex_unlock(&feast->staff.greeter);
	philo = feast->philos;
	satiated = 0;
	grim_loop(feast, philo, satiated);
	return (NULL);
}
