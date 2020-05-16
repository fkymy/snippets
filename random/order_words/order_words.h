#ifndef ORDER_WORDS_H
# define ORDER_WORDS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

typedef struct	s_dict
{
	char		*str;
	int			len;
	int			order;
}				t_dict;

int		is_space(char c);
t_dict	*str_to_dict(char *str);
int		compare_dicts(t_dict s, t_dict t);

#endif
