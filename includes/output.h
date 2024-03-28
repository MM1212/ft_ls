/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 18:00:16 by martiper          #+#    #+#             */
/*   Updated: 2024/03/28 12:49:01 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ft_ls.h>
#include <file.h>
#include <lxt/dst/vector.h>

typedef struct {
  size_t rows;
  size_t cols;
  size_t max_col_len;
  size_t total_row_len;
  t_file*** grid;
} t_grid;

void file_print(t_file* file, t_ft_ls* data);
t_vector* get_files_from_dir(
  t_file* dir,
  t_settings* settings,
  void (*foreach_f)(t_file* file, void* data),
  void* data
);
uint32_t get_total_blocks(t_vector* files);
char get_indicator_style(t_file* file, t_settings* settings);
t_grid  compute_grid(t_vector* files, t_l_fmt_padding* padding, t_settings* settings);
void output_in_grid(t_vector* files, t_ft_ls* data);
