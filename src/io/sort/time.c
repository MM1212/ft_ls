/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:27:34 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.sort.type = SORT_TIME;
}

bool io_describe_sort_time(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "sort_time",
    "sort by time, newest first; see --time",
    false
  );
  if (!opt)
    return false;
  opt->add_switch('t')
    ->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}

static const char* time_options[] = {
  "time_ctime",
  "time_atime",
  NULL
};

void io_manage_sorting_time(struct s_ls* data) {
  if (data->settings.format.type != FORMAT_LONG &&
    !data->io->is_present("sort")) {
    for (uint32_t i = 0; time_options[i]; i++) {
      if (!data->io->is_present(time_options[i]))
        continue;
      data->settings.sort.type = SORT_TIME;
      break;
    }
  }
}
