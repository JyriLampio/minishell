#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	ft_strcpy(char *dst, const char *src)
{
	while (*src)	
		*dst++ = *src++;
	*dst = '\0';
}

char	*ft_strjoin(char *s1, char *s2)
{
	char *join;

	if (!s1 || !s2)
		return (NULL);
	join = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!join)
		return (NULL);
	ft_strcpy(join, s1);
	ft_strcpy(join + ft_strlen(s1), s2);
	return (join);
}

int	main(int ac, char **av, char **envp)
{
	(void)av;
	(void)envp;
	char	*line;
	char	*str;

	if (ac == 1)
	{
		while (1)
		{
			str = malloc(sizeof(char*) + 1);
//			str = getcwd(0, 0);
			str = ft_strjoin("minishell> ", str);
//			str = ft_strjoin(str, " $ ");
			line = readline(str);
		}
	}
	return (0);
}
