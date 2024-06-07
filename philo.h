#include <unistd.h>
#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <sys/time.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>
#include <limits.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <errno.h>
#include <pthread.h>

#define DEBUG_MODE 0 // or false, depending on your debug setting
#define GRN "\33[32m"
#define BLU "\33[34m"
#define MAG "\33[35m"
#define W   "\033[0;37m"
#define YEL "\033[0;33m"
#define RED "\033[0;31m"
#define RST "\033[0m"
#define PHILO_MAX 200 
typedef struct s_table t_table;
typedef struct s_philo philo;
typedef pthread_mutex_t t_mtx;
typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				eating;
	int				meals_eaten;
	size_t			last_meal;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	long			start_time;
	int				num_of_philos;
	int				num_times_to_eat;
	int				*dead;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*write_lock;
	pthread_mutex_t	*dead_lock;
	pthread_mutex_t	*meal_lock;
}					t_philo;

typedef enum s_status
{
	EATING,
	SLEEPING,
	THINKING,
	DEAD
} t_status;

typedef struct s_program
{
	int				dead_flag;
	pthread_mutex_t	dead_lock;
	pthread_mutex_t	meal_lock;
	pthread_mutex_t	write_lock;
	t_philo			*philos;
}					t_program;

void	parse_input(t_philo *philo, char **av);
void data_init(t_philo *philos);
int	ft_usleep(size_t milliseconds);
int thread_create(t_philo *philo);
void	init_forks(pthread_mutex_t *forks, int philo_num);
void	init_program(t_program *program, t_philo *philos);
void	init_philos(t_philo *philos, t_program *program, pthread_mutex_t *forks,
		char **argv);
long get_current_time(void);
// void	destory_all(NULL, &program, forks);