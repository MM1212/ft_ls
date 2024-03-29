/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scontext.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/29 18:03:21 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.display.scontext = true;
}

bool io_describe_display_scontext(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "context",
    "print any security context of each file",
    true
  );
  if (!opt)
    return false;
  opt->add_switch('Z')
    ->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}
