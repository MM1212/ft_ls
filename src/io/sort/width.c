/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   width.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 22:45:31 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.sort.type = SORT_WIDTH;
}

bool io_describe_sort_width(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "sort_width",
    "assume the terminal is N columns wide. This option is used to determine the width of the output, which is used when formatting file names in a column.",
    false
  );
  if (!opt)
    return false;
  opt->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}