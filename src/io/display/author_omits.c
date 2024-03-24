/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   author_omits.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 23:08:28 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 22:48:03 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

static void author_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.display.author = true;
}

static void omit_group_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.display.omit_group = true;
}

static void long_omit_group_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.display.omit_group = true;
  data->settings.format.type = FORMAT_LONG;
}

static void omit_owner_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.display.omit_owner = true;
  data->settings.format.type = FORMAT_LONG;
}

static bool describe_author(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "author",
    "with -l, print the author of each file",
    true
  );
  if (!opt)
    return false;
  opt->set_cb((t_cli_option_cb)author_cb)
    ->end();
  return true;
}

static bool describe_omit_group(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "omit_group",
    "in a long listing, don't print group names",
    false
  );
  if (!opt)
    return false;
  opt->add_flag("no-group")
    ->add_switch('G')
    ->set_cb((t_cli_option_cb)omit_group_cb)
    ->end();
  return true;
}

static bool describe_long_omit_group(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "long_omit_group",
    "like -l, but do not list group information",
    false
  );
  if (!opt)
    return false;
  opt->add_switch('o')
    ->set_cb((t_cli_option_cb)long_omit_group_cb)
    ->end();
  return true;
}

static bool describe_omit_owner(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "omit_owner",
    "like -l, but do not list owner",
    false
  );
  if (!opt)
    return false;
  opt->add_switch('g')
    ->set_cb((t_cli_option_cb)omit_owner_cb)
    ->end();
  return true;
}


bool io_describe_display_omits(t_cli_handle* cli) {
  return describe_author(cli)
    && describe_omit_group(cli)
    && describe_omit_owner(cli)
    && describe_long_omit_group(cli);
}
