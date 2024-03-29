/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:14:33 by martiper          #+#    #+#             */
/*   Updated: 2024/03/29 17:40:19 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <libft.h>
#include <settings.h>
#include <utils.h>
#include <lxt/dst/vector.h>

typedef struct s_l_fmt_padding {
  size_t total_line_width;
  size_t grid_width;
  size_t inode;
  size_t block_size;
  size_t permissions;
  size_t owner_name;
  size_t group_name;
  size_t scontext;
  size_t size;
  size_t name;
  size_t link;
  size_t date;
  t_settings* settings;
} t_l_fmt_padding;

t_l_fmt_padding get_padding(t_list* files, t_settings* settings);
t_l_fmt_padding get_padding2(t_vector* files, t_settings* settings);
void file_padding(t_file* file, t_l_fmt_padding* padding);
size_t get_file_name_and_extras_length(t_file* file, t_l_fmt_padding* padding, t_settings* settings);
