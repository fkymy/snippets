#include "order_words.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct	s_dict
{
	char		*str;
	int			len;
	int			order;
}				t_dict;

// map upper and lower case letters together for case independent cmp
typedef unsigned char u_char;
static const u_char charmap[] = {
	'\000', '\001', '\002', '\003', '\004', '\005', '\006', '\007',
	'\010', '\011', '\012', '\013', '\014', '\015', '\016', '\017',
	'\020', '\021', '\022', '\023', '\024', '\025', '\026', '\027',
	'\030', '\031', '\032', '\033', '\034', '\035', '\036', '\037',
	'\040', '\041', '\042', '\043', '\044', '\045', '\046', '\047',
	'\050', '\051', '\052', '\053', '\054', '\055', '\056', '\057',
	'\060', '\061', '\062', '\063', '\064', '\065', '\066', '\067',
	'\070', '\071', '\072', '\073', '\074', '\075', '\076', '\077',
	'\100', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
	'\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
	'\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
	'\170', '\171', '\172', '\133', '\134', '\135', '\136', '\137',
	'\140', '\141', '\142', '\143', '\144', '\145', '\146', '\147',
	'\150', '\151', '\152', '\153', '\154', '\155', '\156', '\157',
	'\160', '\161', '\162', '\163', '\164', '\165', '\166', '\167',
	'\170', '\171', '\172', '\173', '\174', '\175', '\176', '\177',
	'\200', '\201', '\202', '\203', '\204', '\205', '\206', '\207',
	'\210', '\211', '\212', '\213', '\214', '\215', '\216', '\217',
	'\220', '\221', '\222', '\223', '\224', '\225', '\226', '\227',
	'\230', '\231', '\232', '\233', '\234', '\235', '\236', '\237',
	'\240', '\241', '\242', '\243', '\244', '\245', '\246', '\247',
	'\250', '\251', '\252', '\253', '\254', '\255', '\256', '\257',
	'\260', '\261', '\262', '\263', '\264', '\265', '\266', '\267',
	'\270', '\271', '\272', '\273', '\274', '\275', '\276', '\277',
	'\300', '\301', '\302', '\303', '\304', '\305', '\306', '\307',
	'\310', '\311', '\312', '\313', '\314', '\315', '\316', '\317',
	'\320', '\321', '\322', '\323', '\324', '\325', '\326', '\327',
	'\330', '\331', '\332', '\333', '\334', '\335', '\336', '\337',
	'\340', '\341', '\342', '\343', '\344', '\345', '\346', '\347',
	'\350', '\351', '\352', '\353', '\354', '\355', '\356', '\357',
	'\360', '\361', '\362', '\363', '\364', '\365', '\366', '\367',
	'\370', '\371', '\372', '\373', '\374', '\375', '\376', '\377',
};

int		stricmp(const char *s1, const char *s2)
{
	const u_char *us1 = (const u_char *)s1;
	const u_char *us2 = (const u_char *)s2;

	while (charmap[*us1] == charmap[*us2++])
		if (*us1++ == '\0')
			return (0);
	return (charmap[*us1] - charmap[*--us2]);
}

int		is_space(char c) {
	return ((c == ' ' || c == '\n' || c == '\t') ? 1 : 0);
}

int		compare_dicts(t_dict s, t_dict t)
{
	if (s.len != t.len)
		return (s.len - t.len);
	if (stricmp(s.str, t.str) != 0)
		return (stricmp(s.str, t.str));
	return (s.order - t.order);
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

void	swap_dicts(t_dict *d1, t_dict *d2)
{
	t_dict temp;

	temp = *d1;
	*d1 = *d2;
	*d2 = temp;
}

void	order_words(t_dict *dicts, int(*cmp)(t_dict, t_dict))
{
	int i;
	int j;
	int size;

	size = 0;
	while (dicts[size].str)
		size++;

	i = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - 1 - i)
		{
			if ((*cmp)(dicts[j], dicts[j + 1]) > 0)
				swap_dicts(&dicts[j], &dicts[j + 1]);
			j++;
		}
		i++;
	}
}

void	print_dicts(t_dict *dicts)
{
	int i;
	int len;

	i = 0;
	len = 0;
	while (dicts[i].str)
	{
		if (dicts[i].len > len)
		{
			len = dicts[i].len;
			if (i != 0)
				write(1, "\n", 1);
		}
		write(1, dicts[i].str, dicts[i].len);
		if (dicts[i + 1].str && dicts[i + 1].len == dicts[i].len)
			write(1, " ", 1);
		i++;
	}
}

void	free_dicts(t_dict *dicts)
{
	int i;

	i = 0;
	while (dicts[i].str)
		free(dicts[i++].str);
	free(dicts);
}

int		main(int argc, char *argv[])
{
	t_dict	*dicts;

	if (argc == 2)
	{
		dicts = str_to_dict(argv[1]);
		order_words(dicts, compare_dicts);
		print_dicts(dicts);
		free_dicts(dicts);
	}
	write(1, "\n", 1);
	return (0);
}
