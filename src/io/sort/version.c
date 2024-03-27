/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   version.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:27:42 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.sort.type = SORT_VERSION;
}

bool io_describe_sort_version(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "sort_version",
    "natural sort of (version) numbers within text",
    false
  );
  if (!opt)
    return false;
  opt->add_switch('v')
    ->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}
