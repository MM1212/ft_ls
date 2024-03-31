/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:09:51 by martiper          #+#    #+#             */
/*   Updated: 2024/03/31 16:06:01 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cli.h>

typedef enum {
  IO_EXIT_SUCCESS = 0,
  IO_EXIT_MINOR_ERROR = 1,
  IO_EXIT_MAJOR_ERROR = 2,
} t_io_exit_codes;

struct s_ls;

int io_run(struct s_ls* data, int argc, char** argv, bool* exit_request);
bool io_describe_verbose_options(t_cli_handle* cli);
/* COMMON */
bool io_describe_common_options(t_cli_handle* cli);
bool io_describe_block_size(t_cli_handle* cli);
/* FILTER */
bool io_describe_filter_options(t_cli_handle* cli);
bool io_describe_filter_all(t_cli_handle* cli);
bool io_describe_filter_almost_all(t_cli_handle* cli);
bool io_describe_filter_recursive(t_cli_handle* cli);
bool io_describe_filter_ignore(t_cli_handle* cli);
bool io_describe_filter_ignore_backups(t_cli_handle* cli);
bool io_describe_filter_list_directories(t_cli_handle* cli);
bool io_describe_filter_dereference_links(t_cli_handle* cli);
bool io_describe_filter_dereference_links_cli(t_cli_handle* cli);
/* FORMAT */
bool io_describe_format_options(t_cli_handle* cli);
bool io_describe_format_long(t_cli_handle* cli);
bool io_describe_format_commas(t_cli_handle* cli);
bool io_describe_format_horizontal(t_cli_handle* cli);
bool io_describe_format_vertical(t_cli_handle* cli);
bool io_describe_format_single_column(t_cli_handle* cli);
bool io_describe_format_time(t_cli_handle* cli);
bool io_describe_format_time_style(t_cli_handle* cli);
bool io_describe_format_indicator_style(t_cli_handle* cli);
bool io_describe_format_width(t_cli_handle* cli);
/* DISPLAY */
bool io_describe_display_options(t_cli_handle* cli);
bool io_describe_display_color(t_cli_handle* cli);
bool io_describe_display_inode(t_cli_handle* cli);
bool io_describe_display_numeric_ids(t_cli_handle* cli);
bool io_describe_display_omits(t_cli_handle* cli);
bool io_describe_display_full_time(t_cli_handle* cli);
bool io_describe_display_size(t_cli_handle* cli);
bool io_describe_display_scontext(t_cli_handle* cli);
/* SORT */
bool io_describe_sort_options(t_cli_handle* cli);
bool io_describe_sort_none(t_cli_handle* cli);
bool io_describe_sort_unsorted_mix(t_cli_handle* cli);
bool io_describe_sort_time(t_cli_handle* cli);
bool io_describe_sort_size(t_cli_handle* cli);
bool io_describe_sort_version(t_cli_handle* cli);
bool io_describe_sort_reverse(t_cli_handle* cli);
void io_manage_sorting_time(struct s_ls* data);

