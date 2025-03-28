/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:47:39 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/17 15:48:52 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi_positive_strict(char *str)
{
	size_t	i;
	int		num;

	if (!str)
		return (-1);
	i = 0;
	num = 0;
	if (str[i] == '+')
		i++;
	if (!str[i] || !(str[i] >= '0' && str[i] <= '9'))
		return (-1);
	while (str[i] >= '0' && str[i] <= '9')
	{
		num = (num * 10) + (str[i++] - '0');
		if (num < 0)
			return (-1);
	}
	if (str[i])
		return (-1);
	return (num);
}

void	*philo_log(t_philo *philo, char *action, bool death)
{
	pthread_mutex_lock(&philo->feast->staff.stenographer);
	if (!death && philo->feast->status == CANCELLED)
	{
		pthread_mutex_unlock(&philo->feast->staff.stenographer);
		return (NULL);
	}
	printf(CLR_TIME "%li" CLR_RESET " %i %s\n",
		ms_since(philo->feast->serve_time), philo->id, action);
	pthread_mutex_unlock(&philo->feast->staff.stenographer);
	return (NULL);
}

bool	usleep_until_cancelled(t_feast *feast, long ms_duration)
{
	struct timeval	start;

	gettimeofday(&start, NULL);
	while (ms_since(start) < ms_duration)
	{
		if (feast->status == CANCELLED)
			return (true);
		usleep(1000);
	}
	return (feast->status == CANCELLED);
}

bool	drop_forks(t_feast *feast, t_philo_hand *hand1, t_philo_hand *hand2)
{
	pthread_mutex_lock(&feast->staff.fork_coordinator);
	if (hand1)
	{
		pthread_mutex_unlock(hand1->fork);
		hand1->gripping = false;
	}
	if (hand2)
	{
		pthread_mutex_unlock(hand2->fork);
		hand2->gripping = false;
	}
	pthread_mutex_unlock(&feast->staff.fork_coordinator);
	return (false);
}
