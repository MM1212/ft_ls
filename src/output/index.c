/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:31:13 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 00:09:40 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>
#include "output.h"
#include <sort.h>

static void display_files(t_list* files, t_ft_ls* data) {
  data->padding = get_padding(files, &data->settings);
  data->first_batch_print = true;
  ft_lstiter2(files, (t_lst_iter2)file_print, data);
  ft_printf("\n");
}

static bool display_directory(bool first, char* pre_parents, t_file* dir, t_ft_ls* data);

static bool display_dir_recursive(bool first, char* parent, t_file* file, t_ft_ls* data) {
  if (file->type != FILE_DIRECTORY)
    return true;
  return display_directory(first, parent, file, data);
}

/* if (data->settings.filter.recursive) {
   bool has_dirs = false;
   t_list* iter = files;
   while (iter) {
     t_file* file = iter->content;
     if (file->type == FILE_DIRECTORY) {
       has_dirs = true;
       break;
     }
     iter = iter->next;
   }
   if (has_dirs)
     data->settings.print_dir_name = true;
 } */

static void file_print_wrapper(t_file** file, size_t idx, t_ft_ls* data) {
  (void)idx;
  file_print(*file, data);
}

static bool display_directory(bool first, char* pre_parents, t_file* dir, t_ft_ls* data) {
  bool is_in_dir_cache = data->dir_cache->get(data->dir_cache, dir);
  if (is_in_dir_cache) {
    char* name = data->settings.filter.recursive ? dir->input : dir->name;
    if (dir->parent) {
      char* path = resolve_path(2, pre_parents, name);
      ft_show_error(data, EXIT_MINOR, false, false, "%s not listing already-listed directory", dir->full_path);
      free(path);
    }
    else
      ft_show_error(data, EXIT_MINOR, false, false, "%s not listing already-listed directory", dir->input);
    return false;
  }
  (void)first;
  if (!first)
    ft_printf("\n");
  if (data->settings.print_dir_name) {
    char* name = data->settings.filter.recursive ? dir->input : dir->name;
    if (dir->parent) {
      char* path = resolve_path(2, pre_parents, name);
      ft_printf("%s:\n", path ? path : name);
      free(path);
    }
    else
      ft_printf("%s:\n", !dir->parent ? dir->input : dir->name);
  }
  data->dir_cache->add(data->dir_cache, dir, dir);
  t_vector* files = get_files_from_dir(dir, &data->settings);
  if (!files)
    return false;
  data->padding = get_padding2(files, &data->settings);
  if (data->settings.format.type == FORMAT_LONG)
    ft_printf("total %u\n", get_total_blocks2(files));
  if (!files->size) {
    files->destroy(files);
    return true;
  }
  t_vector* sorted = sort_files2(files, &data->settings);
  data->first_batch_print = true;
  sorted->foreach(sorted, (t_vector_foreach_f)file_print_wrapper, data);
  ft_printf("\n");
  if (data->settings.filter.recursive) {
    pre_parents = resolve_path(2, pre_parents, dir->input);
    if (!pre_parents) {
      files->destroy(files);
      sorted->destroy(sorted);
      ft_show_error(data, EXIT_FATAL, true, false, "cannot allocate memory");
    }
    for (uint32_t i = 0; i < sorted->size; i++) {
      t_file** file = sorted->at(sorted, i);
      display_dir_recursive(false, pre_parents, *file, data);
    }
    free(pre_parents);
  }
  sorted->destroy(sorted);
  data->dir_cache->remove(data->dir_cache, dir);
  files->destroy(files);
  return true;
}

void ft_ls_run(t_ft_ls* data) {
  if (data->file_entries)
    display_files(data->file_entries, data);
  if (!data->dir_entries)
    return;
  if (data->file_entries)
    ft_printf("\n");
  t_list* iter = data->dir_entries;
  while (iter) {
    t_file* dir = iter->content;
    bool ok = display_directory(!iter->prev,"", dir, data);
    if (ok && iter->next)
      ft_printf("\n");
    iter = iter->next;
  }
}
