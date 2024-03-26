/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:48:20 by martiper          #+#    #+#             */
/*   Updated: 2024/03/26 20:45:37 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file.h"
#include <stdint.h>
#include <libft.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>

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
