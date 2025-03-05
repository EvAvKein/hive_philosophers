/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:47:39 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/05 09:23:29 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(char *str)
{
	size_t i;

	i = 0;
	while(str[i])
		i++;
	return (i);
}

long	ms_since(struct timeval time)
{
	struct	timeval		current;
	gettimeofday(&current, NULL);
	return (((current.tv_sec * 1000) + (current.tv_usec / 1000))
		- ((time.tv_sec * 1000) + (time.tv_usec / 1000)));
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
		printf("[%li] %i %s\n",
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
	long duration, bool eating)
{	
	struct timeval	start;

	gettimeofday(&start, NULL);
	while (ms_since(start) < duration)
	{
		if (feast->status == CANCELLED)
			return (true);
		if (!eating && starved_to_death(feast, philo))
			return (true);
		usleep(1000);
	}
	return (feast->status == CANCELLED);
}
