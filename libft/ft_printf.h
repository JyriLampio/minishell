/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alogvine <alogvine@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 13:54:09 by alogvine          #+#    #+#             */
/*   Updated: 2024/01/21 10:03:26 by alogvine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <stdarg.h>
# include <unistd.h>

int	ft_printf(const char *s, ...);
int	ft_putchar(int s, int *check);
int	ft_putstr(char *s, int *check);
int	ft_putnbr(int i, int *check);
int	ft_unsputnbr(unsigned int i, int *check);
int	ft_hex(unsigned long i, int upper, int *check);
int	ft_pointer(void *ptr, int *check);
int	ft_check(const char *s, va_list args, int *check);

#endif
