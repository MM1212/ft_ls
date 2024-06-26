/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:48:20 by martiper          #+#    #+#             */
/*   Updated: 2024/03/29 17:04:50 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file.h"
#include <stdint.h>
#include <libft.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <ft_ls.h>

bool file_is_dir(t_file* file) {
  return file->type == FILE_DIRECTORY || (file->type == FILE_SYMLINK && file->ltype == FILE_DIRECTORY);
}

char* get_file_owner_name(t_file* file) {
  if (!file->statd && !file_stat(file))
    return NULL;
  struct passwd* pw = getpwuid(file->stat.st_uid);
  return pw ? ft_strdup(pw->pw_name) : NULL;
}
char* get_file_group_name(t_file* file) {
  if (!file->statd && !file_stat(file))
    return NULL;
  struct group* gr = getgrgid(file->stat.st_gid);
  return gr ? ft_strdup(gr->gr_name) : NULL;
}

char* resolve_path(size_t count, ...) {
  va_list args;
  va_start(args, count);
  char* path = NULL;
  for (size_t i = 0; i < count; i++) {
    char* part = va_arg(args, char*);
    if (!part || !*part)
      continue;

    if (!path) {
      path = ft_strdup(part);
      continue;
    }
    if (ft_str_startswith(part, "/")) {
      free(path);
      path = ft_strdup(part);
      continue;
    }
    char* pos;
    while ((pos = ft_strnstr(path, "/./", ft_strlen(path))))
      ft_strlcpy(pos, pos + 2, ft_strlen(pos) - 1);
    while ((pos = ft_strnstr(path, "//", ft_strlen(path))))
      ft_strlcpy(pos, pos + 1, ft_strlen(pos) - 1);
    if (path[ft_strlen(path) - 1] != '/' && *part != '/')
      path = ft_strjoin_free(path, "/");
    else if (path[ft_strlen(path) - 1] == '/' && *part == '/')
      part++;
    path = ft_strjoin_free(path, part);
    if (!path)
      break;
  }
  va_end(args);
  return path;
}

bool is_file_executable(t_file* file) {
  if (file->type != FILE_REGULAR)
    return false;
  if (!file_stat(file))
    return false;
  return file->stat.st_mode & S_IXUSR || file->stat.st_mode & S_IXGRP || file->stat.st_mode & S_IXOTH;
}

bool is_file_a_broken_link(t_file* file) {
  return (
    file->type == FILE_SYMLINK &&
    !file->symlinkd && file->symlink &&
    (g_ls_data->settings.filter.dereference_links || g_ls_data->settings.filter.dereference_links_cli)
  );
}

char* serialize_file_name(char* name) {
  for (uint32_t i = 0; name[i]; i++) {
    if (ft_isprint(name[i]))
      continue;
    name[i] = '?';
  }
  return name;
}
