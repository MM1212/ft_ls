/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   width.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/28 17:22:18 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  if (!ft_isnbr)
  data->settings.terminal_width = ft_atoi(opt->value);
}

bool io_describe_format_width(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "width",
    "set output width to COLS.  0 means no limit",
    false
  );
  if (!opt)
    return false;
  opt->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}
