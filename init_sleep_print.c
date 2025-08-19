/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_sleep_print.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oislamog <oislamog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:57:19 by oislamog          #+#    #+#             */
/*   Updated: 2025/08/16 15:49:19 by oislamog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_data(t_data *data, char **argv)
{
	int	i;

	i = 0;
	data->number_of_philo = ft_atol(argv[1]);
	data->time_to_die = ft_atol(argv[2]);
	data->time_to_eat = ft_atol(argv[3]);
	data->time_to_sleep = ft_atol(argv[4]);
	data->must_eat = -1;
	if (argv[5])
		data->must_eat = ft_atol(argv[5]);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->number_of_philo);
	while (i < data->number_of_philo)
	{
		pthread_mutex_init(&data->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&data->write_lock, NULL);
	pthread_mutex_init(&data->death_lock, NULL);
	data->someone_died = 0;
	data->start_time = current_time();
}

void	init_philos(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philo)
	{
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].last_eat = data->start_time;
		philos[i].eat_count = 0;
		philos[i].has_left_fork = 0;
		philos[i].has_right_fork = 0;
		i++;
	}
}

size_t	current_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	smart_sleep(size_t duration, t_philo *philo)
{
	size_t	start;
	int		should_continue ;

	start = current_time();
	pthread_mutex_lock(&philo->data->death_lock);
	should_continue = !philo->data->someone_died;
	pthread_mutex_unlock(&philo->data->death_lock);
	while (should_continue && (current_time() - start < duration))
	{
		usleep(500);
		pthread_mutex_lock(&philo->data->death_lock);
		should_continue = !philo->data->someone_died;
		pthread_mutex_unlock(&philo->data->death_lock);
	}
}

void	print_action(t_philo *philo, char *message)
{
	pthread_mutex_lock(&philo->data->death_lock);
	if (!philo->data->someone_died)
	{
		pthread_mutex_unlock(&philo->data->death_lock);
		pthread_mutex_lock(&philo->data->write_lock);
		pthread_mutex_lock(&philo->data->death_lock);
		if (!philo->data->someone_died)
			printf("%zu %d %s\n", current_time() - philo->data->start_time,
				philo->id, message);
		pthread_mutex_unlock(&philo->data->death_lock);
		pthread_mutex_unlock(&philo->data->write_lock);
	}
	else
		pthread_mutex_unlock(&philo->data->death_lock);
}
