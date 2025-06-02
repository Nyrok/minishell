/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:25:08 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/15 14:40:59 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t		len_s;
	size_t		len_sub;
	char		*result;
	size_t		i;

	if (!s)
		return (NULL);
	len_s = ft_strlen(s);
	if (start >= len_s)
		return (ft_strdup(""));
	len_sub = len_s - start;
	if (len_sub > len)
		len_sub = len;
	result = malloc((len_sub + 1) * sizeof(char));
	if (!result)
		return (NULL);
	i = 0;
	while (i < len_sub && s[start + i])
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
