/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:18:40 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/15 14:40:32 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	n;

	n = ft_strlen(dest);
	i = 0;
	while (src[i] != '\0' && n + i + 1 < size)
	{
		dest[n + i] = src[i];
		i++;
	}
	dest[n + i] = '\0';
	if (n <= size)
		return (n + ft_strlen(src));
	return (size + ft_strlen(src));
}
