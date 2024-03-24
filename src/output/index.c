/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 17:31:13 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 22:25:24 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_ls.h>
#include "output.h"

static void display_files(t_list* files, t_ft_ls* data) {
  data->padding = get_padding(files, &data->settings);
  data->first_batch_print = true;
  ft_lstiter2(files, (t_lst_iter2)file_print, data);
  ft_printf("\n");
}

static void display_directory(t_file* dir, t_ft_ls* data) {
  t_list* files = get_files_from_dir(dir, &data->settings);
  if (!files)
    return;
  if (ft_lstsize(files)) {
    data->padding = get_padding(files, &data->settings);
    if (data->settings.format.type == FORMAT_LONG)
      ft_printf("total %u\n", get_total_blocks(files));
    data->first_batch_print = true;
    ft_lstiter2(files, (t_lst_iter2)file_print, data);
    // ft_lstiter(files, (t_lst_iter)file_debug_print);
    ft_printf("\n");
  }
  ft_lstclear(&files, (void (*)(void *))file_free);
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
    if (data->settings.print_dir_name)
      ft_printf("%s:\n", dir->input);
    display_directory(dir, data);
    if (iter->next)
      ft_printf("\n");
    iter = iter->next;
  }
}