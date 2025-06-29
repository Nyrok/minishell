/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hkonte <hkonte@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:51:55 by hkonte            #+#    #+#             */
/*   Updated: 2025/06/09 16:52:27 by hkonte           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*rm_quotes(const char *str, size_t start, size_t n)
{
	size_t	i;
	size_t	j;
	size_t	size;
	int		quotes;
	char	*result;

	quotes = 0;
	i = -1;
	while (++i < n)
		if (str[start + i] == '\'' || str[start + i] == '"')
			quotes++;
	size = n - quotes;
	result = malloc((size + 1) * sizeof(char));
	if (!result)
		return (NULL);
	i = -1;
	j = 0;
	while (++i < n)
		if (str[start + i] != '\'' && str[start + i] != '"')
			result[j++] = str[start + i];
	result[i] = '\0';
	return (result);
}
