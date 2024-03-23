/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 21:05:32 by martiper          #+#    #+#             */
/*   Updated: 2024/03/23 23:05:40 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"

static const char* block_size_options[] = {
  "si",
  "human-readable",
  "kB",
  "k,K,KiB",
  "MB",
  "M,MiB",
  "GB",
  "G,GiB",
  "TB",
  "T,TiB",
  "PB",
  "P,PiB",
  "EB",
  "E,EiB",
  NULL
};

bool io_describe_block_size(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "block_size",
    "with -l, scale sizes by SIZE when printing them; e.g., '--block-size=M'; see SIZE format below",
    false
  );
  if (!opt)
    return false;
  opt->add_flag("block-size")
    ->set_variable_hint("SIZE")
    ->set_type(CLI_OPTION_SELECT)
    ->set_flags(CLI_OPTION_FLAG_REQUIRED);
  for (uint32_t i = 0; block_size_options[i]; i++)
  {
    char** aliases = ft_split(block_size_options[i], ",");
    opt->add_choice(aliases[0], aliases);
    ft_split_free(aliases);
  }
  opt->end();
  return cli->is_valid();
}
