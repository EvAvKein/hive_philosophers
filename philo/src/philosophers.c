/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:37:34 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/01 19:15:25 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	prepare_feast(t_feast *feast, t_philo_args data)
{
	*feast = (t_feast){.status = COOKING, .philos = NULL,
		.num_of_philos = data.num_of_philos,
		.forks = malloc(sizeof(pthread_mutex_t) * (data.num_of_philos + 1)),
		.threads = malloc(sizeof(pthread_t) * (data.num_of_philos + 1))};
	if (!feast->forks || !feast->threads)
	{
		if (feast->forks)
			free(feast->forks);
		if (feast->threads)
			free(feast->threads);
		write(2, "Feast off: A feast malloc failed :(\n", 37);
		return (false);
	}
	memset(feast->threads, '\0',
		sizeof(pthread_t) * (data.num_of_philos + 1));
	memset(feast->forks, '\0',
		sizeof(pthread_mutex_t) * (data.num_of_philos + 1));
	feast->num_of_philos = data.num_of_philos;
	return (true);
}

int	philosophers(t_philo_args data)
{
	t_feast feast;

	if (!prepare_feast(&feast, data))
		return (EXIT_FAILURE);
	if (!launch_feast(&feast, data))
		return (EXIT_FAILURE);
	end_feast(&feast, NULL);
	return (EXIT_SUCCESS);	
}
