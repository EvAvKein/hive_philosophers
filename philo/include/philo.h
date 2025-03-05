/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 09:18:19 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/05 20:30:11 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <string.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_philo_args
{
	int		num_of_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		must_eat;
}			t_philo_args;

typedef struct s_fork
{
	pthread_mutex_t	mutex;
	bool			available;
}			t_fork;

# define CANCELLED	0
# define COOKING	1
# define SERVED		2

typedef struct s_feast
{
	int				status;
	int				num_of_philos;
	pthread_mutex_t	stenographer;
	t_fork			*forks;
	pthread_t		*threads;
	struct s_philo	*philos;
	struct timeval	serve_time;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
}					t_feast;

typedef struct s_philo
{
	t_feast				*feast;
	struct s_philo		*next;
	t_fork				*forks[2];
	int					id;
	struct timeval		last_satiated;
	int					ate;
	bool				dead;
}						t_philo;

bool	launch_feast(t_feast *feast, t_philo_args data);
void	*philo_routine(void *arg);
void	wait_for_philos(t_feast *feast);
bool	end_feast(t_feast *feast, char *announcement);

long	ms_of(struct timeval timestamp);
long	ms_now(void);
long	ms_since(struct timeval time);

void	*philo_log(t_philo *philo, char *action);
bool	drop_forks(t_fork *fork1, t_fork *fork2);
bool	starved_to_death(t_feast *feast, t_philo *philo);
bool	usleep_until_death(t_feast *feast, t_philo *philo,
			long ms_duration, bool eating);

#endif