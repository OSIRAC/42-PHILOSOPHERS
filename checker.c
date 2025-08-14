/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oislamog <oislamog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:57:12 by oislamog          #+#    #+#             */
/*   Updated: 2025/05/27 16:13:30 by oislamog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	checker(char **str, int nbr)
{
	int	i;
	int	j;

	i = 1;
	j = 0;
	while (i < nbr)
	{
		j = 0;
		if ((str[i][j] == '+') && str[i][j + 1] != '\0')
			j++;
		while (str[i][j] == '0')
			j++;
		if (ft_strlen (&str[i][j]) > 10)
			return (1);
		while (str[i][j])
		{
			if (!(str[i][j] >= '0' && str[i][j] <= '9'))
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

long	ft_atol(char *str)
{
	long	result;
	int		sign;

	result = 0;
	sign = 1;
	while (*str == 32 || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		sign = -1;
	if (*str == '-' || *str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str - '0';
		str++;
	}
	return (sign * result);
}

int	not_enough_eat(t_philo *philo)
{
	int	i;
	int	philo_count;
	int	eat_count;

	i = 0;
	philo_count = philo->data->number_of_philo;
	while (i < philo_count)
	{
		pthread_mutex_lock(&philo->data->death_lock);
		eat_count = philo[i].eat_count;
		pthread_mutex_unlock(&philo->data->death_lock);
		if (eat_count < philo->data->must_eat)
			return (0);
		i++;
	}
	return (1);
}

int	check_death(t_philo *philo)
{
	int		i;
	size_t	time_since_last_eat;
	size_t	current;

	i = 0;
	while (i < philo->data->number_of_philo)
	{
		pthread_mutex_lock(&philo->data->death_lock);
		current = current_time();
		time_since_last_eat = current - philo[i].last_eat;
		if (time_since_last_eat > philo->data->time_to_die)
		{
			philo->data->someone_died = 1;
			pthread_mutex_unlock(&philo->data->death_lock);
			pthread_mutex_lock(&philo->data->write_lock);
			printf("%zu %d died\n", current - philo->data->start_time,
				philo[i].id);
			pthread_mutex_unlock(&philo->data->write_lock);
			return (1);
		}
		pthread_mutex_unlock(&philo->data->death_lock);
		i++;
	}
	return (0);
}
