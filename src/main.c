/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 11:16:47 by martiper          #+#    #+#             */
/*   Updated: 2024/03/28 00:10:02 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ft_ls.h>
#include <file.h>
#include <sort.h>

t_ft_ls* g_ls_data = NULL;

static int dir_cache_hash(const t_file* file) {
  return file->id;
}

static int dir_cache_cmp(const t_file* a, const t_file* b) {
  return a->id - b->id;
}

static bool init(t_ft_ls* data, char** env) {
  ft_bzero(data, sizeof(t_ft_ls));
  data->io = cli_begin("ft_ls", data);
  data->colors = colors_registry_create(env);
  data->settings.is_tty = isatty(STDOUT_FILENO);
  data->dir_cache = hashtable_create(30, (t_hashtable_hash)dir_cache_hash, (t_hashtable_cmp)dir_cache_cmp, NULL);
  g_ls_data = data;
  if (!data->io || !data->colors || !data->dir_cache)
    return false;
  return true;
}

static int sort_entries(t_file* a, t_file* b) {
  return file_is_dir(a) - file_is_dir(b);
}

static void manage_settings(t_ft_ls* data) {
  if (data->settings.format.type == FORMAT_NONE) {
    data->settings.format.type = data->settings.is_tty ? FORMAT_HORIZONTAL : FORMAT_SINGLE_COLUMN;
  }
  io_manage_sorting_time(data);
}

static void parse_entries(t_ft_ls* data, char** entries) {
  t_list* base = NULL;
  uint32_t i;
  for (i = 0; entries[i]; i++) {
    t_file* file = file_from_stat(entries[i], NULL, true);
    if (!file)
      continue;
    ft_lstadd_back(&base, ft_lstnew(file));
  }
  if (!base)
    return;
  base = sort_files(base, &data->settings);
  data->settings.print_dir_name = data->settings.filter.recursive || i > 1;
  if (data->settings.filter.list_directories) {
    data->file_entries = base;
    return;
  }
  else {
    t_list* iter = base;
    while (iter) {
      t_file* file = iter->content;
      if (file->type != FILE_DIRECTORY) {
        t_list* tmp = iter->next;
        if (iter->next)
          iter->next->prev = iter->prev;
        if (iter->prev)
          iter->prev->next = iter->next;
        else
          base = iter->next;
        iter->prev = NULL;
        iter->next = NULL;
        t_list* last = ft_lstlast(data->file_entries);
        if (last) {
          last->next = iter;
          iter->prev = last;
        }
        else
          data->file_entries = iter;
        iter = tmp;
      }
      else
        iter = iter->next;
    }
  }
  data->dir_entries = base;
}

int main(int ac, char** av, char** env) {
  t_ft_ls data;
  int status;
  bool exit_request;

  init(&data, env);
  if ((status = io_run(&data, ac, av, &exit_request)) != 0)
    ft_exit(&data, status, NULL);
  if (exit_request)
    ft_exit(&data, 0, NULL);
  char** paths = data.io->args;
  if (!paths || !paths[0])
    paths = (char* []){ ".", NULL };
  manage_settings(&data);
  // settings_print(&data.settings);
  parse_entries(&data, paths);
  ft_ls_run(&data);
  ft_exit(&data, data.exit_status, NULL);
}
