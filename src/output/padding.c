/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padding.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 18:20:32 by martiper          #+#    #+#             */
/*   Updated: 2024/03/28 00:19:31 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "padding.h"
#include <file.h>
#include <system.h>

static void init(t_l_fmt_padding* p) {
  ft_bzero(p, sizeof(t_l_fmt_padding));
}

void file_padding(t_file* file, t_l_fmt_padding* padding) {
  if (!file->statd && !file_stat(file))
    return;
  t_settings* settings = padding->settings;
  if (settings->display.inode)
    padding->inode = FT_MAX(padding->inode, ft_unbrlen_base(file->stat.st_ino, 10));
  if (settings->display.block_size)
    padding->block_size = FT_MAX(padding->block_size, ft_unbrlen_base(FS_BLOCK_SIZE(file->stat.st_blocks), 10));

  bool show_ids = settings->display.numeric_ids;
  bool compute_owner = settings->display.author || !settings->display.omit_owner;
  if (compute_owner)
    padding->owner_name = FT_MAX(padding->owner_name, file->owner && !show_ids ? ft_strlen(file->owner) : ft_unbrlen_base(file->stat.st_uid, 10));
  if (!settings->display.omit_group)
    padding->group_name = FT_MAX(padding->group_name, file->group && !show_ids ? ft_strlen(file->group) : ft_unbrlen_base(file->stat.st_gid, 10));
  // TODO: compute based on block-size
  padding->size = FT_MAX(padding->size, ft_unbrlen_base(file->stat.st_size, 10));
  padding->name = FT_MAX(padding->name, ft_strlen(file->name));
  padding->link = FT_MAX(padding->link, ft_unbrlen_base(file->stat.st_nlink, 10));
}

t_l_fmt_padding get_padding(t_list* files, t_settings* settings) {
  t_l_fmt_padding padding;
  init(&padding);
  padding.settings = settings;
  if (settings->format.type != FORMAT_LONG)
    return padding;
  ft_lstiter2(files, (t_lst_iter2)file_padding, &padding);
  return padding;
}

static void file_padding2_wrapper(t_file* file, size_t idx, t_l_fmt_padding* padding)
{
  (void)idx;
  file_padding(file, padding);
}

t_l_fmt_padding get_padding2(t_vector* files, t_settings* settings) {
  t_l_fmt_padding padding;
  init(&padding);
  padding.settings = settings;
  if (settings->format.type != FORMAT_LONG)
    return padding;
  files->foreach(files, (t_vector_foreach_f)file_padding2_wrapper, &padding);
  return padding;
}
