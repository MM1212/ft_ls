/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:29:22 by martiper          #+#    #+#             */
/*   Updated: 2024/03/26 12:29:02 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"

static bool (*io_describe_filters[])(t_cli_handle*) = {
  io_describe_filter_all,
  io_describe_filter_almost_all,
  io_describe_filter_recursive,
  io_describe_filter_ignore,
  io_describe_filter_ignore_backups,
  io_describe_filter_list_directories,
  io_describe_filter_dereference_links,
  io_describe_filter_dereference_links_cli
};

bool io_describe_filter_options(t_cli_handle *cli) {
  for (size_t i = 0; i < sizeof(io_describe_filters) / sizeof(*io_describe_filters); i++) {
    if (!io_describe_filters[i](cli))
      return false;
  }
  return true;
}
