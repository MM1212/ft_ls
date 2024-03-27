/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:28:27 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void all_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.filter.all = true;
  if (data->settings.filter.almost_all)
    data->settings.filter.almost_all = false;
}

static void almost_all_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  if (data->settings.filter.all)
    return;
  data->settings.filter.almost_all = true;
}

bool io_describe_filter_all(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "all",
    "do not ignore entries starting with .",
    true
  );
  if (!opt)
    return false;
  opt->add_switch('a')
    ->set_cb((t_cli_option_cb)all_cb)
    ->end();
  return cli->is_valid();
}

bool io_describe_filter_almost_all(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "almost_all",
    "do not list implied . and ..",
    false
  );
  if (!opt)
    return false;
  opt->add_flag("almost-all")
    ->add_switch('A')
    ->set_cb((t_cli_option_cb)almost_all_cb)
    ->end();
  return cli->is_valid();
}
