#include "philo.h"

static inline bool	is_space(char c)
{
	return ((c >= 9 && c <= 13) || 32 == c);
}

static inline bool	is_digit(char c)
{
	return (c >= '0' && c <= '9');
}
static const char	*valid_input(const char *str)
{
	int			len;
	const char	*number;

	len = 0;
	while (is_space(*str))
		++str;
	if (*str == '+')
		++str;
	else if (*str == '-')
		printf("%s","Feed me only positive values");
	if (!is_digit(*str))
		printf("%s","Feed me only numbers");
	number = str;
	while (is_digit(*str++))
		++len;
	if (len > 10)
		printf("%s","Number too big");
	return (number);
}
static long	ft_atol(const char *str)
{
	long	num;

	num = 0;
	str = valid_input(str);
	while (is_digit(*str))
		num = (num * 10) + (*str++ - '0');
	if (num > INT_MAX)
		printf("%s","INT_MAX is the limit, not the sky");
	return (num);
}
void	parse_input(t_philo *philo, char **av)
{
	philo->num_of_philos = ft_atol(av[1]);
	if (philo->num_of_philos > PHILO_MAX)
	{
		printf(RED"Max philos are %d\n",
			PHILO_MAX);
		return ;
	}
	philo->time_to_die = ft_atol(av[2]) * 1e3;
	philo->time_to_eat = ft_atol(av[3]) * 1e3;
	philo->time_to_sleep = ft_atol(av[4]) * 1e3;
	if (philo->time_to_die < 6e4
		|| philo->time_to_sleep < 6e4
		|| philo->time_to_eat < 6e4)
		{
            printf("Use timestamps greater than 60ms\n");
            return ;
        }
	if (av[5])
		philo->num_times_to_eat = ft_atol(av[5]);
	else
		philo->num_times_to_eat = -1;
}