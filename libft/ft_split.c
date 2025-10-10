/* ************************************************************************** */
/*																			*/
/*														:::	  ::::::::   */
/*   ft_split.c										 :+:	  :+:	:+:   */
/*													+:+ +:+		 +:+	 */
/*   By: hkonte <hkonte@student.42.fr>			  +#+  +:+	   +#+		*/
/*												+#+#+#+#+#+   +#+		   */
/*   Created: 2024/11/15 14:27:31 by hkonte			#+#	#+#			 */
/*   Updated: 2024/11/15 14:46:50 by hkonte		   ###   ########.fr	   */
/*																			*/
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(char const *s, char c)
{
	int	i;
	int	cmpt;

	i = 0;
	cmpt = 0;
	if (s[0] != c && s[0] != '\0')
		cmpt = 1;
	while (s[i] == c && s[i])
		i++;
	while (s[i])
	{
		if (s[i] != c && s[i] && i != 0)
			cmpt++;
		while (s[i] != c && s[i])
			i++;
		while (s[i] == c && s[i])
			i++;
	}
	return (cmpt);
}

char	*add_word(char const *s, int i, int k)
{
	char	*res;
	int		j;

	res = ft_calloc(k + 1, sizeof(char));
	if (!res)
		return (NULL);
	j = 0;
	while (k > 0)
	{
		res[j] = s[i];
		j++;
		i++;
		k--;
	}
	res[j] = '\0';
	return (res);
}

void	free_split(char **res, int j)
{
	while (j >= 0)
	{
		if (res[j])
			free(res[j]);
		res[j] = NULL;
		j--;
	}
	free(res);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	res = ft_calloc(sizeof(char *), ft_count(s, c) + 1);
	if (!res)
		return (NULL);
	while (s[i] && j < ft_count(s, c))
	{
		while (s[i] == c && s[i])
			i++;
		k = 0;
		while (s[i + k] != c && s[i + k])
			k++;
		res[j] = add_word(s, i, k);
		if (!res[j])
			return (free_split(res, j), NULL);
		i += k;
		j++;
	}
	res[j] = NULL;
	return (res);
}
