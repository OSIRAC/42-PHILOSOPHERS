/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oislamog <oislamog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:57:04 by oislamog          #+#    #+#             */
/*   Updated: 2025/08/16 16:15:27 by oislamog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	philo_take_one_fork(t_philo *philo, int fork_id, int *has_fork)
{
	pthread_mutex_lock(&philo->data->forks[fork_id]);
	*has_fork = 1;
	pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		pthread_mutex_unlock(&philo->data->forks[fork_id]);
		*has_fork = 0;
		return (0);
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	print_action(philo, "has taken a fork");
	return (1);
}

void	philo_take_right_first(t_philo *philo)
{
	int	id;
	int	left;
	int	right;

	id = philo->id;
	left = id - 1;
	right = id % philo->data->number_of_philo;
	if (!philo_take_one_fork(philo, right, &philo->has_right_fork))
		return ;
	if (!philo_take_one_fork(philo, left, &philo->has_left_fork))
	{
		pthread_mutex_unlock(&philo->data->forks[right]);
		philo->has_right_fork = 0;
	}
}

void	philo_take_left_first(t_philo *philo)
{
	int	id;
	int	left;
	int	right;

	id = philo->id;
	left = id - 1;
	right = id % philo->data->number_of_philo;
	if (!philo_take_one_fork(philo, left, &philo->has_left_fork))
		return ;
	if (!philo_take_one_fork(philo, right, &philo->has_right_fork))
	{
		pthread_mutex_unlock(&philo->data->forks[left]);
		philo->has_left_fork = 0;
	}
}

void	take_forks(t_philo *philo)
{
	int	id;

	id = philo->id;
	if (id % 2 == 0)
		philo_take_left_first(philo);
	else
		philo_take_right_first(philo);
}

void	put_down_forks(t_philo *philo)
{
	int	id;
	int	left;
	int	right;

	id = philo->id;
	left = id - 1;
	right = (id % philo->data->number_of_philo);
	if (philo->has_left_fork)
	{
		pthread_mutex_unlock(&philo->data->forks[left]);
		philo->has_left_fork = 0;
	}
	if (philo->has_right_fork)
	{
		pthread_mutex_unlock(&philo->data->forks[right]);
		philo->has_right_fork = 0;
	}
}
