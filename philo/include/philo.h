/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 09:18:19 by ekeinan           #+#    #+#             */
/*   Updated: 2025/02/27 12:55:14 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdbool.h>
# include <pthread.h>

typedef struct s_philo_args {
	size_t	num_of_philos;
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
	int			status;
	t_philo		**philos;
}				t_feast;

# define ABSENT		0
# define DEAD		1
# define SLEEPING	2
# define THINKING	3
# define EATING		4
# define SATIATED	5

typedef struct	s_philo
{
	t_feast			*feast;
	struct s_philo	prev;
	struct s_philo	next;
	pthread_t		thread;
	pthread_mutex_t	fork;
	char			*id;
	int				status;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat;
	int				ate;
}				t_philo;

int	philosophers(t_philo_args data);

#endif