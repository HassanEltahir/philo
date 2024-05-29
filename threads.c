#include "philo.h"
long get_current_time(void)
{
    struct timeval  time;

    gettimeofday(&time, NULL);
    return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int ft_usleep(size_t milliseconds)
{
    size_t start_time;

    start_time = get_current_time();
    while (get_current_time() - start_time < milliseconds)
        usleep(500);
    return (0);
}

void    print_status(t_philo *philo, char *status)
{
    pthread_mutex_lock(philo->write_lock);
    printf( YEL"%ld %d %s\n", get_current_time() - philo->start_time, philo->id, status);
    pthread_mutex_unlock(philo->write_lock);
}
void thinking(t_philo *philo)
{
    print_status(philo, "is thinking");
}
void sleeping(t_philo *philo)
{
    print_status(philo, "is sleeping");
    ft_usleep(philo->time_to_sleep);
}
void eat(t_philo *philo)
{
   if (philo->num_of_philos % 2)
	{
		pthread_mutex_lock(philo->r_fork);
		pthread_mutex_lock(philo->l_fork);
	}
	else
	{
		pthread_mutex_lock(philo->l_fork);
		pthread_mutex_lock(philo->r_fork);
	}
    print_status(philo, "has taken the left fork");
    print_status(philo, "has taken the right fork");
    print_status(philo, "is eating");
    philo->last_meal = get_current_time();
    ft_usleep(philo->time_to_eat);
    pthread_mutex_unlock(philo->l_fork);
    pthread_mutex_unlock(philo->r_fork);
    philo->meals_eaten++;
    if (philo->num_times_to_eat != -1 && philo->meals_eaten == philo->num_times_to_eat)
        philo->num_times_to_eat = 0;
}
int dead_loop(t_philo *philo)
{
    if (philo->num_times_to_eat == 0)
        return (1);
    if (philo->time_to_die < get_current_time() - philo->last_meal)
    {
        pthread_mutex_lock(philo->dead_lock);
        *philo->dead = 1;
        pthread_mutex_unlock(philo->dead_lock);
        print_status(philo, "died");
        return (1);
    }
    return (0);
}
void	*philo_routine(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
	while (1)
	{
		eat(philo);
        if(dead_loop(philo))
            break;
		sleeping(philo);
        if(dead_loop(philo))
            break;
		thinking(philo);
            break;
	}
	return (NULL);
}
int thread_create(t_philo *philos)
{
    pthread_t   *threads;
    int         i;

    i = 0;
    while (i < philos->num_of_philos)
    {
        pthread_create(&threads[i], NULL, &philo_routine, &philos[i]);
        i++;
    }
    while(!dead_loop(philos))
    {
        if(dead_loop(philos))
            break;
    }
    while (i < philos->num_of_philos)
    {
        pthread_join(threads[i], NULL);
        i++;
    }
    return (0);
}