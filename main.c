#include "philo.h"

void thread_join(t_program *program, t_philo *philos)
{
    int i;

    i = 0;
    while (i < philos[0].num_of_philos)
    {
        pthread_join(program->philos[i].thread, NULL);
        i++;
    }
}
int main(int ac,char **av)
{
        t_program         program;
        t_philo			philos[PHILO_MAX];
	    pthread_mutex_t	forks[PHILO_MAX];

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
            while(1)
            {
                if(check_if_dead(philos) == 1 || check_if_all_ate(philos) == 1)
                break;
            }    
            thread_join(&program, philos);
        }
        else
            printf(RED"Invalid number of arguments\n");
}