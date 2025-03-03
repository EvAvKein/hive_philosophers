/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:05:38 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/03 16:21:12 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	drop_forks(pthread_mutex_t *fork1, pthread_mutex_t *fork2)
{
	if (fork1)
		pthread_mutex_unlock(fork1);
	if (fork2)
		pthread_mutex_unlock(fork2);
	return (false);
}

static bool	eat(t_feast *feast, t_philo *philo,
	pthread_mutex_t *fork1, pthread_mutex_t *fork2)
{
	if (feast->status == CANCELLED)
		return (false);
	pthread_mutex_lock(fork1);
	philo_log(philo, "has taken a fork");
	if (feast->status == CANCELLED || starved_to_death(feast, philo))
		return (drop_forks(fork1, NULL));
	pthread_mutex_lock(fork2);
	philo_log(philo, "has taken a fork");
	if (feast->status == CANCELLED || starved_to_death(feast, philo))
		return (drop_forks(fork1, fork2));
	philo_log(philo, "is eating");
	if (usleep_until_death(feast, philo, feast->time_to_eat))
		return (drop_forks(fork1, fork2));
	philo->ate++;
	gettimeofday(&philo->last_satiated, NULL);
	drop_forks(fork1, fork2);
	return (true);
}

static bool	wait_for_serving(t_feast *feast)
{
	while (1)
	{
		if (feast->status == COOKING)
			continue ;
		if (feast->status == SERVED)
			return (true);
		if (feast->status == CANCELLED)
			return (false);
	}
}

void	*philo_routine(void *arg)
{
	t_feast			*feast;
	t_philo			*philo;
	
	philo = (t_philo *)arg;
	feast = philo->feast;
	if (!wait_for_serving(feast))
		return (NULL);
	philo->last_satiated = feast->serve_time;
	while (feast->status != CANCELLED)
	{
		if (feast->must_eat >= 0 && philo->ate == feast->must_eat)
			return (NULL); 
		philo_log(philo, "is thinking");
		if (!eat(feast, philo, philo->forks[0], philo->forks[1]))
			return (NULL);
		philo_log(philo, "is sleeping");
		if (usleep_until_death(feast, philo, feast->time_to_sleep))
			return (NULL);
	}
	return (NULL);
}
