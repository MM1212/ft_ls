/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   long.c                                             :+:      :+:    :+:   */
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
  data->settings.format.type = FORMAT_LONG;
}

bool io_describe_format_long(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "long_format",
    "use a long listing format",
    false
  );
  if (!opt)
    return false;
  opt->add_switch('l')
    ->set_aliases("--format=long,--format=verbose")
    ->set_cb((t_cli_option_cb)cbg)
    ->end();
  return cli->is_valid();
}
