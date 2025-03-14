/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:05:38 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/14 12:46:27 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	pickup_fork_until_cancelled(
	t_feast *feast, t_philo *philo, t_philo_hand *hand)
{
	pthread_mutex_lock(hand->fork);
	hand->gripping = true;
	if (feast->status == CANCELLED)
	{
		pthread_mutex_unlock(hand->fork);
		hand->gripping = false;
		return (true);
	}
	philo_log(philo, CLR_VERB "has" CLR_FORK " taken a fork" CLR_RESET, false);
	return (false);
}

static bool	eat(t_feast *feast, t_philo *philo,
	t_philo_hand *hand1, t_philo_hand *hand2)
{
	if (feast->status == CANCELLED)
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
	philo_log(philo, CLR_VERB "is" CLR_EAT " eating" CLR_RESET, false);
	if (usleep_until_cancelled(feast, feast->time_to_eat))
		return (drop_forks(hand1, hand2));
	drop_forks(hand1, hand2);
	philo->ate++;
	return (true);
}

static void	philoop(t_feast *feast, t_philo *philo)
{
	while (feast->status != CANCELLED)
	{
		if (!eat(feast, philo, &philo->hands[0], &philo->hands[1]))
			return ;
		philo_log(philo, CLR_VERB "is" CLR_SLEEP " sleeping" CLR_RESET, false);
		if (usleep_until_cancelled(feast, feast->time_to_sleep))
			return ;
		if (feast->status == CANCELLED || philo->ate == feast->must_eat)
			return ;
		philo_log(philo, CLR_VERB "is" CLR_THINK " thinking" CLR_RESET, false);
		if ((feast->time_to_eat + feast->time_to_sleep)
			< (feast->time_to_die - 10))
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
	pthread_mutex_lock(&feast->greeter);
	if (feast->status == CANCELLED)
	{
		pthread_mutex_unlock(&feast->greeter);
		return (NULL);
	}
	pthread_mutex_unlock(&feast->greeter);
	philo_log(philo, CLR_VERB "is" CLR_THINK " thinking" CLR_RESET, false);
	if (philo->id % 2)
	{
		if (usleep_until_cancelled(feast, feast->time_to_eat))
			return (NULL);
	}
	philoop(feast, philo);
	return (NULL);
}
