/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   numeric_ids.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:28:59 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.display.numeric_ids = true;
  data->settings.format.type = FORMAT_LONG;
}

bool io_describe_display_numeric_ids(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "numeric_ids",
    "like -l, but list numeric user and group IDs",
    false
  );
  if (!opt)
    return false;
  opt->add_switch('n')
    ->add_flag("numeric-uid-gid")
    ->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}
