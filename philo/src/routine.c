/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 13:05:38 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/01 18:55:43 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *arg)
{
	t_philo *philo;
	char	c;
	
	philo = (t_philo *)arg;
	c = '0' + philo->id;
	write(STDOUT_FILENO, &c, 1);
	return (NULL);
	// t_feast *feast;

	// feast = philo->feast;
	// while (1)
	// {
	// 	if (feast->status == COOKING)
	// 		continue ;
	// 	if (feast->status == CANCELLED)
	// 		return ;
	// 	if (feast->status == SERVED)
	// 		break ;
	// }
	// write(1, philo->id, 1);
}