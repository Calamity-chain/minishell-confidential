/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltoscani <ltoscani@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 14:50:28 by ltoscani          #+#    #+#             */
/*   Updated: 2025/09/19 14:50:33 by ltoscani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* Converts to int the initial portion of a string. The string can begin with
 * an arbitrary amount of white spaces (isspace(3)), followed by a single,
 * optional sign('+' or '-'). Returns the converted value, or 0 on error. */
int	ft_atoi(const char *nptr)
{
	int	result;
	int	sign;
	int	sign_count;

	result = 0;
	sign = 1;
	sign_count = 0;
	while (*nptr == ' ' || (*nptr >= '\t' && *nptr <= '\r'))
		nptr++;
	while (*nptr == '+' || *nptr == '-')
	{
		if (++sign_count > 1)
			return (result);
		if (*nptr == '-')
			sign = -sign;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
		result = (result * 10) + (*nptr++ - '0');
	return (result * sign);
}
