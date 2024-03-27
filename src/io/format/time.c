/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 18:53:53 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static const char* time_style_options[] = {
  "atime,access,use",
  "ctime,change,status",
  "mtime,modify",
  "birth,creation",
  NULL
};



static void cb(t_cli_option* opt, t_ft_ls* data) {
  if (ft_strcmp(opt->value, "atime") == 0)
    data->settings.format.time = TIME_ACCESSED;
  else if (ft_strcmp(opt->value, "ctime") == 0 || ft_strcmp(opt->value, "birth") == 0)
    data->settings.format.time = TIME_STATUS_CHANGED;
  else if (ft_strcmp(opt->value, "mtime") == 0)
    data->settings.format.time = TIME_MODIFIED;
}

static void c_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.format.time = TIME_STATUS_CHANGED;
}

static void u_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.format.time = TIME_ACCESSED;
}

static bool add_c_flag(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(
    "time_ctime",
    "with -lc: sort by, and show, ctime (time of last\
 modification of file status information);\n\
with -l: show ctime and sort by name;\n\
otherwise: sort by ctime, newest first",
false
);
  if (!opt)
    return false;
  opt->set_cb((t_cli_option_cb)c_cb)
    ->add_switch('c')
    ->end();
  return cli->is_valid();
}

static bool add_u_flag(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(
    "time_atime",
    "with -lu: sort by, and show, access time;\n\
with -l: show access time and sort by name;\n\
otherwise: sort by access time, newest first",
false
);
  if (!opt)
    return false;
  opt->set_cb((t_cli_option_cb)u_cb)
    ->add_switch('u')
    ->end();
  return cli->is_valid();
}

bool io_describe_format_time(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "time",
    "change the default of using modification times;\n\
 - access time (-u): atime, access, use;\n\
 - change time (-c): ctime, status;\n\
 - birth time: birth, creation;\r\n\
 with -l, WORD determines which time to show;\r\n\
 with --sort=time, sort by WORD (newest first)",
    true
  );
  if (!opt)
    return false;
  opt->set_cb((t_cli_option_cb)cb)
    ->set_type(CLI_OPTION_SELECT)
    ->set_flags(CLI_OPTION_FLAG_REQUIRED);
  for (uint32_t i = 0; time_style_options[i]; i++)
  {
    char** aliases = ft_split(time_style_options[i], ",");
    opt->add_choice(aliases[0], aliases);
    ft_split_free(aliases);
  }
  opt->end();

  if (!add_c_flag(cli))
    return false;
  if (!add_u_flag(cli))
    return false;
  return cli->is_valid();
}
