/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_directories.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:28:38 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.filter.list_directories = true;
}

bool io_describe_filter_list_directories(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "list_directories",
    "list directories themselves, not their contents",
    false
  );
  if (!opt)
    return false;
  opt->add_switch('d')
    ->add_flag("directory")
    ->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}
