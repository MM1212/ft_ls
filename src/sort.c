/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 11:49:36 by martiper          #+#    #+#             */
/*   Updated: 2024/03/26 17:15:23 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "sort.h"

int sort_by_ascii(t_file* a, t_file* b) {
  return ft_strcmp(a->name, b->name);
}

static time_t get_file_cmp_time(t_file* file, t_settings* settings) {
  switch (settings->format.time) {
  case TIME_ACCESSED:
    return file->stat.st_atime;
  case TIME_CREATED:
  case TIME_STATUS_CHANGED:
    return file->stat.st_ctime;
  case TIME_MODIFIED:
  default:
    return file->stat.st_mtime;
  }
}

static size_t get_file_cmp_nano_time(t_file* file, t_settings* settings) {
  switch (settings->format.time) {
  case TIME_ACCESSED:
    return file->stat.st_atim.tv_nsec;
  case TIME_CREATED:
  case TIME_STATUS_CHANGED:
    return file->stat.st_ctim.tv_nsec;
  case TIME_MODIFIED:
  default:
    return file->stat.st_mtim.tv_nsec;
  }
}

int sort_by_time(t_file* a, t_file* b, t_settings* settings) {
  if (!file_stat(a) || !file_stat(b))
    return 0;
  time_t a_time = get_file_cmp_time(a, settings);
  time_t b_time = get_file_cmp_time(b, settings);
  if (a_time == b_time) {
    size_t a_nano = get_file_cmp_nano_time(a, settings);
    size_t b_nano = get_file_cmp_nano_time(b, settings);
    if (a_nano == b_nano)
      return ft_strcmp(a->name, b->name);
    return b_nano - a_nano;
  }
  return b_time - a_time;
}

int sort_by_size(t_file* a, t_file* b) {
  if (!file_stat(a) || !file_stat(b))
    return 0;
  if (a->size == b->size)
    return ft_strcmp(a->name, b->name);
  return b->size - a->size;
}

int sort_by_reverse(t_file* a, t_file* b) {
  (void)a;
  (void)b;
  return -1;
}

int sort_by_version(t_file* a, t_file* b) {
  bool a_is_version = ft_isnbr(a->name);
  bool b_is_version = ft_isnbr(b->name);
  if (!a_is_version && !b_is_version)
    return ft_strcmp(a->name, b->name);
  if (!a_is_version && b_is_version)
    return 1;
  if (a_is_version && !b_is_version)
    return -1;
  long a_version = ft_atol(a->name);
  long b_version = ft_atol(b->name);
  if (a_version == b_version)
    return ft_strcmp(a->name, b->name);
  return a_version - b_version;
}

void sort_files(t_list* files, t_settings* settings) {
  if (settings->sort.type == SORT_NONE)
    return;
  if (settings->sort.type == SORT_ASCII)
    settings->sort.cmp = (t_settings_sort_cmp)sort_by_ascii;
  else if (settings->sort.type == SORT_TIME)
    settings->sort.cmp = (t_settings_sort_cmp)sort_by_time;
  else if (settings->sort.type == SORT_SIZE)
    settings->sort.cmp = (t_settings_sort_cmp)sort_by_size;
  else if (settings->sort.type == SORT_VERSION)
    settings->sort.cmp = (t_settings_sort_cmp)sort_by_version;

  ft_lstsort2(files, (t_lst_cmp2)settings->sort.cmp, settings);
  if (settings->sort.reverse)
    ft_lstsort(files, (int (*)(void*, void*))sort_by_reverse);
}
