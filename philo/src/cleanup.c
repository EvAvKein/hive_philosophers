/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:12:53 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/06 14:07:15 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_for_philos(t_feast *feast)
{
	int			i;

	if (feast->threads)
	{
		i = 0;
		while (i < feast->num_of_philos && feast->threads[i])
			pthread_join(feast->threads[i++], NULL);
	}
}

static void	free_all_philos(t_philo *philos)
{
	t_philo		*first_philo;
	t_philo		*current_philo;
	t_philo		*next_philo;

	first_philo = philos;
	current_philo = first_philo;
	while (current_philo)
	{
		next_philo = current_philo->next;
		free(current_philo);
		if (next_philo == first_philo)
			break ;
		current_philo = next_philo;
	}
}

static void	destroy_all_forks(t_feast *feast)
{
	int			i;

	if (feast->forks)
	{
		i = 0;
		while (i < feast->num_of_philos)
			pthread_mutex_destroy(&feast->forks[i++].mutex);
	}
}

bool	end_feast(t_feast *feast, char *announcement)
{
	size_t	len;

	feast->status = CANCELLED;
	if (announcement)
	{
		len = 0;
		while (announcement[len])
			len++;
		write(STDERR_FILENO, announcement, len);
	}
	if (feast->threads)
	{
		wait_for_philos(feast);
		free(feast->threads);
	}
	if (feast->forks)
	{
		destroy_all_forks(feast);
		free(feast->forks);
	}
	free_all_philos(feast->philos);
	return (false);
}
