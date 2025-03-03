/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:47:39 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/03 16:21:50 by ekeinan          ###   ########.fr       */
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

int		ms_since(struct timeval time)
{
	struct	timeval		current;

	gettimeofday(&current, NULL);
	return ((current.tv_usec - time.tv_usec) / 1000);
}

void	*philo_log(t_philo *philo, char *action)
{
	if (philo->dead || philo->feast->status == SERVED)
		printf("%i %i %s\n",
			ms_since(philo->feast->serve_time), philo->id, action);
	return (NULL);
}

bool	starved_to_death(t_feast *feast, t_philo *philo)
{	
	if (ms_since(philo->last_satiated) > feast->time_to_die)
	{
		feast->status = CANCELLED;
		philo->dead = true;
		philo_log(philo, "died");
		return (true);
	}
	return (false);
}

bool	usleep_until_death(t_feast *feast, t_philo *philo, int ms_time)
{
	while (ms_time)
	{
		if (feast->status == CANCELLED || starved_to_death(feast, philo))
			return (true);
		usleep(1000);
		ms_time--;
	}
	return (feast->status == CANCELLED);
}
