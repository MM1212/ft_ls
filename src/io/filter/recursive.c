/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recursive.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/23 23:18:38 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"

bool io_describe_filter_recursive(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "recursive",
    "list subdirectories recursively",
    true
  );
  if (!opt)
    return false;
  opt->add_switch('R')
    ->end();
  return cli->is_valid();
}
