/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_style.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:27:57 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static const char* time_style_options[] = {
  "full-iso",
  "long-iso",
  "iso",
  "locale",
  "+*",
  NULL
};



static void cb(t_cli_option* opt, t_ft_ls* data) {
  if (ft_strcmp(opt->value, "full-iso") == 0)
    data->settings.format.time_style = TIME_STYLE_FULL;
  else if (ft_strcmp(opt->value, "long-iso") == 0)
    data->settings.format.time_style = TIME_STYLE_LONG;
  else if (ft_strcmp(opt->value, "iso") == 0)
    data->settings.format.time_style = TIME_STYLE_ISO;
  else if (ft_strcmp(opt->value, "locale") == 0)
    data->settings.format.time_style = TIME_STYLE_LOCALE;
  else {
    data->settings.format.time_style = TIME_STYLE_FORMAT;
    data->settings.format.time_format = opt->value + 1;
  }
}

bool io_describe_format_time_style(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "time-style",
    "time/date format with -l; see TIME_STYLE below",
    true
  );
  if (!opt)
    return false;
  opt->set_cb((t_cli_option_cb)cb)
    ->set_type(CLI_OPTION_SELECT)
    ->set_default_value("locale")
    ->set_flags(CLI_OPTION_FLAG_REQUIRED)
    ->add_flags(CLI_OPTION_FLAG_FUZZY)
    ->set_variable_hint("TIME_STYLE")
    ->set_description_footer("\
The TIME_STYLE argument can be full-iso, long-iso, iso, locale, or +FORMAT.\r\n\
FORMAT is interpreted like in date(1).  If FORMAT is FORMAT1<newline>FORMAT2,\
 then FORMAT1 applies to non-recent files and FORMAT2 to recent files.\r\n\
TIME_STYLE prefixed with 'posix-' takes effect only outside the POSIX locale.\r\n\
Also the TIME_STYLE environment variable sets the default style to use.\r\n\
");
  for (uint32_t i = 0; time_style_options[i]; i++)
  {
    char** aliases = ft_split(time_style_options[i], ",");
    opt->add_choice(aliases[0], aliases);
    ft_split_free(aliases);
  }
  opt->end();
  return cli->is_valid();
}
