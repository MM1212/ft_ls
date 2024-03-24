/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 22:37:51 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static const char* color_options[] = {
  "always,yes,force",
  "never,no,none",
  "auto,tty,if-tty",
  NULL
};



static void cb(t_cli_option* opt, t_ft_ls* data) {
  if (ft_strcmp(opt->value, "always") == 0)
    data->settings.display.color = SETTINGS_COLOR_ALWAYS;
  else if (ft_strcmp(opt->value, "never") == 0)
    data->settings.display.color = SETTINGS_COLOR_NEVER;
  else if (ft_strcmp(opt->value, "auto") == 0)
    data->settings.display.color = SETTINGS_COLOR_AUTO;
}

bool io_describe_display_color(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "color",
    "colorize the output; WHEN can be 'always' (default if omitted), 'auto', or 'never'; more info below",
    true
  );
  if (!opt)
    return false;
  opt->set_cb((t_cli_option_cb)cb)
    ->set_type(CLI_OPTION_SELECT)
    ->set_flags(CLI_OPTION_FLAG_OPTIONAL)
    ->set_variable_hint("WHEN")
    ->set_description_footer("\
Using color to distinguish file types is disabled both by default and \
with --color=never.  With --color=auto, ls emits color codes only when \
standard output is connected to a terminal.  The LS_COLORS environment \
variable can change the settings.  Use the dircolors command to set it.\
");
  for (uint32_t i = 0; color_options[i]; i++)
  {
    char** aliases = ft_split(color_options[i], ",");
    opt->add_choice(aliases[0], aliases);
    ft_split_free(aliases);
  }
  opt->end();
  return cli->is_valid();
}
