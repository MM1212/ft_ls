/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commas.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 22:37:50 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cbg(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.format.type = FORMAT_COMMAS;
}

bool io_describe_format_commas(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "commas_format",
    "fill width with a comma separated list of entries",
    false
  );
  if (!opt)
    return false;
  opt->add_switch('m')
    ->set_aliases("--format=commas")
    ->set_cb((t_cli_option_cb)cbg)
    ->end();
  return cli->is_valid();
}
