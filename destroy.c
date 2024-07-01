#include "philo.h"
void destory_all( t_program *program, pthread_mutex_t *forks)
{
    int	i;

    i = 0;
    // if (str)
    //     printf(RED"%s\n", str);
    while (i < program->philos[0].num_of_philos)
    {
        pthread_mutex_destroy(&forks[i]);
        i++;
    }
    // pthread_mutex_unlock(&program->dead_lock);
    // pthread_mutex_unlock(&program->meal_lock);
    // pthread_mutex_unlock(&program->write_lock);
    pthread_mutex_destroy(&program->dead_lock);
    pthread_mutex_destroy(&program->meal_lock);
    pthread_mutex_destroy(&program->write_lock);
    // free(program->philos);
    // exit(1);
}