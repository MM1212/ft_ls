/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:31:13 by martiper          #+#    #+#             */
/*   Updated: 2024/03/26 21:35:28 by martiper         ###   ########.fr       */
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

static void display_directory(char* pre_parents, t_file* dir, t_ft_ls* data);

static void display_dir_recursive(char* parent, t_file* file, t_ft_ls* data) {
  if (file->type != FILE_DIRECTORY)
    return;
  if (data->first_batch_print)
    data->first_batch_print = false;
  else
    ft_printf("\n");
  display_directory(parent, file, data);
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

static void file_print_wrapper(t_file* file, size_t idx, t_ft_ls* data) {
  (void)idx;
  file_print(file, data);
}

static void display_directory(char* pre_parents, t_file* dir, t_ft_ls* data) {
  t_vector* files = get_files_from_dir(dir, &data->settings);
  if (!files)
    return;
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
  data->padding = get_padding2(files, &data->settings);
  if (data->settings.format.type == FORMAT_LONG)
    ft_printf("total %u\n", get_total_blocks2(files));
  if (!files->size) {
    files->destroy(files);
    return;
  }
  data->first_batch_print = true;
  sort_files2(files, &data->settings);
  files->foreach(files, (t_vector_foreach_f)file_print_wrapper, data);
  ft_printf("\n");
  if (data->settings.filter.recursive) {
    pre_parents = resolve_path(2, pre_parents, dir->input);
    if (!pre_parents) {
      files->destroy(files);
      ft_show_error(data, EXIT_FATAL, true, false, "cannot allocate memory");
    }
    for (uint32_t i = 0; i < files->size; i++) {
      t_file* file = files->at(files, i);
      display_dir_recursive(pre_parents, file, data);
    }
    free(pre_parents);
  }
  files->destroy(files);
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
    display_directory("", dir, data);
    if (iter->next)
      ft_printf("\n");
    iter = iter->next;
  }
}
