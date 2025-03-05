/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:12:53 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/05 20:12:37 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_for_philos(t_feast *feast)
{
	int			i;

	if (feast->threads)
	{
		i = 0;
		while (i < feast->num_of_philos)
			pthread_join(feast->threads[i++], NULL);
	}
}

static void	destroy_all_forks(t_fork *forks, int fork_count)
{
	while (fork_count--)
		pthread_mutex_destroy(&forks[fork_count].mutex);
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
		destroy_all_forks(feast->forks, feast->num_of_philos);
		free(feast->forks);
	}
	free_all_philos(feast->philos);
	return (false);
}
