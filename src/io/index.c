/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:12:37 by martiper          #+#    #+#             */
/*   Updated: 2024/03/26 12:36:01 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <debug.h>
#include <ft_ls.h>

int io_run(struct s_ls* data, int ac, char** av, bool* exit_request) {
  *exit_request = false;
  t_cli_handle* handle = data->io;
  if (
    !io_describe_common_options(handle) ||
    !io_describe_filter_options(handle) ||
    !io_describe_format_options(handle) ||
    !io_describe_display_options(handle) ||
    !io_describe_sort_options(handle) ||
    !io_describe_verbose_options(handle) ||
    !handle->parse(ac, av)
  ) {
    // if (DEBUG)
    //   handle->print();
    handle->output_error();
    ft_fprintf(STDERR_FILENO, "Try '%s --help' for more information.\n", av[0]);
    return IO_EXIT_MAJOR_ERROR;
  }
  *exit_request = handle->settings.should_exit;
  return IO_EXIT_SUCCESS;
}
