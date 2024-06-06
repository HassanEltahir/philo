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
        ft_usleep(200); // 200 milliseconds
    }
    if (philo->id % 2 == 0)
    {
        // usleep(200);
        pthread_mutex_lock(philo->r_fork);
        pthread_mutex_lock(philo->l_fork);
    }
    else
    {
        pthread_mutex_lock(philo->l_fork);
        pthread_mutex_lock(philo->r_fork);
    }

    // print_status(philo, "has taken a fork");
    print_status(philo, EATING);
    philo->last_meal = get_current_time();
    philo->meals_eaten++;
    pthread_mutex_unlock(philo->l_fork);
    pthread_mutex_unlock(philo->r_fork);
    ft_usleep(philo->time_to_eat);
}
    int dead_loop(t_philo *philos)
    {
        if (philos->num_times_to_eat == 0)
            return (1);
        if (philos->time_to_die < get_current_time() - philos->last_meal)
        {
            pthread_mutex_lock(philos->dead_lock);
            *philos->dead = 1;
            pthread_mutex_unlock(philos->dead_lock);
            print_status(philos, DEAD);
            printf("I am dead\n");
            return (1);
            exit(1);
        }
        return (0);
    }
void	*philo_routine(void *pointer)
{
    t_philo	*philo;

        philo = (t_philo *)pointer;
        while (!dead_loop(philo))
        {
            if(*philo->dead)
            {
                break;
            }
            pthread_mutex_lock(philo->meal_lock);
            if(philo->num_times_to_eat != -1 && philo->meals_eaten >= philo->num_times_to_eat)
            {
                pthread_mutex_unlock(philo->meal_lock);
                break;
            }
            pthread_mutex_unlock(philo->meal_lock);
            eat(philo);
            if(*philo->dead)
            {
                break;
            }
            sleeping(philo);
            thinking(philo);
            if(*philo->dead)
            {
                print_status(philo, DEAD);
                break;
            }
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
        i = 0;
            while(i < philos->num_of_philos)
            {
                if(*philos[i].dead == 1)
                {
                    return (0);
                }
                pthread_join(threads[i], NULL);
                i++;
                return (0);

            }
            i++;
        return (0);
    }