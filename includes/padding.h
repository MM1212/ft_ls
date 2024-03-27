/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:14:33 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:30:16 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <libft.h>
#include <settings.h>
#include <utils.h>
#include <lxt/dst/vector.h>

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
t_l_fmt_padding get_padding2(t_vector* files, t_settings* settings);
