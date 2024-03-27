/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   indicator_style.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:15:40 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static const char* indicator_style_options[] = {
  "none",
  "slash",
  "file-type",
  "classify",
  NULL
};

static void default_cb(t_cli_option* opt, t_ft_ls* data) {
  if (ft_strcmp(opt->value, "none") == 0)
    data->settings.format.indicator_style = INDICATOR_STYLE_NONE;
  else if (ft_strcmp(opt->value, "slash") == 0)
    data->settings.format.indicator_style = INDICATOR_STYLE_SLASH;
  else if (ft_strcmp(opt->value, "file-type") == 0)
    data->settings.format.indicator_style = INDICATOR_STYLE_FILE_TYPE;
  else if (ft_strcmp(opt->value, "classify") == 0)
    data->settings.format.indicator_style = INDICATOR_STYLE_CLASSIFY;
}

static void file_type_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.format.indicator_style = INDICATOR_STYLE_FILE_TYPE;
}

static void classify_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.format.indicator_style = INDICATOR_STYLE_CLASSIFY;
}

static void slash_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.format.indicator_style = INDICATOR_STYLE_SLASH;
}

static bool describe_file_type(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "file-type",
    "likewise, except do not append '*'",
    true
  );
  if (!opt)
    return false;
  opt->set_cb((t_cli_option_cb)file_type_cb)
    ->end();
  return true;
}

static bool describe_classify(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "classify",
    "append indicator (one of */=>@|) to entries",
    true
  );
  if (!opt)
    return false;
  opt->add_switch('F')
    ->set_cb((t_cli_option_cb)classify_cb)
    ->end();
  return true;
}

static bool describe_slash(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "slash",
    "append / indicator to directories",
    false
  );
  if (!opt)
    return false;
  opt->add_switch('p')
    ->set_cb((t_cli_option_cb)slash_cb)
    ->end();
  return true;
}

bool io_describe_format_indicator_style(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "indicator-style",
    "append indicator with style WORD to entry names: \
none (default), slash (-p), \
file-type (--file-type), classify (-F)",
true
);
  if (!opt)
    return false;
  opt->set_cb((t_cli_option_cb)default_cb)
    ->set_type(CLI_OPTION_SELECT)
    ->set_flags(CLI_OPTION_FLAG_REQUIRED);
  for (uint32_t i = 0; indicator_style_options[i]; i++)
  {
    char** aliases = ft_split(indicator_style_options[i], ",");
    opt->add_choice(aliases[0], aliases);
    ft_split_free(aliases);
  }
  opt->end();
  if (!describe_classify(cli))
    return false;
  if (!describe_file_type(cli))
    return false;
  if (!describe_slash(cli))
    return false;
  return cli->is_valid();
}
