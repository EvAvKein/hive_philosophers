/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ekeinan <ekeinan@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/14 10:26:06 by ekeinan           #+#    #+#             */
/*   Updated: 2025/03/14 12:44:08 by ekeinan          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLOR_H
# define COLOR_H

# define CLR_S "\e["
# define CLR_E "m"

# define CLR_FMT_STD		"0"
# define CLR_FMT_BOLD		"1"
# define CLR_FMT_DIM		"2"
# define CLR_FMT_ITALIC		"3"
# define CLR_FMT_UNDERLINE	"4"
# define CLR_FMT_FLASH		"5"
# define CLR_FMT_INVERT		"7"
# define CLR_FMT_INVIS		"8"

# define CLR_TXT_BLK		"30"
# define CLR_TXT_RED		"31"
# define CLR_TXT_GRN		"32"
# define CLR_TXT_BRN		"33"
# define CLR_TXT_BLU		"34"
# define CLR_TXT_PRP		"35"
# define CLR_TXT_CYN		"36"
# define CLR_TXT_WHT		"37"

# define CLR_RESET CLR_S CLR_FMT_STD ";" CLR_TXT_WHT  CLR_E

#endif