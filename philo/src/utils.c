/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:47:39 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/05 17:15:54 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ms_of(struct timeval timestamp)
{
	return (((timestamp.tv_sec * 1000) + (timestamp.tv_usec / 1000)));
}

long	ms_now()
{
	struct	timeval		current;
	gettimeofday(&current, NULL);
	return (ms_of(current));
}

long	ms_since(struct timeval time)
{
	return (ms_now() - ms_of(time));
}

void	*philo_log(t_philo *philo, char *action)
{
	if (philo->dead || philo->feast->status == SERVED)
	{
		pthread_mutex_lock(&philo->feast->stenographer);
		if (!philo->dead && philo->feast->status == CANCELLED)
		{
			pthread_mutex_unlock(&philo->feast->stenographer);
			return (NULL);
		}
		printf("%li %i %s\n",
			ms_since(philo->feast->serve_time), philo->id, action);
		pthread_mutex_unlock(&philo->feast->stenographer);
	}
	return (NULL);
}

bool	starved_to_death(t_feast *feast, t_philo *philo)
{	
	if (ms_since(philo->last_satiated) > feast->time_to_die)
	{
		if (feast->status == SERVED)
			philo->dead = true;
		feast->status = CANCELLED;
		philo_log(philo, "died");
		return (true);
	}
	return (false);
}

bool	usleep_until_death(t_feast *feast, t_philo *philo,
	long ms_duration, bool eating)
{	
	struct timeval	start;

	gettimeofday(&start, NULL);
	while (ms_since(start) < ms_duration)
	{
		if (feast->status == CANCELLED)
			return (true);
		if (!eating && starved_to_death(feast, philo))
			return (true);
		usleep(1000);
	}
	return (feast->status == CANCELLED);
}
