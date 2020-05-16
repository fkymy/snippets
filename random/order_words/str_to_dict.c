#include "order_words.h"

int		is_space(char c) {
	return ((c == ' ' || c == '\n' || c == '\t') ? 1 : 0);
}

int		word_count(char *str)
{
	int len;

	len = 0;
	while (*str)
	{
		if (is_space(*str))
			str++;
		else
		{
			len++;
			while (*str && !is_space(*str))
				str++;
		}
	}

	return (len);
}

char	**str_split(char *str)
{
	char **res;
	int res_size;
	int w_len;
	int i = 0;
	int j = 0;
	int k;

	res_size = word_count(str);
	w_len = 0;
	if (!(res = (char **)malloc(sizeof(char *) * (res_size + 1))))
		return (NULL);
	while (str[i] && j < res_size)
	{
		while (str[i] && is_space(str[i]))
			i++;
		while (str[i] && !is_space(str[i]))
		{
			w_len++;
			i++;
		}
		if (!(res[j] = (char *)malloc(sizeof(char) * (w_len + 1))))
			return (NULL);
		k = 0;
		while (w_len)
			res[j][k++] = str[i - w_len--];
		res[j++][k] = '\0';
	}
	res[j] = NULL;
	return (res);
}

int		strs_size(char **strs)
{
	int i;

	i = 0;
	while (strs[i])
		i++;
	return (i);
}

t_dict	*str_to_dict(char *str)
{
	t_dict	*dicts;
	char	**strs;
	int		order = 1;
	int		i;
	int		j;

	strs = str_split(str);
	if (!(dicts = (t_dict *)malloc(sizeof(t_dict) * (strs_size(strs) + 1))))
		return (NULL);
	i = 0;
	j = 0;
	while (strs[i])
	{
		dicts[j].str = strdup(strs[i]);
		dicts[j].len = strlen(strs[i]);
		dicts[j].order = order++;
		i++;
		j++;
	}
	dicts[j].str = NULL;
	while (strs[i])
		free(strs[i]);
	free(strs);
	return (dicts);
}
