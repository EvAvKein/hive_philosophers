/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 14:12:53 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/16 16:20:41 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	wait_for_everyone(t_feast *feast)
{
	int			i;

	if (feast->philo_threads)
	{
		i = 0;
		while (i < feast->num_of_philos && feast->philo_threads[i])
			pthread_join(feast->philo_threads[i++], NULL);
	}
	if (feast->grim_reaper)
		pthread_join(feast->grim_reaper, NULL);
}

static void	destroy_all_forks(t_feast *feast)
{
	int			i;

	if (feast->forks)
	{
		i = 0;
		while (i < feast->num_of_philos)
			pthread_mutex_destroy(&feast->forks[i++]);
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

static void	dismiss_staff(t_feast *feast)
{
	pthread_mutex_destroy(&feast->greeter);
	pthread_mutex_destroy(&feast->stenographer);
	pthread_mutex_destroy(&feast->fork_coordinator);
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
	if (feast->philo_threads)
	{
		wait_for_everyone(feast);
		free(feast->philo_threads);
	}
	if (feast->forks)
	{
		destroy_all_forks(feast);
		free(feast->forks);
		feast->forks = NULL;
	}
	free_all_philos(feast->philos);
	dismiss_staff(feast);
	return (false);
}
