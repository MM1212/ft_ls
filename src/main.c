/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 11:16:47 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 17:53:55 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <ft_ls.h>
#include <file.h>

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
  base = ft_lstsort(base, (int (*)(void* a, void* b))sort_entries);
  data->settings.print_dir_name = data->settings.filter.recursive || i > 1;
  if (data->settings.filter.list_directories) {
    data->file_entries = base;
    return;
  }
  else if (!file_is_dir(base->content)) {
    data->file_entries = base;
    t_list* last;
    while (base) {
      t_file* file = base->content;
      if (file_is_dir(file))
        break;
      last = base;
      base = base->next;
    }
    if (last)
      last->next = NULL;
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
  parse_entries(&data, paths);
  ft_ls_run(&data);
  ft_exit(&data, data.exit_status, NULL);
}
