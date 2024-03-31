/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_column.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/31 16:05:51 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.format.type = FORMAT_SINGLE_COLUMN;
}

bool io_describe_format_single_column(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "single_column_format",
    "list one file name per line",
    false
  );
  if (!opt)
    return false;
  opt->add_switch('1')
    ->set_aliases("--format=single-column")
    ->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}
