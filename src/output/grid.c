/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grid.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:22:57 by martiper          #+#    #+#             */
/*   Updated: 2024/03/29 14:10:22 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"

static t_grid  compute_grid(t_vector* files, t_l_fmt_padding* padding, t_settings* settings) {
  t_grid grid;
  ft_bzero(&grid, sizeof(t_grid));
  grid.max_cols = files->size;
  // try to fit as many columns as possible
  size_t line_length = 0;
  for (uint32_t i = 0; i < files->size; i++) {
    t_file* file = *(t_file**)files->at(files, i);
    line_length += get_file_name_and_extras_length(file, padding, settings);
    if (i < files->size - 1)
      line_length += 2;
    if (line_length > (size_t)settings->terminal_width - 1) {
      grid.cols = i;
      break;
    }
  }
  if (!grid.cols)
    grid.cols = files->size;
  grid.rows = files->size / grid.cols + (files->size % grid.cols != 0);
  return grid;
}

static void indent(size_t from, size_t to)
{
  ft_printf("%*s", (int)(to - from), "");
}

static void output_grid(t_vector* files, t_grid grid, t_ft_ls* data, size_t* max_cols_len) {
  for (size_t row = 0; row < grid.rows; row++) {
    for (size_t col = 0; col < grid.cols; col++) {
      size_t idx = row * grid.cols + col;
      if (data->settings.format.type == FORMAT_VERTICAL)
        idx = col * grid.rows + row;
      if (idx >= files->size)
        break;
      t_file* file = *(t_file**)files->at(files, idx);
      file_print(file, data);
      size_t len = get_file_name_and_extras_length(file, &data->padding, &data->settings);
      indent(len, max_cols_len[col] + (2 * (col < grid.cols - 1)));
    }
    ft_printf("\n");
  }
}

void output_in_grid(t_vector* files, t_ft_ls* data) {
  t_grid grid = compute_grid(files, &data->padding, &data->settings);
  size_t max_cols_len[grid.max_cols]; // max len per column

  size_t max_width = (size_t)data->settings.terminal_width - 1;

  while (true) {
    grid.rows = files->size / grid.cols + (files->size % grid.cols != 0);
    for (size_t col = 0; col < grid.cols; col++)
    {
      max_cols_len[col] = 0;
      for (size_t row = 0; row < grid.rows; row++)
      {
        size_t idx = row * grid.cols + col;
        if (data->settings.format.type == FORMAT_VERTICAL)
          idx = col * grid.rows + row;
        if (idx >= files->size)
          break;
        t_file* file = *(t_file**)files->at(files, idx);
        size_t len = get_file_name_and_extras_length(file, &data->padding, &data->settings);
        max_cols_len[col] = FT_MAX(max_cols_len[col], len);
      }
    }
    size_t line_len = 0;
    for (size_t col = 0; col < grid.cols; col++)
      line_len += max_cols_len[col] + 2 * (col < grid.cols - 1);
    if (line_len > max_width && grid.cols > 1)
      grid.cols--;
    else
      break;
  }
  grid.rows = files->size / grid.cols + (files->size % grid.cols != 0);
  output_grid(files, grid, data, max_cols_len);
}
