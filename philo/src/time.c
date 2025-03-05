/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 19:37:46 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/05 19:39:21 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ms_of(struct timeval timestamp)
{
	return (((timestamp.tv_sec * 1000) + (timestamp.tv_usec / 1000)));
}

long	ms_now()
{
	struct	timeval		current;
	gettimeofday(&current, NULL);
	return (ms_of(current));
}

long	ms_since(struct timeval time)
{
	return (ms_now() - ms_of(time));
}
