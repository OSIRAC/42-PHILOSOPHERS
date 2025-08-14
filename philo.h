/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oislamog <oislamog@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 15:58:14 by oislamog          #+#    #+#             */
/*   Updated: 2025/05/27 15:58:18 by oislamog         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_data
{
	int				number_of_philo;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	pthread_mutex_t	death_lock;
	pthread_t		observer;
	size_t			start_time;
	int				someone_died;
	int				must_eat;
}	t_data;

typedef struct s_philo
{
	int			id;
	size_t		last_eat;
	int			eat_count;
	pthread_t	thread;
	t_data		*data;
	int			has_left_fork;
	int			has_right_fork;
}	t_philo;

size_t	current_time(void);
void	print_action(t_philo *philo, char *message);
void	init_data(t_data *data, char **argv);
void	init_philos(t_philo *philos, t_data *data);
void	*philosopher(void *arg);
void	*observer_fn(void *arg);
int		checker(char **str, int nbr);
int		ft_strlen(char *str);
long	ft_atol(char *str);
int		not_enough_eat(t_philo *philo);
int		check_death(t_philo *philo);
void	smart_sleep(size_t duration, t_philo *philo);
void	put_down_forks(t_philo *philo);
void	destroy_mutex(t_data *data);
void	philo_take(t_philo *philo);
void	philo_take2(t_philo *philo);
void	think(t_philo *philo);
void	eat(t_philo *philo);
void	manage_threads(t_philo *philos, t_data *data);
void	take_forks(t_philo *philo);
int		is_someone_died(t_data *data);
void	set_someone_died(t_data *data);

#endif