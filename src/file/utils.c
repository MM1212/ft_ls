/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:48:20 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 22:27:44 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file.h"
#include <stdint.h>
#include <libft.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <system.h>

bool file_is_dir(t_file* file) {
  return file->type == FILE_DIRECTORY || (file->type == FILE_SYMLINK && file->ltype == FILE_DIRECTORY);
}

static void add_file_block(t_file* file, uint32_t* total) {
  if (!file->statd)
    file_stat(file);
  *total += FS_BLOCK_SIZE(file->stat.st_blocks);
}

uint32_t get_total_blocks(t_list* files) {
  uint32_t total = 0;
  ft_lstiter2(files, (t_lst_iter2)add_file_block, &total);
  return total;
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
