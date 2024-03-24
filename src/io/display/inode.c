/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inode.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 22:07:50 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void ignore_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.display.inode = true;
}

bool io_describe_display_inode(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "inode",
    "print the index number of each file",
    true
  );
  if (!opt)
    return false;
  opt->add_switch('i')
    ->set_cb((t_cli_option_cb)ignore_cb)
    ->end();
  return cli->is_valid();
}
