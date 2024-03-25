/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 18:01:44 by martiper          #+#    #+#             */
/*   Updated: 2024/03/25 21:21:06 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

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

t_list* get_files_from_dir(t_file* dir, t_settings* settings) {
  t_list* files = NULL;
  DIR* d = opendir(dir->input);
  if (!d) {
    ft_fprintf(2, "ft_ls: cannot open directory '%s': %s\n", dir->input, strerror(errno));
    return NULL;
  }
  struct dirent* entry;
  while ((entry = readdir(d))) {
    if (filter_file(entry, settings))
      continue;
    t_file* file = file_from_dir_entry(entry->d_name, dir->full_path, entry);
    if (!file)
      continue;
    ft_lstadd_back(&files, ft_lstnew(file));
  }
  closedir(d);
  return files;
}
