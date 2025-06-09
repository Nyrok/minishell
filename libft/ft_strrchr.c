/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 11:39:26 by hkonte            #+#    #+#             */
/*   Updated: 2024/11/15 14:29:40 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *str, const char c)
{
	char			*occ;

	occ = NULL;
	while (*str)
	{
		if (*str == c)
			occ = (char *)str;
		str++;
	}
	if (c == '\0')
		return ((char *)str);
	return (occ);
}
