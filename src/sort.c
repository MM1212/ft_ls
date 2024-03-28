/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 11:49:36 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 23:47:55 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <time.h>
#include "sort.h"
#include <ft_ls.h>

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

static void choose_sort_cmp(t_settings* settings) {
  switch (settings->sort.type) {
  case SORT_NONE:
    break;
  case SORT_WIDTH:
    // settings->sort.cmp = (t_settings_sort_cmp)sort_by_width;
    break;
  case SORT_ASCII:
    settings->sort.cmp = (t_settings_sort_cmp)sort_by_ascii;
    break;
  case SORT_TIME:
    settings->sort.cmp = (t_settings_sort_cmp)sort_by_time;
    break;
  case SORT_SIZE:
    settings->sort.cmp = (t_settings_sort_cmp)sort_by_size;
    break;
  case SORT_VERSION:
    settings->sort.cmp = (t_settings_sort_cmp)sort_by_version;
    break;
  }
}

static int sort2_wrapper(t_file** a, t_file** b, t_settings* settings) {
  if (settings->sort.reverse)
    return settings->sort.cmp(*b, *a, settings);
  return settings->sort.cmp(*a, *b, settings);
}

static int sort_wrapper(t_file* a, t_file* b, t_settings* settings) {
  if (settings->sort.reverse)
    return settings->sort.cmp(b, a, settings);
  return settings->sort.cmp(a, b, settings);
}

void reverse_sort(t_vector* files) {
  t_file* tmp, ** a, ** b;
  for (size_t i = 0; i < files->size / 2; i++) {
    a = files->at(files, i);
    b = files->at(files, files->size - i - 1);
    tmp = *a;
    *a = *b;
    *b = tmp;
  }
}

t_list* sort_files(t_list* files, t_settings* settings) {
  if (settings->sort.type == SORT_NONE)
    return files;

  choose_sort_cmp(settings);
  return ft_lstsort2(files, (t_lst_cmp2)sort_wrapper, settings);
}

t_vector* sort_files2(t_vector* files, t_settings* settings) {
  if (settings->sort.type == SORT_NONE)
    return files;

  choose_sort_cmp(settings);
  t_vector* sorted = files->sort3(files, (t_vector_cmp2_f)sort2_wrapper, settings, NULL);
  return sorted;
}
