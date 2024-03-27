/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 18:01:44 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 00:12:35 by martiper         ###   ########.fr       */
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

t_vector* get_files_from_dir(t_file* dir, t_settings* settings) {
  t_vector* files = vector_create3(sizeof(t_file), 16, (t_vector_delete_f)file_cleanup);
  if (!files) {
    ft_show_error(g_ls_data, EXIT_FATAL, false, false, "cannot allocate memory");
    return NULL;
  }
  DIR* d = opendir(dir->full_path);
  if (!d) {
    ft_show_error(g_ls_data, EXIT_MINOR, false, true, "cannot open directory '%s'", dir->input);
    return NULL;
  }
  struct dirent* entry;
  while ((entry = readdir(d))) {
    if (filter_file(entry, settings))
      continue;
    t_file_type type = get_file_type_by_dirent(entry->d_type);
    t_file* file = files->emplace_back(files);
    if (!file) {
      ft_show_error(g_ls_data, EXIT_FATAL, false, false, "cannot allocate memory");
      break;
    }
    file_from_dir_entry(entry->d_name, dir->full_path, entry, file);
    if (type == FILE_SYMLINK && settings->filter.dereference_links) {
      t_file link;
      if (file_from_symlink(file, &link)) {
        file_cleanup(file);
        *file = link;
      }
    }
    if (!file)
      continue;
    file->parent = dir;
  }
  closedir(d);
  return files;
}

char* resolve_path(size_t count, ...) {
  va_list args;
  va_start(args, count);
  char* path = NULL;
  for (size_t i = 0; i < count; i++) {
    char* part = va_arg(args, char*);
    if (!part || !*part)
      continue;
    char* tmp = !path ? ft_strdup(part) : ft_strjoin_sep("/", path, part, NULL);
    free(path);
    path = tmp;
  }
  va_end(args);
  char** parts = ft_split(path, "/");
  if (!parts)
    return path;
  char* resolved = ft_strjoin_sep2("/", parts);
  ft_split_free(parts);
  if (!resolved) {
    free(path);
    return NULL;
  }
  if (ft_str_startswith(path, "/")) {
    char* tmp = ft_strjoin(2, "/", resolved);
    free(resolved);
    resolved = tmp;
  }
  if (ft_str_endswith(path, "/")) {
    char* tmp = ft_strjoin(2, resolved, "/");
    free(resolved);
    resolved = tmp;
  }
  free(path);
  return resolved;
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
