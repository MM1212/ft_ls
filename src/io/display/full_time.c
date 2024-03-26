/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   full_time.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/26 12:34:43 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.format.type = FORMAT_LONG;
  data->settings.format.time_style = TIME_STYLE_FULL;
}

bool io_describe_display_full_time(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "full_time",
    "like -l --time-style=full-iso",
    false
  );
  if (!opt)
    return false;
  opt->add_flag("full-time")
    ->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}
