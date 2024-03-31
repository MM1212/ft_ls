/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 11:16:47 by martiper          #+#    #+#             */
/*   Updated: 2024/03/31 12:10:00 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ft_ls.h>
#include <file.h>
#include <sort.h>
#include <sys/ioctl.h>
#include <block_size.h>

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
  data->settings.terminal_width = -1;
  g_ls_data = data;
  if (!data->io || !data->colors || !data->dir_cache)
    return false;
  block_size_compute();
  return true;
}

static void manage_settings(t_ft_ls* data) {
  if (data->settings.format.type == FORMAT_NONE) {
    data->settings.format.type = !data->settings.is_tty ? FORMAT_SINGLE_COLUMN : FORMAT_VERTICAL;
  }
  io_manage_sorting_time(data);
  if (data->settings.terminal_width < 0) {
    // get terminal width
    struct winsize w;
    if (!data->settings.is_tty || ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
      data->settings.terminal_width = DEFAULT_TERMINAL_WIDTH;
    else
      data->settings.terminal_width = w.ws_col;
  }
  if (!data->io->is_present("tabsize"))
    data->settings.tab_size = 8;
  if (!data->settings.format.block_size_scalar)
    data->settings.format.block_size_scalar = 1;
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
  if (data.io->is_present("debug"))
    settings_print(&data.settings);
  parse_entries(&data, paths);
  ft_ls_run(&data);
  ft_exit(&data, data.exit_status, NULL);
}
