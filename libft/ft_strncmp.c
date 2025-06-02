/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 14:07:59 by hkonte            #+#    #+#             */
/*   Updated: 2024/07/11 14:50:47 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t	index;

	index = 0;
	if (n == 0)
		return (0);
	while (s1[index] == s2[index] && s1[index] != '\0' && s2[index] != '\0')
	{
		if (++index >= n)
		{
			index--;
			return ((unsigned char)s1[index] - (unsigned char)s2[index]);
		}
	}
	return ((unsigned char)s1[index] - (unsigned char)s2[index]);
}
