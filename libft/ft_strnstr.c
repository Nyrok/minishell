/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:25:08 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/15 14:40:59 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

char	*ft_strnstr(const char *hay, const char *ndle, size_t len)
{
	size_t	i;
	size_t	k;
	size_t	n;

	n = ft_strlen(ndle);
	i = 0;
	if (n == 0)
		return ((char *)hay);
	if (hay[i] != '\0' && len < n)
		return (0);
	while (hay[i] != '\0' && i < len)
	{
		k = 0;
		while (i + k < len && hay[i + k] && ndle[k] == hay[i + k])
			k++;
		if (ndle[k - 1] == hay[i + k - 1] && k == n)
			return ((char *)&hay[i]);
		i++;
	}
	return (0);
}
