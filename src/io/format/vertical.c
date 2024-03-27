/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vertical.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:27:48 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.format.type = FORMAT_VERTICAL;
}

bool io_describe_format_vertical(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "vertical_format",
    "list entries by columns",
    false
  );
  if (!opt)
    return false;
  opt->add_switch('C')
    ->set_aliases("--format=vertical")
    ->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}
