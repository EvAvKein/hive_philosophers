/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:37:34 by ekeinan           #+#    #+#             */
/*   Updated: 2025/02/27 12:59:11 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	*philo_routine(void *arg)
{
	t_philo *philo;
	t_feast *feast;

	philo = arg;
	feast = philo->feast;
	while (1)
	{
		if (feast->status == COOKING)
			continue ;
		if (feast->status == CANCELLED)
			// pthread_detach();
		if (feast->status == SERVED)
			break ;
	}
	write(1, philo.id, 1);
	// pthread_detach();
}



static	bool init_feast(t_feast *feast, t_philo_args data)
{
	t_philo		*philo;
	pthread_t	thread;
	int			*arg;
	int			philo_i;

	philo_i = 0;
	while (philo_i < data.num_of_philos)
	{
		philo = malloc(sizeof(t_philo));
		if (!philo)
			return (destroy_philos());
		if (!pthread_create(&thread, NULL, philo_routine, arg))
			return (destroy_philos());
		philo->thread = thread;
		
	}
	
}

int	philosophers(t_philo_args data)
{
	t_feast feast;

	if (!init_feast(&feast, data))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);	
}