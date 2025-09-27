/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 13:13:55 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/29 13:14:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_intsize(int n)
{
	long int	val;
	int			count;
	int			is_negative;

	count = 0;
	is_negative = n < 0;
	if (is_negative)
		val = -(long int)n;
	else
		val = (long int)n;
	while (val > 0)
	{
		val /= 10;
		count++;
	}
	return (1 + (n == 0) + count + is_negative);
}

static char	*ft_strrev(char *str)
{
	int		i;
	int		n;
	char	temp;

	if (!str)
		return (NULL);
	i = 0;
	n = ft_strlen(str) - 1;
	while (i < n)
	{
		temp = str[i];
		str[i] = str[n];
		str[n] = temp;
		i++;
		n--;
	}
	return (str);
}

char	*ft_itoa(int n)
{
	long int	val;
	char		*result;
	int			is_negative;
	int			i;

	is_negative = n < 0;
	if (is_negative)
		val = -(long int)n;
	else
		val = (long int)n;
	result = ft_calloc(ft_intsize(n), sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	if (val == 0)
		result[i++] = '0';
	while (val > 0)
	{
		result[i++] = '0' + (val % 10);
		val /= 10;
	}
	if (is_negative)
		result[i++] = '-';
	result[i] = '\0';
	return (ft_strrev(result));
}
