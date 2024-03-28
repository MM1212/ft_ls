/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grid.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 11:22:57 by martiper          #+#    #+#             */
/*   Updated: 2024/03/28 14:47:40 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"

t_grid  compute_grid(t_vector* files, t_l_fmt_padding* padding, t_settings* settings) {
  t_grid grid;

  ft_bzero(&grid, sizeof(t_grid));
  bool overflows = padding->total_line_width > settings->terminal_width - 1;
  if (settings->format.type == FORMAT_NONE)
    settings->format.type = FORMAT_VERTICAL;
  size_t max_files_per_row = (settings->terminal_width - 1) / (padding->grid_width + 2);
  grid.cols = !overflows ? files->size : max_files_per_row;
  if (files->size % grid.cols != 0 && settings->format.type == FORMAT_VERTICAL)
    grid.cols -= files->size % grid.cols - 1;

  if (grid.cols == 0)
    grid.cols = 1;

  grid.rows = files->size / grid.cols;
  if (files->size % grid.cols)
    grid.rows++;
  ft_printf("terminal_width: %u\n", settings->terminal_width);
  ft_printf("cell_width: %u\n", padding->grid_width);
  ft_printf("total_line_width: %u\n", padding->total_line_width);
  ft_printf("max_files_per_row: %u\n", max_files_per_row);
  ft_printf("files->size: %u\n", files->size);
  ft_printf("cols: %u\n", grid.cols);
  ft_printf("rows: %u\n", grid.rows);
  return grid;
}

void output_in_grid(t_vector* files, t_ft_ls* data) {
  t_grid grid = compute_grid(files, &data->padding, &data->settings);
  t_file* buff[grid.rows + 1][grid.cols + 1];

  ft_bzero(buff, sizeof(buff));
  grid.grid = (t_file***)buff;
  ft_printf("grid: rows: %d cols: %d per_cell: %d| sizeof: %u\n", grid.rows, grid.cols, data->padding.grid_width, sizeof(buff));
  for (size_t i = 0; i < files->size; i++) {
    size_t row = i / grid.cols;
    size_t col = i % grid.cols;
    if (data->settings.format.type == FORMAT_VERTICAL) {
      row = i % grid.rows;
      col = i / grid.rows;
    }
    t_file* file = *(t_file**)files->at(files, i);
    ft_printf("adding %s to grid[%d][%d]\n", file->name, row, col);
    buff[row][col] = file;
  }
}
