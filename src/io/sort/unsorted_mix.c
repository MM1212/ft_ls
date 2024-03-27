/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unsorted_mix.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:27:39 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.sort.type = SORT_NONE;
  data->settings.format.block_size = BLOCK_SIZE_BYTES;
  data->settings.format.type = FORMAT_LONG;
  data->settings.display.block_size = false;
  data->settings.display.color = SETTINGS_COLOR_NEVER;
}

bool io_describe_sort_unsorted_mix(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "unsorted_mix",
    "do not sort, enable -aU, disable -ls --color",
    false
  );
  if (!opt)
    return false;
  opt->add_switch('f')
    ->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}
