#include "philo.h"
int main(int ac,char **av)
{
        t_program         program;
        t_philo			philos[PHILO_MAX];
	    pthread_mutex_t	forks[PHILO_MAX];

        if (ac == 5 || ac == 6)
        {
            parse_input(philos,av);
            init_program(&program,philos);
            init_forks(forks,philos->num_of_philos);
        	init_philos(philos, &program, forks, av);
    	    thread_create(philos);
        	// destory_all(NULL, &program, forks);
        }
        else
            printf(RED"Invalid number of arguments\n");
}