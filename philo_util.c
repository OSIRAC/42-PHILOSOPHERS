/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_util.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oislamog <oislamog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:57:04 by oislamog          #+#    #+#             */
/*   Updated: 2025/05/27 15:57:04 by oislamog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	print_action(philo, "is eating");
	pthread_mutex_lock(&philo->data->death_lock);
	philo->last_eat = current_time();
	pthread_mutex_unlock(&philo->data->death_lock);
	smart_sleep(philo->data->time_to_eat, philo);
	pthread_mutex_lock(&philo->data->death_lock);
	philo->eat_count++;
	pthread_mutex_unlock(&philo->data->death_lock);
}

void	take_forks(t_philo *philo)
{
	int	id;
	int	left;
	int	right;
	int	first;
	int	second;

	id = philo->id;
	left = id - 1;
	right = id % philo->data->number_of_philo;
	philo->has_left_fork = 0;
	philo->has_right_fork = 0;
	if (left < right)
	{
		first = left;
		second = right;
	}
	else
	{
		first = right;
		second = left;
	}
	pthread_mutex_lock(&philo->data->forks[first]);
	if (is_someone_died(philo->data))
	{
		pthread_mutex_unlock(&philo->data->forks[first]);
		return ;
	}
	if (first == left)
		philo->has_left_fork = 1;
	else
		philo->has_right_fork = 1;
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks[second]);
	if (is_someone_died(philo->data))
	{
		pthread_mutex_unlock(&philo->data->forks[second]);
		pthread_mutex_unlock(&philo->data->forks[first]);
		philo->has_left_fork = 0;
		philo->has_right_fork = 0;
		return ;
	}
	if (second == left)
		philo->has_left_fork = 1;
	else
		philo->has_right_fork = 1;
	print_action(philo, "has taken a fork");
}

void	put_down_forks(t_philo *philo)
{
	int	id;
	int	left;
	int	right;

	id = philo->id;
	left = id - 1;
	right = (id % philo->data->number_of_philo);
	if (philo->has_right_fork)
	{
		pthread_mutex_unlock(&philo->data->forks[right]);
		philo->has_right_fork = 0;
	}
	if (philo->has_left_fork)
	{
		pthread_mutex_unlock(&philo->data->forks[left]);
		philo->has_left_fork = 0;
	}
}
