/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:28:13 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static const char* format_options[] = {
  "long,verbose",
  "commas",
  "horizontal,across",
  // "vertical",
  "single-column",
  NULL
};

static bool (*io_describe_formats[])(t_cli_handle*) = {
  io_describe_format_long,
  io_describe_format_commas,
  io_describe_format_horizontal,
  // io_describe_format_vertical,
  io_describe_format_time,
  io_describe_format_time_style,
  io_describe_format_indicator_style
};

static void cb(t_cli_option* opt, t_ft_ls* data) {
  if (ft_strcmp(opt->value, "long") == 0)
    data->settings.format.type = FORMAT_LONG;
  else if (ft_strcmp(opt->value, "commas") == 0)
    data->settings.format.type = FORMAT_COMMAS;
  else if (ft_strcmp(opt->value, "horizontal") == 0)
    data->settings.format.type = FORMAT_HORIZONTAL;
  // else if (ft_strcmp(opt->value, "vertical") == 0)
  //   data->settings.format.type = FORMAT_VERTICAL;
  else if (ft_strcmp(opt->value, "single-column") == 0)
    data->settings.format.type = FORMAT_SINGLE_COLUMN;
}

bool io_describe_format_options(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "format",
    "across -x, commas -m, horizontal -x, long -l, single-column -1, verbose -l, vertical -C",
    true
  );
  if (!opt)
    return false;
  opt->set_cb((t_cli_option_cb)cb)
    ->set_type(CLI_OPTION_SELECT)
    ->set_flags(CLI_OPTION_FLAG_REQUIRED);
  for (uint32_t i = 0; format_options[i]; i++)
  {
    char** aliases = ft_split(format_options[i], ",");
    opt->add_choice(aliases[0], aliases);
    ft_split_free(aliases);
  }
  opt->end();
  for (size_t i = 0; i < sizeof(io_describe_formats) / sizeof(*io_describe_formats); i++) {
    if (!io_describe_formats[i](cli))
      return false;
  }
  return cli->is_valid();
}
