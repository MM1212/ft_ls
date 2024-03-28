/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 19:42:32 by martiper          #+#    #+#             */
/*   Updated: 2024/03/28 11:39:32 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "output.h"
#include <ft_ls.h>
#include <system.h>
#include <date.h>

# define PRINT_BUFF_SIZE 2048
# define GET_REMAINING_SIZE (PRINT_BUFF_SIZE - (p - buf))

void file_print(t_file* file, t_ft_ls* data) {
  char  buf[2048];
  char* p = buf;
  t_l_fmt_padding* padding = &data->padding;
  if (data->settings.format.type == FORMAT_LONG && !file_stat(file))
    return;
  if (!data->first_batch_print) {
    switch (data->settings.format.type) {
    case FORMAT_LONG:
    case FORMAT_SINGLE_COLUMN:
      p += ft_sprintf(p, GET_REMAINING_SIZE, "\n");
      break;
    case FORMAT_COMMAS:
      p += ft_sprintf(p, GET_REMAINING_SIZE, ", ");
      break;
    case FORMAT_HORIZONTAL:
      p += ft_sprintf(p, GET_REMAINING_SIZE, "  ");
      break;
    default:
      break;
    }
  }
  if (data->settings.display.inode)
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%-*u ", padding->inode, file->id);
  if (data->settings.display.block_size && file_stat(file))
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%*u ", padding->block_size, FS_BLOCK_SIZE(file->stat.st_blocks));
  bool is_format_long = data->settings.format.type == FORMAT_LONG;
  if (is_format_long) {
    // PERMISSIONS
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%c", file->type);
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%.3s", file->perms.user);
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%.3s", file->perms.group);
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%.3s", file->perms.other);
    p += ft_sprintf(p, GET_REMAINING_SIZE, " ");
    // LINKS
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%*u", padding->link, file->stat.st_nlink);
    p += ft_sprintf(p, GET_REMAINING_SIZE, " ");

    bool show_ids = data->settings.display.numeric_ids;
    if (show_ids) {
      // OWNER
      if (!data->settings.display.omit_owner)
        p += ft_sprintf(p, GET_REMAINING_SIZE, "%*u ", padding->owner_name, file->stat.st_uid);
      // GROUP
      if (!data->settings.display.omit_group)
        p += ft_sprintf(p, GET_REMAINING_SIZE, "%*u ", padding->group_name, file->stat.st_gid);
      // AUTHOR
      if (data->settings.display.author)
        p += ft_sprintf(p, GET_REMAINING_SIZE, "%*u ", padding->owner_name, file->stat.st_uid);
    }
    else {
      // OWNER
      if (!data->settings.display.omit_owner)
        p += ft_sprintf(p, GET_REMAINING_SIZE, "%-*s ", padding->owner_name, file->owner);
      // GROUP
      if (!data->settings.display.omit_group)
        p += ft_sprintf(p, GET_REMAINING_SIZE, "%-*s ", padding->group_name, file->group);
      // AUTHOR
      if (data->settings.display.author)
        p += ft_sprintf(p, GET_REMAINING_SIZE, "%-*s ", padding->owner_name, file->owner);
    }
    if (
      data->settings.display.omit_group &&
      !data->settings.display.author &&
      data->settings.display.omit_owner
      )
      p += ft_sprintf(p, GET_REMAINING_SIZE, " ");
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%*d ", padding->size, file->stat.st_size);
    // DATE
    date_output_verbose_date(file, &data->settings);
  }
  // file name
  char* color_code = get_color_for_file(file, data->colors, &data->settings);
  p += ft_sprintf(p, GET_REMAINING_SIZE, "%s%s%s", color_code ? color_code : "", file->name, color_code ? COLORS_RESET : "");
  char indicator = get_indicator_style(file, &data->settings);
  if (indicator)
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%c", indicator);
  if (is_format_long && file->type == FILE_SYMLINK && file->symlinkd && file->symlink) {
    t_file link;

    if (file_from_symlink_view(&link, file)) {
      char* color_code = get_color_for_file(&link, data->colors, &data->settings);
      p += ft_sprintf(p, GET_REMAINING_SIZE, " -> %s%s%s", color_code ? color_code : "", file->symlink, color_code ? COLORS_RESET : "");
      indicator = get_indicator_style(&link, &data->settings);
      if (indicator)
        p += ft_sprintf(p, GET_REMAINING_SIZE, "%c", indicator);
    }
  }
  data->first_batch_print = false;
  ft_printf("%s", buf);
}
