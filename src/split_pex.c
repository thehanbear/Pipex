#include "../pipex.h"

static int	check_quotes(const char *str, int i)
{
	int	k;
	int	double_quotes;
	int	single_quotes;
	int	escape;

	double_quotes = 0;
	single_quotes = 0;
	escape = 0;
	k = 0;
	while (k <= i)
	{
		if (str[k] == '\\' && !escape)
			escape = 1;
		else
		{
			if (str[k] == '"' && !escape && !single_quotes)
				double_quotes = !double_quotes;
			if (str[k] == '\'' && !escape && !double_quotes)
				single_quotes = !single_quotes;
			escape = 0;
		}
		k++;
	}
	return (!double_quotes && !single_quotes);
}

static int	is_sep(const char *str, int i, const char *c_set)
{
	int	j;

	j = 0;
	while (c_set[j])
	{
		if (str[i] == c_set[j])
			return (check_quotes(str, i));
		j++;
	}
	return (str[i] == '\0');
}

static char	*write_word(char *dest, const char *str, const char *c_set)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] && is_sep(str, i, c_set))
		i++;
	while (str[i] && !is_sep(str, i, c_set))
	{
		if (str[i] == '\\')
			i++;
		dest[j++] = str[i++];
	}
	dest[j] = '\0';
	return (dest);
}

static char	**write_arr(char **arr, const char *str, const char *c_set)
{
	int	len;
	int	i;
	int	word;

	len = 0;
	i = 0;
	word = 0;
	while (str[i])
	{
		if (is_sep(str, i, c_set))
			i++;
		else
		{
			while (!is_sep(str, i + len, c_set))
				len++;
			arr[word] = malloc(sizeof(char) * (len + 1));
			if (!arr[word])
				return (NULL);
			write_word(arr[word++], str + i, c_set);
			i += len;
			len = 0;
		}
	}
	return (arr);
}

char	**split_cmd(const char *str)
{
	char	**arr;
	int		word;
	int		i;

	word = 0;
	i = 0;
	while (str[i])
	{
		if (!is_sep(str, i, " \t") && is_sep(str, i + 1, " \t"))
			word++;
		i++;
	}
	arr = malloc((word + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	write_arr(arr, str, " \t");
	arr[word] = NULL;
	return (arr);
}
