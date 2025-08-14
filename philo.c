/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ancengiz <ancengiz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:57:51 by oislamog          #+#    #+#             */
/*   Updated: 2025/08/14 19:55:31 by ancengiz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->number_of_philo == 1)
		return (print_action(philo, "has taken a fork"),
			smart_sleep(philo->data->time_to_die + 1, philo), NULL);
	if (philo->id % 2 == 0)
		smart_sleep(1, philo);
	while (1)
	{
		if (is_someone_died(philo->data))
			break ;
		print_action(philo, "is thinking");
		if (is_someone_died(philo->data))
			break ;
		take_forks(philo);
		if (is_someone_died(philo->data))
			return (put_down_forks(philo), NULL);
		eat(philo);
		put_down_forks(philo);
		if (is_someone_died(philo->data))
			break ;
		print_action(philo, "is sleeping");
		smart_sleep(philo->data->time_to_sleep, philo);
	}
	return (NULL);
}

void	*observer_fn(void *arg)
{
	t_philo		*philos;
	t_data		*data;

	philos = (t_philo *)arg;
	data = philos[0].data;
	while (1)
	{
		if (is_someone_died(data))
			break ;
		if (data->must_eat > 0 && not_enough_eat(philos))
		{
			set_someone_died(data);
			return (NULL);
		}
		if (check_death(philos) == 1)
			return (NULL);
		usleep(1000);
	}
	return (NULL);
}

void	manage_threads(t_philo *philos, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philo)
	{
		pthread_create(&philos[i].thread, NULL, philosopher, &philos[i]);
		i++;
	}
	pthread_create(&data->observer, NULL, observer_fn, philos);
	i = 0;
	while (i < data->number_of_philo)
		pthread_join(philos[i++].thread, NULL);
	pthread_join(data->observer, NULL);
}

int	main(int argc, char **argv)
{
	t_data	data;
	t_philo	*philos;

	if ((argc != 5 && argc != 6) || checker(argv, argc))
		return (printf("PLEASE GİVE A RIGHT NUMBER\n"), 1);
	init_data(&data, argv);
	philos = malloc(sizeof(t_philo) * data.number_of_philo);
	init_philos(philos, &data);
	manage_threads(philos, &data);
	destroy_mutex(&data);
	free(philos);
	free(data.forks);
	return (0);
}

void	destroy_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_philo)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->death_lock);
}
