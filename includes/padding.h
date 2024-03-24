/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:14:33 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 19:40:42 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <libft.h>
#include <settings.h>

#define FT_MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct s_l_fmt_padding {
  size_t inode;
  size_t block_size;
  size_t owner_name;
  size_t group_name;
  size_t size;
  size_t name;
  size_t link;
  t_settings* settings;
} t_l_fmt_padding;

t_l_fmt_padding get_padding(t_list* files, t_settings* settings);
