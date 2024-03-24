/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 19:42:32 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 22:27:37 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include <ft_ls.h>
#include <system.h>

void file_print(t_file* file, t_ft_ls* data) {
  t_l_fmt_padding* padding = &data->padding;
  if (!data->first_batch_print) {
    switch (data->settings.format.type) {
    case FORMAT_LONG:
    case FORMAT_SINGLE_COLUMN:
      ft_printf("\n");
      break;
    case FORMAT_COMMAS:
      ft_printf(", ");
      break;
    case FORMAT_HORIZONTAL:
      ft_printf("  ");
      break;
    default:
      break;
    }
  }
  if (data->settings.display.inode)
    ft_printf("%-*u ", padding->inode, file->id);
  if (data->settings.display.block_size && file_stat(file))
    ft_printf("%*u ", padding->block_size, FS_BLOCK_SIZE(file->stat.st_blocks));
  if (data->settings.format.type == FORMAT_LONG) {
    // PERMISSIONS
    ft_printf("%c", file->type);
    ft_printf("%.3s", file->perms.user);
    ft_printf("%.3s", file->perms.group);
    ft_printf("%.3s", file->perms.other);
    ft_printf(" ");
    // LINKS
    ft_printf("%*u", padding->link, file->stat.st_nlink);
    ft_printf(" ");

    bool show_ids = data->settings.display.numeric_ids;
    if (show_ids) {
      // OWNER
      if (!data->settings.display.omit_owner)
        ft_printf("%*u ", padding->owner_name, file->stat.st_uid);
      // GROUP
      if (!data->settings.display.omit_group)
        ft_printf("%*u ", padding->group_name, file->stat.st_gid);
      // AUTHOR
      if (data->settings.display.author)
        ft_printf("%*u ", padding->owner_name, file->stat.st_uid);
    }
    else {
      // OWNER
      if (!data->settings.display.omit_owner)
        ft_printf("%-*s ", padding->owner_name, file->owner);
      // GROUP
      if (!data->settings.display.omit_group)
        ft_printf("%-*s ", padding->group_name, file->group);
      // AUTHOR
      if (data->settings.display.author)
        ft_printf("%-*s ", padding->owner_name, file->owner);
    }
    if (
      data->settings.display.omit_group &&
      !data->settings.display.author &&
      data->settings.display.omit_owner
      )
      ft_printf(" ");
    // SIZE, TODO: compute based on block-size
    ft_printf("%*d ", padding->size, file->stat.st_size);
    // DATE
    char date[DATE_SIZE];
    ft_printf("%s ", get_file_verbose_time(file, date));
  }
  char* color_code = get_color_for_file(file, data->colors, &data->settings);
  ft_printf("%s%s%s", color_code ? color_code : "", file->name, color_code ? COLORS_RESET : "");
  data->first_batch_print = false;
}
