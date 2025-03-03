/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 09:18:19 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/03 18:02:38 by ekeinan          ###   ########.fr       */
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

typedef struct s_philo_args {
	int		num_of_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		must_eat;
}			t_philo_args;

# define CANCELLED	0
# define COOKING	1
# define SERVED		2

typedef struct	s_feast
{
	int				status;
	int				num_of_philos;
	pthread_mutex_t	stenographer;
	pthread_mutex_t	*forks;
	pthread_t		*threads;
	struct s_philo	*philos;
	struct timeval	serve_time;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
}					t_feast;

typedef struct	s_philo_init_data
{
	t_philo_args	args;
	t_feast			*feast;
	int				id;
}					t_philo_init_data;

typedef struct	s_philo
{
	t_feast				*feast;
	struct s_philo		*next;
	pthread_mutex_t		*forks[2];
	int					id;
	struct timeval		last_satiated;
	int					ate;
	bool				dead;
}						t_philo;

bool	launch_feast(t_feast *feast, t_philo_args data);
void	*philo_routine(void *arg);
void	wait_for_philos(t_feast *feast);
bool	end_feast(t_feast *feast, char *announcement);

size_t	ft_strlen(char *str);
long	ms_since(struct timeval time);
void	*philo_log(t_philo *philo, char *action);
bool	starved_to_death(t_feast *feast, t_philo *philo);
bool	usleep_until_death(t_feast *feast, t_philo *philo, int ms_time);

#endif