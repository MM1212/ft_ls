/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:09:51 by martiper          #+#    #+#             */
/*   Updated: 2024/03/23 23:21:01 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cli.h>

typedef enum {
  IO_EXIT_SUCCESS = 0,
  IO_EXIT_MINOR_ERROR = 1,
  IO_EXIT_MAJOR_ERROR = 2,
} t_io_exit_codes;

int io_run(int argc, char **argv, bool *exit_request);
bool io_describe_verbose_options(t_cli_handle *cli);
/* COMMON */
bool io_describe_common_options(t_cli_handle *cli);
bool io_describe_block_size(t_cli_handle *cli);
/* FILTER */
bool io_describe_filter_options(t_cli_handle *cli);
bool io_describe_filter_all(t_cli_handle *cli);
bool io_describe_filter_almost_all(t_cli_handle *cli);
bool io_describe_filter_recursive(t_cli_handle *cli);
