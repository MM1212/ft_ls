/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:29:22 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 21:57:43 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"

static bool (*io_describe_displays[])(t_cli_handle*) = {
  io_describe_display_color,
  io_describe_display_inode,
  io_describe_display_numeric_ids,
  io_describe_display_omits,
  io_describe_display_full_time,
  io_describe_display_size
};

bool io_describe_display_options(t_cli_handle *cli) {
  for (size_t i = 0; i < sizeof(io_describe_displays) / sizeof(*io_describe_displays); i++) {
    if (!io_describe_displays[i](cli))
      return false;
  }
  return true;
}
