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

void	philo_take(t_philo *philo)
{
	int	id;
	int	left;
	int	right;

	id = philo->id;
	left = id - 1;
	right = id % philo->data->number_of_philo;
	pthread_mutex_lock(&philo->data->forks[right]);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->forks[right]);
		return ;
	}
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks[left]);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->forks[left]);
		pthread_mutex_unlock(&philo->data->forks[right]);
		return ;
	}
	print_action(philo, "has taken a fork");
}

void	philo_take2(t_philo *philo)
{
	int	id;
	int	left;
	int	right;

	id = philo->id;
	left = id - 1;
	right = id % philo->data->number_of_philo;
	pthread_mutex_lock(&philo->data->forks[left]);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->forks[left]);
		return ;
	}
	print_action(philo, "has taken a fork");
	pthread_mutex_lock(&philo->data->forks[right]);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->forks[right]);
		pthread_mutex_unlock(&philo->data->forks[left]);
		return ;
	}
	print_action(philo, "has taken a fork");
}

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
	int	number;

	number = philo->data->number_of_philo;
	id = philo->id;
	if (id == number && number % 2 == 1)
	{
		philo_take(philo);
	}
	else if (id % 2 == 0)
	{
		philo_take(philo);
	}
	else
	{
		philo_take2(philo);
	}
}

void	put_down_forks(t_philo *philo)
{
	int	id;
	int	left;
	int	right;

	id = philo->id;
	left = id - 1;
	right = (id % philo->data->number_of_philo);
	pthread_mutex_unlock(&philo->data->forks[left]);
	pthread_mutex_unlock(&philo->data->forks[right]);
}
