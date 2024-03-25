/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   date.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 20:58:32 by martiper          #+#    #+#             */
/*   Updated: 2024/03/25 22:58:59 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "date.h"

#define SIX_MONTHS_EPOC (60 * 60 * 24 * 30 * 6) // 6 months in seconds

static bool is_date_6_months_ago(time_t date) {
  time_t now = time(NULL);
  return now - date > SIX_MONTHS_EPOC;
}

void date_output_verbose_date(t_file* file, t_settings* settings) {
  if (!file->statd && !file_stat(file))
    return ;
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
    ft_printf("%s ", date_format(date, "%b %_d"));
    ft_printf("%s ", date_format(date, is_old ? " %Y" : "%H:%M"));
    break;
  case TIME_STYLE_ISO:
    if (is_old)
      ft_printf("%s ", date_format(date, "%Y-%m-%d "));
    else
      ft_printf("%s ", date_format(date, "%m-%d %H:%M"));
    break;
  case TIME_STYLE_FORMAT:
    ft_printf("%s ", date_format(date, settings->format.time_format));
    break;
  }
}
// [[CC]YY]MMDDhhmm[.ss]
