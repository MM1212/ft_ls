/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ignore.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:28:29 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void cb(t_cli_option* opt, t_ft_ls* data) {
  data->settings.filter.ignore_pattern = opt->value;
}

static void ignore_backups_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.filter.ignore_backups = true;
}

bool io_describe_filter_ignore(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "ignore",
    "do not list implied entries matching shell PATTERN; supported glob patterns: *, ?",
    true
  );
  if (!opt)
    return false;
  opt->add_switch('I')
    ->set_flags(CLI_OPTION_FLAG_OPTIONAL)
    ->set_cb((t_cli_option_cb)cb)
    ->end();
  return cli->is_valid();
}

bool io_describe_filter_ignore_backups(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "ignore_backups",
    "do not list implied entries ending with ~",
    false
  );
  if (!opt)
    return false;
  opt->add_flag("ignore-backups")
    ->add_switch('B')
    ->set_cb((t_cli_option_cb)ignore_backups_cb)
    ->end();
  return cli->is_valid();
}
