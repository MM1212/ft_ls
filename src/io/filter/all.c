/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/23 23:19:44 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"

bool io_describe_filter_all(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "all",
    "do not ignore entries starting with .",
    true
  );
  if (!opt)
    return false;
  opt->add_switch('a')
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
    ->end();
  return cli->is_valid();
}
