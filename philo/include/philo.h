/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 09:18:19 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/18 10:29:04 by ekeinan          ###   ########.fr       */
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

# if COLOR
#  define CLR_RESET "\e[0;37m"
#  define CLR_VERB 	"\e[2;37m"
#  define CLR_THINK "\e[1;35m"
#  define CLR_FORK 	"\e[2;32m"
#  define CLR_EAT 	"\e[1;32m"
#  define CLR_SLEEP "\e[1;36m"
#  define CLR_DIED 	"\e[1;31m"
#  define CLR_TIME 	"\e[1;30m"
# else
#  define COLOR 0
#  define CLR_RESET ""
#  define CLR_VERB 	""
#  define CLR_THINK ""
#  define CLR_FORK	""
#  define CLR_EAT	""
#  define CLR_SLEEP ""
#  define CLR_DIED	""
#  define CLR_TIME	""
# endif

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

typedef struct s_philo_hand
{
	pthread_mutex_t	*fork;
	bool			gripping;
}					t_philo_hand;

typedef struct s_philo
{
	struct s_feast	*feast;
	t_philo_hand	hands[2];
	int				id;
	atomic_long		last_satiated;
	atomic_int		ate;
	struct s_philo	*next;
}					t_philo;

typedef struct s_staff
{
	pthread_mutex_t	greeter;
	pthread_mutex_t	stenographer;
	pthread_mutex_t	fork_coordinator;
	bool			greeter_staffed;
	bool			stenographer_staffed;
	bool			fork_coordinator_staffed;
}					t_staff;

typedef struct s_feast
{
	int				num_of_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	t_staff			staff;
	atomic_int		status;
	pthread_mutex_t	*forks;
	t_philo			*philos;
	pthread_t		*philo_threads;
	pthread_t		grim_reaper;
	struct timeval	serve_time;
}					t_feast;

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
bool	drop_forks(t_feast *feast, t_philo_hand *hand1, t_philo_hand *hand2);
bool	usleep_until_cancelled(t_feast *feast, long ms_duration);

#endif