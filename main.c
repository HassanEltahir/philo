#include "philo.h"

void thread_join(t_program *program, t_philo *philos)
{
    int i;

    i = 0;
    while (i < philos[0].num_of_philos)
    // while (i < philos[0].num_of_philos)
    {
        // printf("joining thread %d\n", i);
        pthread_join(program->philos[i].thread, NULL);
        i++;
    }
}

void *inspector(void *pointer)
{
    t_philo *philos;
    // t_program *program;

    philos = (t_philo *)pointer;
    
    while(1)
    {
        if(check_if_dead(philos) == 1 || check_if_all_ate(philos) == 1)
        {
            // printf("All philosophers have eaten enough meals or one of them has died\n");
            return (NULL);
        }
        usleep(100);
    }   


    return (NULL);
}


int main(int ac,char **av)
{
        t_program         program;
        t_philo			philos[PHILO_MAX];
	    pthread_mutex_t	forks[PHILO_MAX];
        pthread_t inspector_thread;

        if (ac == 5 || ac == 6)
        {
            if (atoi(av[1]) > 200)
            {
                return (1);
            }
            parse_input(philos,av);
            init_program(&program,philos);
            init_forks(forks,philos->num_of_philos);
        	init_philos(philos, &program, forks, av);
    	    thread_create(&program, forks);
            pthread_create(&inspector_thread, NULL, &inspector, &philos);

            pthread_join(inspector_thread, NULL);
            thread_join(&program, philos);
            destory_all( &program, forks);
            // }
        }
        else
            printf(RED"Invalid number of arguments\n");
}