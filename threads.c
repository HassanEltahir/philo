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
    printf( RED"%ld %d %s\n", get_current_time() - philo->start_time, philo->id, status);
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
bool forks(t_philo *philo)
{
    pthread_mutex_lock(philo->left_fork);
    if (!philo->l_fork)
    {
        pthread_mutex_unlock(philo->left_fork);
        return false;
    }

    pthread_mutex_lock(philo->right_fork);
    if (!philo->r_fork)
    {
        pthread_mutex_unlock(philo->right_fork);
        pthread_mutex_unlock(philo->left_fork);
        return false;
    }

    // Both forks are available
    philo->l_fork = 0;
    philo->r_fork = 0;
    return true;
}

void drop_forks(t_philo *philo)
{
    philo->l_fork = 1;
    pthread_mutex_unlock(philo->left_fork);
    philo->r_fork = 1;
    pthread_mutex_unlock(philo->right_fork);
}

void eat(t_philo *philo)
{
        if(philo->l_fork == 1 && philo->r_fork == 1)
        {
            print_status(philo, "is eating");
        }
        philo->last_meal = get_current_time();
        philo->meals_eaten++;
}
void	*philo_routine(void *pointer)
{
	t_philo	*philo;

	philo = (t_philo *)pointer;
    while (1)
	{
        if(*philo->dead)
        {
            break;
        }
        // forks(philo);
        if(forks(philo))
        {
            eat(philo);
            drop_forks(philo);
            sleeping(philo);
            thinking(philo);
        }   
        pthread_mutex_lock(philo->meal_lock);
        if(philo->num_times_to_eat != -1 && philo->meals_eaten >= philo->num_times_to_eat)
        {
            pthread_mutex_unlock(philo->meal_lock);
            break;
        }
        pthread_mutex_unlock(philo->meal_lock);
	}
	return (NULL);
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
        printf("died");
        return (1);
    }
    return (0);
}
int thread_create(t_philo *philos)
{
    pthread_t   *threads;
    int         i;
    i = 0;

    threads = malloc(sizeof(pthread_t) * philos->num_of_philos + 1);
    if (!threads)
        return (1);
        
    while (i < philos->num_of_philos)
    {
        pthread_create(&threads[i], NULL, &philo_routine, &philos[i]);
        i++;
    }
    i = 0;
    while(dead_loop(philos) == 0)
    {
        pthread_join(threads[i], NULL);
        i++;
    }
    return (0);
}