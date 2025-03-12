/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:05:38 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/12 16:01:40 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	pickup_fork_until_cancelled(
	t_feast *feast, t_philo *philo, t_philo_hand *hand)
{
	pthread_mutex_lock(hand->fork);
	hand->gripping = true;
	pthread_mutex_lock(&feast->status_check);
	if (feast->status == CANCELLED)
	{
		pthread_mutex_unlock(&feast->status_check);
		pthread_mutex_unlock(hand->fork);
		hand->gripping = false;
		return (true);
	}
	philo_log(philo, "has taken a fork", false);
	pthread_mutex_unlock(&feast->status_check);
	return (false);
}

static bool	eat(t_feast *feast, t_philo *philo,
	t_philo_hand *hand1, t_philo_hand *hand2)
{
	if (is_cancelled(feast))
		return (false);
	if ((ms_now() + feast->time_to_eat)
		< (philo->last_satiated + feast->time_to_die))
	{
		if (usleep_until_cancelled(feast, feast->time_to_eat / 2))
			return (NULL);
	}
	if (pickup_fork_until_cancelled(feast, philo, hand1))
		return (NULL);
	if (pickup_fork_until_cancelled(feast, philo, hand2))
		return (drop_forks(hand1, NULL));
	philo->last_satiated = ms_since(feast->serve_time);
	pthread_mutex_lock(&feast->status_check);
	philo_log(philo, "is eating", false);
	pthread_mutex_unlock(&feast->status_check);
	if (usleep_until_cancelled(feast, feast->time_to_eat))
		return (drop_forks(hand1, hand2));
	drop_forks(hand1, hand2);
	pthread_mutex_lock(&feast->status_check);
	philo->ate++;
	pthread_mutex_unlock(&feast->status_check);
	return (true);
}

static void	philoop(t_feast *feast, t_philo *philo)
{
	while (!is_cancelled(feast))
	{
		if (!eat(feast, philo, &philo->hands[0], &philo->hands[1]))
			return ;
		pthread_mutex_lock(&feast->status_check);
		philo_log(philo, "is sleeping", false);
		pthread_mutex_unlock(&feast->status_check);
		if (usleep_until_cancelled(feast, feast->time_to_sleep))
			return ;
		if (is_cancelled(feast) || philo->ate == feast->must_eat)
			return ;
		pthread_mutex_lock(&feast->status_check);
		philo_log(philo, "is thinking", false);
		pthread_mutex_unlock(&feast->status_check);
		if ((feast->time_to_eat + feast->time_to_sleep) < feast->time_to_die)
		{
			if (usleep_until_cancelled(
					feast,
					(feast->time_to_die
						- feast->time_to_sleep - feast->time_to_eat) / 2))
				return ;
		}
	}
}

void	*philo_routine(void *arg)
{
	t_feast			*feast;
	t_philo			*philo;

	philo = (t_philo *)arg;
	feast = philo->feast;
	pthread_mutex_lock(&feast->status_check);
	if (feast->status == CANCELLED)
	{
		pthread_mutex_unlock(&feast->status_check);
		return (NULL);
	}
	philo_log(philo, "is thinking", false);
	pthread_mutex_unlock(&feast->status_check);
	if (philo->id % 2)
	{
		if (usleep_until_cancelled(feast, feast->time_to_eat))
			return (NULL);
	}
	philoop(feast, philo);
	return (NULL);
}
