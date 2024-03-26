/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:31:13 by martiper          #+#    #+#             */
/*   Updated: 2024/03/26 17:19:54 by martiper         ###   ########.fr       */
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

static void display_directory(char* pre_parents, t_file* dir, t_ft_ls* data) {
  t_list* files = get_files_from_dir(dir, &data->settings);
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
  data->padding = get_padding(files, &data->settings);
  if (data->settings.format.type == FORMAT_LONG)
    ft_printf("total %u\n", get_total_blocks(files));
  if (!files)
    return;
  data->first_batch_print = true;
  sort_files(files, &data->settings);
  ft_lstiter2(files, (t_lst_iter2)file_print, data);
  ft_printf("\n");
  if (data->settings.filter.recursive) {
    pre_parents = resolve_path(2, pre_parents, dir->input);
    if (!pre_parents) {
      ft_lstclear(&files, (void (*)(void*))file_free);
      ft_show_error(data, EXIT_FATAL, true, false, "cannot allocate memory");
    }
    t_list* iter = files;
    while (iter) {
      display_dir_recursive(pre_parents, iter->content, data);
      iter = iter->next;
    }
    free(pre_parents);
  }
  ft_lstclear(&files, (void (*)(void*))file_free);
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
