/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:05:38 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/05 18:35:32 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	pickup_fork_until_death(
	t_feast *feast, t_philo *philo, t_fork *fork)
{
	while (!fork->available)
	{
		if (feast->status == CANCELLED || starved_to_death(feast, philo))
			return (true);
		usleep(1000);
	}
	fork->available = false;
	pthread_mutex_lock(&fork->mutex);
	if (feast->status == CANCELLED || starved_to_death(feast, philo))
	{
		pthread_mutex_unlock(&fork->mutex);
		fork->available = true;
		return (true);
	}
	philo_log(philo, "has taken a fork");
	return (false);
}

static bool	drop_forks(t_fork *fork1, t_fork *fork2)
{
	if (fork1)
	{
		pthread_mutex_unlock(&fork1->mutex);
		fork1->available = true;
	}
	if (fork2)
	{
		pthread_mutex_unlock(&fork2->mutex);
		fork2->available = true;
	}
	return (false);
}

static bool	eat(t_feast *feast, t_philo *philo,
	t_fork *fork1, t_fork *fork2)
{
	if (feast->status == CANCELLED)
		return (false);
	if ((ms_now() > ms_of(feast->serve_time))
		//&& philo->id != feast->num_of_philos
		&& !(philo->id % 2)
		&& (ms_of(philo->last_satiated) + feast->time_to_die)
			> (ms_now() + feast->time_to_eat))
	{
		if (usleep_until_death(
				feast, philo, feast->time_to_eat
				//feast, philo, feast->time_to_eat / ((philo->id % 2) + 1)
			, false))
			return (NULL);
	}
	if (pickup_fork_until_death(feast, philo, fork1))
		return (drop_forks(fork1, NULL));
	if (pickup_fork_until_death(feast, philo, fork2))
		return (drop_forks(fork1, fork2));
	gettimeofday(&philo->last_satiated, NULL);
	philo_log(philo, "is eating");
	if (usleep_until_death(feast, philo, feast->time_to_eat, true))
		return (drop_forks(fork1, fork2));
	drop_forks(fork1, fork2);
	philo->ate++;
	return (true);
}

static void	*die_alone(t_feast *feast, t_philo *philo)
{
	pthread_mutex_lock(&philo->forks[0]->mutex);
	philo_log(philo, "has taken a fork");
	while (!starved_to_death(feast, philo))
		;
	pthread_mutex_unlock(&philo->forks[0]->mutex);
	return (NULL);
}

void	*philo_routine(void *arg)
{
	t_feast			*feast;
	t_philo			*philo;

	philo = (t_philo *)arg;
	feast = philo->feast;
	while (feast->status == COOKING)
		;
	if (feast->status == CANCELLED)
		return (NULL);
	philo_log(philo, "is thinking");
	philo->last_satiated = feast->serve_time;
	if (philo->forks[0] == philo->forks[1])
		return (die_alone(feast, philo));
	if (philo->id % 2
		&& usleep_until_death(feast, philo, feast->time_to_eat, false))
			return (NULL);
	while (feast->status != CANCELLED)
	{
		if (feast->must_eat >= 0 && philo->ate == feast->must_eat)
			return (NULL);
		if (!eat(feast, philo, philo->forks[0], philo->forks[1]))
			return (NULL);
		philo_log(philo, "is sleeping");
		if (usleep_until_death(feast, philo, feast->time_to_sleep, false))
			return (NULL);
		philo_log(philo, "is thinking");
	}
	return (NULL);
}
