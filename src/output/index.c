/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:31:13 by martiper          #+#    #+#             */
/*   Updated: 2024/03/29 17:39:03 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>
#include "output.h"
#include <sort.h>
#include <system.h>

static bool display_directory(bool pos[2], char* pre_parents, t_file* dir, t_ft_ls* data);

static bool display_dir_recursive(bool pos[2], char* parent, t_file* file, t_ft_ls* data) {
  if (file->type != FILE_DIRECTORY)
    return true;
  return display_directory(pos, parent, file, data);
}

void output_files(t_vector* sorted, t_ft_ls* data) {
  switch (data->settings.format.type) {
  case FORMAT_SINGLE_COLUMN:
  case FORMAT_LONG:
    for (uint32_t i = 0; i < sorted->size; i++) {
      t_file** file = sorted->at(sorted, i);
      file_print(*file, data);
      ft_putchar_fd(EOL_BYTE, 1);
    }
    break;
  case FORMAT_VERTICAL:
  case FORMAT_HORIZONTAL:
    if (!data->settings.terminal_width)
      print_files_with_separator(sorted, ' ', data);
    else
      output_in_grid(sorted, data);
    break;
  case FORMAT_COMMAS:
    print_files_with_separator(sorted, ',', data);
    break;
  case FORMAT_NONE:
    break;
  }
}

static void display_files(t_list* files, t_ft_ls* data) {
  data->padding = get_padding(files, &data->settings);
  t_vector* sorted = vector_create3(sizeof(t_file*), ft_lstsize(files), NULL);
  if (!sorted)
    ft_show_error(EXIT_FATAL, true, false, "cannot allocate memory");
  t_list* iter = files;
  size_t idx = 0;
  while (iter) {
    *((t_file**)sorted->at(sorted, idx++)) = iter->content;
    iter = iter->next;
  }
  output_files(sorted, data);
  sorted->destroy(sorted);
  ft_printf("\n");
}

struct get_files_cache {
  bool first;
  uint32_t blocks_size;
  t_ft_ls* data;
  t_l_fmt_padding padding;
};

static void on_each_file(t_file* file, struct get_files_cache* cache) {
  if ((cache->data->settings.format.type == FORMAT_LONG || cache->data->settings.display.block_size) && file_stat(file))
    cache->blocks_size += FS_BLOCK_SIZE(file->stat.st_blocks);
  file_padding(file, &cache->padding);
  if (cache->data->settings.format.type != FORMAT_LONG && cache->data->settings.format.type != FORMAT_SINGLE_COLUMN) {
    if (!cache->first) {
      cache->padding.total_line_width += 2;
    }
  }
  cache->first = false;
}

static bool display_directory(bool pos[2], char* pre_parents, t_file* dir, t_ft_ls* data) {
  bool first = pos[0];
  bool is_in_dir_cache = data->dir_cache->get(data->dir_cache, dir);
  if (is_in_dir_cache) {
    ft_show_error(EXIT_MINOR, false, false, "%s not listing already-listed directory", dir->display_path);
    return false;
  }
  (void)first;
  struct get_files_cache cache = { true, 0, data, {0} };
  ft_bzero(&cache.padding, sizeof(t_l_fmt_padding));
  cache.padding.settings = &data->settings;
  t_vector* files = get_files_from_dir(dir, &data->settings, (void*)on_each_file, &cache);
  if (!files)
    return false;
  if (!first)
    ft_printf("\n");
  if (data->settings.print_dir_name) {
    ft_printf("%s:\n", dir->display_path);
  }
  data->padding = cache.padding;
  if (data->settings.format.type == FORMAT_LONG || data->settings.display.block_size)
    ft_printf("total %u\n", cache.blocks_size);
  if (!files->size) {
    files->destroy(files);
    return true;
  }
  t_vector* sorted = sort_files2(files, &data->settings);
  output_files(sorted, data);
  if (data->settings.filter.recursive) {
    data->dir_cache->add(data->dir_cache, dir, dir);
    pre_parents = resolve_path(2, pre_parents, dir->display_path);
    if (!pre_parents) {
      files->destroy(files);
      if (sorted != files)
        sorted->destroy(sorted);
      ft_show_error(EXIT_FATAL, true, false, "cannot allocate memory");
    }
    for (uint32_t i = 0; i < sorted->size; i++) {
      t_file** file = sorted->at(sorted, i);
      display_dir_recursive((bool[2]) { false, false }, pre_parents, * file, data);
    }
    free(pre_parents);
    data->dir_cache->remove(data->dir_cache, dir);
  }
  if (sorted != files)
    sorted->destroy(sorted);
  files->destroy(files);
  return true;
}

void ft_ls_run(t_ft_ls* data) {
  if (data->file_entries)
    display_files(data->file_entries, data);
  if (!data->dir_entries)
    return;
  if (data->file_entries)
    ft_printf("\n");
  t_list* iter = data->dir_entries;
  bool skip_nl = false;
  while (iter) {
    t_file* dir = iter->content;
    bool ok = display_directory((bool[2]) { !iter->prev || skip_nl, !iter->next }, "", dir, data);
    skip_nl = !ok && !iter->prev;
    iter = iter->next;
  }
}
