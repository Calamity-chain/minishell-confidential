/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmei <rmei@student.42berlin.de>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 11:44:22 by rmei              #+#    #+#             */
/*   Updated: 2024/05/10 11:50:12 by rmei             ###   ########.fr       */
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
