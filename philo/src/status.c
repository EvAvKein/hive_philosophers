/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 15:27:03 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/09 16:09:04 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_cancelled(t_feast *feast)
{
	bool	cancelled;

	pthread_mutex_lock(&feast->status_check);
	cancelled = feast->status == CANCELLED;
	pthread_mutex_unlock(&feast->status_check);
	return (cancelled);
}

void	set_status(t_feast *feast, t_status	status)
{
	pthread_mutex_lock(&feast->status_check);
	feast->status = status;
	pthread_mutex_unlock(&feast->status_check);
}
