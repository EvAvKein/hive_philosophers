/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:05:38 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/01 19:41:28 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *arg)
{
	t_feast *feast;
	t_philo *philo;
	
	philo = (t_philo *)arg;
	feast = philo->feast;
	while (1)
	{
		if (feast->status == COOKING)
			continue ;
		if (feast->status == SERVED)
			break ;
		if (feast->status == CANCELLED)
			return (NULL);
	}
	while (feast->status != CANCELLED)
	{
		
	}
	return (NULL);
}
