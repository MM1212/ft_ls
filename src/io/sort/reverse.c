/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reverse.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 22:41:38 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.sort.reverse = true;
}

bool io_describe_sort_reverse(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "reverse",
    "reverse order while sorting",
    true
  );
  if (!opt)
    return false;
  opt->add_switch('r')
    ->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}
