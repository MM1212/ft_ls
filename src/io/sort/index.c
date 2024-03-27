/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 18:02:50 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static const char* sort_options[] = {
  "none",
  "time",
  "size",
  "version",
  // "width",
  NULL
};

static bool (*io_describe_sorts[])(t_cli_handle*) = {
  io_describe_sort_none,
  io_describe_sort_time,
  io_describe_sort_size,
  io_describe_sort_version,
  io_describe_sort_reverse,
  // io_describe_sort_width,
};

static void cb(t_cli_option* opt, t_ft_ls* data) {
  if (ft_strcmp(opt->value, "none") == 0)
    data->settings.sort.type = SORT_NONE;
  else if (ft_strcmp(opt->value, "time") == 0)
    data->settings.sort.type = SORT_TIME;
  else if (ft_strcmp(opt->value, "size") == 0)
    data->settings.sort.type = SORT_SIZE;
  else if (ft_strcmp(opt->value, "version") == 0)
    data->settings.sort.type = SORT_VERSION;
  // else if (ft_strcmp(opt->value, "width") == 0)
  //   data->settings.sort.type = SORT_WIDTH;
}

bool io_describe_sort_options(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "sort",
    "sort by WORD instead of name: none (-U), size (-S), time (-t), version (-v), extension (-X)",
    true
  );
  if (!opt)
    return false;
  opt->set_cb((t_cli_option_cb)cb)
    ->set_type(CLI_OPTION_SELECT)
    ->set_flags(CLI_OPTION_FLAG_REQUIRED);
  for (uint32_t i = 0; sort_options[i]; i++)
  {
    char** aliases = ft_split(sort_options[i], ",");
    opt->add_choice(aliases[0], aliases);
    ft_split_free(aliases);
  }
  opt->end();
  for (size_t i = 0; i < sizeof(io_describe_sorts) / sizeof(*io_describe_sorts); i++) {
    if (!io_describe_sorts[i](cli))
      return false;
  }
  return cli->is_valid();
}
