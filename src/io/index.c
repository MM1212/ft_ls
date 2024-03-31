/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:12:37 by martiper          #+#    #+#             */
/*   Updated: 2024/03/31 12:02:29 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <debug.h>
#include <ft_ls.h>

#ifdef DEBUG

static void debug_settings(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  (void)data;
  // data->io->print();
  // opt->_handle->settings.should_exit = true;
  // opt->_handle->settings.run_cb_only_once = true;
}

static bool add_debug(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(
    "debug",
    "output debug information and exit",
    true
  );
  if (opt == NULL)
    return false;
  opt->add_switch('D')->set_cb((t_cli_option_cb)debug_settings);
  return opt->end() != NULL;
}

#endif

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
#ifdef DEBUG
    !add_debug(handle) ||
#endif
    !handle->parse(ac, av)
    ) {
// #ifdef DEBUG
//     handle->print();
// #endif
    handle->output_error();
    ft_fprintf(STDERR_FILENO, "Try '%s --help' for more information.\n", av[0]);
    return IO_EXIT_MAJOR_ERROR;
  }
  *exit_request = handle->settings.should_exit;
  return IO_EXIT_SUCCESS;
}
