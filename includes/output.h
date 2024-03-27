/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 18:00:16 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:30:18 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ft_ls.h>
#include <file.h>
#include <lxt/dst/vector.h>

void file_print(t_file* file, t_ft_ls* data);
t_vector* get_files_from_dir(
  t_file* dir,
  t_settings* settings,
  void (*foreach_f)(t_file* file, void* data),
  void* data
);
uint32_t get_total_blocks(t_vector* files);
void print_indicator_style(t_file* file, t_settings* settings);
