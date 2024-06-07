#include "philo.h"
void destory_all(pthread_t *threads, t_program *program, pthread_mutex_t *forks)
{
    int i;

    i = 0;
    while (i < program->philos->num_of_philos)
    {
        pthread_mutex_destroy(&forks[i]);
        i++;
    }
    pthread_mutex_destroy(&program->dead_lock);
    pthread_mutex_destroy(&program->meal_lock);
    pthread_mutex_destroy(&program->write_lock);
    // free(threads);
    // free(program->philos);
}