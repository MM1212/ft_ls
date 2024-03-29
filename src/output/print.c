/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 19:42:32 by martiper          #+#    #+#             */
/*   Updated: 2024/03/29 15:59:41 by martiper         ###   ########.fr       */
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

  bool broken_link = is_file_a_broken_link(file);

  if (data->settings.display.inode)
    p += ft_sprintf(p, GET_REMAINING_SIZE, broken_link ? "%*c " : "%*u ", padding->inode, broken_link ? '?' : file->id);
  if (data->settings.display.block_size && file_stat(file))
    p += ft_sprintf(p, GET_REMAINING_SIZE, broken_link ? "%*c " : "%*u ", padding->block_size, broken_link ? '?' : FS_BLOCK_SIZE(file->stat.st_blocks));
  bool is_format_long = data->settings.format.type == FORMAT_LONG;

  if (is_format_long) {
    // PERMISSIONS
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%c", file->type);
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%.3s", file->perms.user);
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%.3s", file->perms.group);
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%.3s", file->perms.other);
    p += ft_sprintf(p, GET_REMAINING_SIZE, " ");
    // LINKS
    p += ft_sprintf(p, GET_REMAINING_SIZE, broken_link ? "%*c" : "%*u", padding->link, broken_link ? '?' : file->stat.st_nlink);
    p += ft_sprintf(p, GET_REMAINING_SIZE, " ");

    bool show_ids = data->settings.display.numeric_ids;
    if (show_ids) {
      // OWNER
      if (!data->settings.display.omit_owner)
        p += ft_sprintf(p, GET_REMAINING_SIZE, broken_link ? "%*c " : "%*u ", padding->owner_name, broken_link ? '?' : file->stat.st_uid);
      // GROUP
      if (!data->settings.display.omit_group)
        p += ft_sprintf(p, GET_REMAINING_SIZE, broken_link ? "%*c " : "%*u ", padding->group_name, broken_link ? '?' : file->stat.st_gid);
      // AUTHOR
      if (data->settings.display.author)
        p += ft_sprintf(p, GET_REMAINING_SIZE, broken_link ? "%*c " : "%*u ", padding->owner_name, broken_link ? '?' : file->stat.st_uid);
    }
    else {
      // OWNER
      if (!data->settings.display.omit_owner)
        p += ft_sprintf(p, GET_REMAINING_SIZE, broken_link ? "%*.1s " : "%-*s ", padding->owner_name, file->owner);
      // GROUP
      if (!data->settings.display.omit_group)
        p += ft_sprintf(p, GET_REMAINING_SIZE, broken_link ? "%*.1s " : "%-*s ", padding->group_name, file->group);
      // AUTHOR
      if (data->settings.display.author)
        p += ft_sprintf(p, GET_REMAINING_SIZE, broken_link ? "%*.1s " : "%-*s ", padding->owner_name, file->owner);
    }
    if (
      data->settings.display.omit_group &&
      !data->settings.display.author &&
      data->settings.display.omit_owner
      )
      p += ft_sprintf(p, GET_REMAINING_SIZE, " ");
    p += ft_sprintf(p, GET_REMAINING_SIZE, broken_link ? "%*c " : "%*d ", padding->size, broken_link ? '?' : file->stat.st_size);
    // DATE
    if (broken_link)
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%*c ", padding->date, '?');
    else
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%s ", date_get_verbose_date(file, &data->settings));
  }
  // file name
  char* color_code = get_color_for_file(file, data->colors, &data->settings);
  p += ft_sprintf(p, GET_REMAINING_SIZE, "%s%s%s", color_code ? color_code : "", file->name, color_code ? COLORS_RESET : "");
  char indicator = get_indicator_style(file, &data->settings);
  if (indicator)
    p += ft_sprintf(p, GET_REMAINING_SIZE, "%c", indicator);
  if (is_format_long && file->type == FILE_SYMLINK && !(data->settings.filter.dereference_links || data->settings.filter.dereference_links_cli)) {
    char* color_code = get_color_for_file(file, data->colors, &data->settings);
    p += ft_sprintf(p, GET_REMAINING_SIZE, " -> %s%s%s", color_code ? color_code : "", file->symlink, color_code ? COLORS_RESET : "");
    t_file link_view;
    if (file_from_symlink_view(&link_view, file)) {
      indicator = get_indicator_style(&link_view, &data->settings);
      if (indicator)
        p += ft_sprintf(p, GET_REMAINING_SIZE, "%c", indicator);
    }
  }
  write(1, buf, p - buf);
}

void print_files_with_separator(t_vector* files, char sep, t_ft_ls* data) {
  size_t pos = 0;
  size_t line_length = data->settings.terminal_width;
  for (size_t i = 0; i < files->size; i++) {
    t_file* file = *(t_file**)files->at(files, i);
    size_t len = get_file_name_and_extras_length(file, &data->padding, &data->settings);
    if (i > 0) {
      char separator;
      if (
        !line_length ||
        ((pos + len + 2 < line_length) && (pos <= SIZE_MAX - len - 2))
        ) {
        pos += 2;
        separator = ' ';
      }
      else {
        pos = 0;
        separator = EOL_BYTE;
      }
      ft_printf("%c%c", sep, separator);
    }
    file_print(file, data);
    pos += len;
  }
  ft_putchar_fd(EOL_BYTE, 1);
}
