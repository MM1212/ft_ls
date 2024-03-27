/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 18:01:44 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 15:58:26 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <system.h>

static bool filter_file(struct dirent* entry, t_settings* settings) {
  bool is_hidden = entry->d_name[0] == '.';
  // -a, -A
  if (!settings->filter.all && is_hidden) {
    bool is_dot = ft_strcmp(entry->d_name, ".") == 0 || ft_strcmp(entry->d_name, "..") == 0;
    if (!settings->filter.almost_all || is_dot)
      return true;
  }
  // -I
  if (settings->filter.ignore_pattern && ft_wildcard_match(settings->filter.ignore_pattern, entry->d_name))
    return true;
  // -B
  if (settings->filter.ignore_backups && ft_str_endswith(entry->d_name, "~"))
    return true;
  return false;
}

t_vector* get_files_from_dir(\
  t_file* dir, \
  t_settings* settings, \
  void (*foreach_f)(t_file* file, void* data),
  void* data
) {
  t_vector* files = vector_create3(sizeof(t_file), 16, (t_vector_delete_f)file_cleanup);
  if (!files) {
    ft_show_error(EXIT_FATAL, false, false, "cannot allocate memory");
    return NULL;
  }
  DIR* d = opendir(dir->full_path);
  if (!d) {
    ft_show_error(EXIT_MINOR, false, true, "cannot open directory '%s'", dir->display_path);
    return NULL;
  }
  struct dirent* entry;
  while ((entry = readdir(d))) {
    if (filter_file(entry, settings))
      continue;
    // t_file_type type = get_file_type_by_dirent(entry->d_type);
    t_file* file = files->emplace_back(files);
    if (!file) {
      ft_show_error(EXIT_FATAL, false, false, "cannot allocate memory");
      break;
    }
    file_from_dir_entry(dir, entry, file);
    if (!file)
      continue;
    file->parent = dir;
    if (foreach_f)
      foreach_f(file, data);
  }
  closedir(d);
  return files;
}

static void add_file_block(t_file* file, uint32_t* total) {
  if (!file->statd && !file_stat(file))
    return;
  *total += FS_BLOCK_SIZE(file->stat.st_blocks);
}

uint32_t get_total_blocks(t_list* files) {
  uint32_t total = 0;
  ft_lstiter2(files, (t_lst_iter2)add_file_block, &total);
  return total;
}

static void add_file_block2(t_file* file, size_t idx, uint32_t* total) {
  (void)idx;
  if (!file->statd && !file_stat(file))
    return;
  *total += FS_BLOCK_SIZE(file->stat.st_blocks);
}

uint32_t get_total_blocks2(t_vector* files) {
  uint32_t total = 0;
  files->foreach(files, (t_vector_foreach_f)add_file_block2, &total);
  return total;
}
