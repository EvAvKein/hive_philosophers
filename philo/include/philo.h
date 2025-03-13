/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 09:18:19 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/13 18:51:19by ekeinan          ###   ########.fr       */
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
# include <stdatomic.h>

typedef struct s_philo_args
{
	int		num_of_philos;
	int		time_to_die;
	int		time_to_eat;
	int		time_to_sleep;
	int		must_eat;
}			t_philo_args;

typedef enum e_status
{
	CANCELLED,
	COOKING,
	CRAVINGS,
}			t_status;

typedef struct s_feast
{
	atomic_int		status;
	pthread_mutex_t	greeter;
	pthread_mutex_t	stenographer;
	pthread_mutex_t	*forks;
	pthread_t		grim_reaper;
	pthread_t		*philo_threads;
	struct s_philo	*philos;
	struct timeval	serve_time;
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
}					t_feast;

typedef struct s_philo_hand
{
	pthread_mutex_t	*fork;
	bool			gripping;
}					t_philo_hand;

typedef struct s_philo
{
	t_feast			*feast;
	struct s_philo	*next;
	t_philo_hand	hands[2];
	int				id;
	atomic_long		last_satiated;
	atomic_int		ate;
}						t_philo;

bool	launch_feast(t_feast *feast, t_philo_args data);
void	*grim_reaper_routine(void *arg);
void	*philo_routine(void *arg);
void	wait_for_everyone(t_feast *feast);
bool	end_feast(t_feast *feast, char *announcement);

long	ms_of(struct timeval timestamp);
long	ms_now(void);
long	ms_since(struct timeval time);

int		ft_atoi_positive_strict(char *str);
void	*philo_log(t_philo *philo, char *action, bool death);
bool	drop_forks(t_philo_hand *hand1, t_philo_hand *hand2);
bool	usleep_until_cancelled(t_feast *feast, long ms_duration);

#endif