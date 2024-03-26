/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   date.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 20:58:32 by martiper          #+#    #+#             */
/*   Updated: 2024/03/26 10:50:58 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "date.h"

#define SIX_MONTHS_EPOC (60 * 60 * 24 * 30 * 6) // 6 months in seconds

static bool is_date_6_months_ago(time_t date) {
  time_t now = time(NULL);
  return now - date > SIX_MONTHS_EPOC;
}
// Wed Jun 30 21:49:08 1993
static char* get_verbose_locale_time(time_t date, bool is_old) {
  static char buff[33];
  char* str = ctime(&date);
  if (!str)
    return NULL;
  str += 4;
  ft_bzero(buff, sizeof(buff));
  ft_strlcat(buff, str, 8);
  if (is_old) {
    ft_strlcat(buff, " ", 33);
    str += 16;
    str[4] = '\0';
  }
  else {
    str += 7;
    str[5] = '\0';
  }
  ft_strlcat(buff, str, 33);
  return buff;
}

void date_output_verbose_date(t_file* file, t_settings* settings) {
  if (!file->statd && !file_stat(file))
    return;
  time_t date;
  size_t nsecs;
  switch (settings->format.time) {
  case TIME_ACCESSED:
    date = file->stat.st_atime;
    nsecs = file->stat.st_atim.tv_nsec;
    break;
  case TIME_CREATED:
  case TIME_STATUS_CHANGED:
    date = file->stat.st_ctime;
    nsecs = file->stat.st_ctim.tv_nsec;
    break;
  case TIME_MODIFIED:
  default:
    date = file->stat.st_mtime;
    nsecs = file->stat.st_mtim.tv_nsec;
    break;
  }
  bool is_old = is_date_6_months_ago(date);
  switch (settings->format.time_style) {
  case TIME_STYLE_FULL:
    ft_printf("%s.%09u ", \
      date_format(date, "%Y-%m-%d %H:%M:%S"), \
      nsecs \
    );
    ft_printf("%s ", date_format(date, "%z"));
    break;
  case TIME_STYLE_LONG:
    ft_printf("%s ", date_format(date, "%Y-%m-%d %H:%M"));
    break;
  case TIME_STYLE_LOCALE:
    ft_printf("%s ", get_verbose_locale_time(date, is_old));
    break;
  case TIME_STYLE_ISO:
    if (is_old)
      ft_printf("%s ", date_format(date, "%Y-%m-%d "));
    else
      ft_printf("%s ", date_format(date, "%m-%d %H:%M"));
    break;
  case TIME_STYLE_FORMAT: {
    char buff[2048];
    ft_bzero(buff, sizeof(buff));
    strftime(buff, 2048, settings->format.time_format, localtime(&date));
    ft_printf("%s ", buff);
    break;
  }
  }
}
// [[CC]YY]MMDDhhmm[.ss]
