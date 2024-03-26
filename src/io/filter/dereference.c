
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursive.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 22:07:50 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.filter.dereference_links = true;
}

static void cli_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.filter.dereference_links_cli = true;
}

bool io_describe_filter_dereference_links_cli(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "dereference_links_cli",
    "follow symbolic links listed on the command line",
    false
  );
  if (!opt)
    return false;
  opt->add_switch('H')
    ->add_flag("dereference-command-line")
    ->set_cb((t_cli_option_cb)cli_cb)
    ->end();
  return cli->is_valid();
}

bool io_describe_filter_dereference_links(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "dereference",
    "when showing file information for a symbolic \
link, show information for the file the link \
references rather than for the link itself",
true
);
  if (!opt)
    return false;
  opt->add_switch('L')
    ->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}
