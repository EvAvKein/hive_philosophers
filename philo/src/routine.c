/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:05:38 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/01 23:30:39 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *arg)
{
	t_feast *feast;
	t_philo *philo;
	// char	c;
	
	philo = (t_philo *)arg;
	feast = philo->feast;
	// c = '0' + philo->id;
	// write(STDOUT_FILENO, &c, 1);
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
