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
        {
            usleep(500);
        }
        return (0);
    }

    void    print_status(t_philo *philo, t_status status)
    {
        pthread_mutex_lock(philo->write_lock);
        if (status == EATING)
            printf(GRN "%ld %d is eating\n" RST, get_current_time() - philo->start_time, philo->id);
        else if (status == SLEEPING)
            printf(BLU "%ld %d is sleeping\n" RST, get_current_time() - philo->start_time, philo->id);
        else if (status == THINKING)
            printf(MAG "%ld %d  is thinking\n" RST, get_current_time() - philo->start_time, philo->id);
        else if (status == DEAD)
            printf(RED"%ld %d is dead\n" RST, get_current_time() - philo->start_time, philo->id);
        pthread_mutex_unlock(philo->write_lock);
    }
    void thinking(t_philo *philo)
    {
        print_status(philo, THINKING);
    }
    void sleeping(t_philo *philo)
    {
        print_status(philo, SLEEPING);
        // pthread_mutex_unlock(philo->write_lock);
        ft_usleep(philo->time_to_sleep);
    }
void eat(t_philo *philo)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(philo->r_fork);
        pthread_mutex_lock(philo->l_fork);
    }
    else
    {
        pthread_mutex_lock(philo->l_fork);
        pthread_mutex_lock(philo->r_fork);
    }
    print_status(philo, EATING);
    philo->last_meal = get_current_time();
    philo->meals_eaten++;
    ft_usleep(philo->time_to_eat);
    pthread_mutex_unlock(philo->l_fork);
    pthread_mutex_unlock(philo->r_fork);
}
int dead_loop1(t_philo *philo)
{
    pthread_mutex_lock(philo->dead_lock);
    if (*philo->dead == 1)
    {
        pthread_mutex_unlock(philo->dead_lock);
        return (1);
    }
    pthread_mutex_unlock(philo->dead_lock);
    return (0);
}
void	*philo_routine(void *pointer)
{
    t_philo *philo;

    philo = (t_philo *)pointer;
    while (1)
    {
        eat(philo);
        sleeping(philo);
        thinking(philo);
    }
    return (NULL);
}
int	thread_create(t_program *program, pthread_mutex_t *forks)
{
	int			i;

	i = 0;

	while (i < program->philos[0].num_of_philos)
	{
		if (pthread_create(&program->philos[i].thread, NULL, &philo_routine,
				&program->philos[i]) != 0)
			destory_all("Thread creation error", program, forks);
		i++;
	}
	return (0);
}
