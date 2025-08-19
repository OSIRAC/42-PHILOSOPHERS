/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_util2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oislamog <oislamog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:51:21 by oislamog          #+#    #+#             */
/*   Updated: 2025/08/16 15:54:48 by oislamog         ###   ########.fr       */
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

int	philo_think(t_philo *philo)
{
	print_action(philo, "is thinking");
	if (philo->data->number_of_philo % 2 == 1)
		smart_sleep(philo->data->time_to_eat / 2, philo);
	pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->someone_died)
		return (pthread_mutex_unlock(&philo->data->death_lock), 0);
	pthread_mutex_unlock(&philo->data->death_lock);
	return (1);
}

int	philo_try_to_eat(t_philo *philo)
{
	take_forks(philo);
	pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		put_down_forks(philo);
		return (0);
	}
	pthread_mutex_unlock(&philo->data->death_lock);
	eat(philo);
	put_down_forks(philo);
	return (1);
}

int	philo_sleep(t_philo *philo)
{
	print_action(philo, "is sleeping");
	smart_sleep(philo->data->time_to_sleep, philo);
	pthread_mutex_lock(&philo->data->death_lock);
	if (philo->data->someone_died)
		return (pthread_mutex_unlock(&philo->data->death_lock), 0);
	pthread_mutex_unlock(&philo->data->death_lock);
	return (1);
}

void	*philosopher_loop(t_philo *philo)
{
	int	should_continue ;

	pthread_mutex_lock(&philo->data->death_lock);
	should_continue = !philo->data->someone_died;
	pthread_mutex_unlock(&philo->data->death_lock);
	while (should_continue)
	{
		if (!philo_think(philo))
			break ;
		if (!philo_try_to_eat(philo))
			break ;
		if (!philo_sleep(philo))
			break ;
		pthread_mutex_lock(&philo->data->death_lock);
		should_continue = !philo->data->someone_died;
		pthread_mutex_unlock(&philo->data->death_lock);
	}
	return (NULL);
}
