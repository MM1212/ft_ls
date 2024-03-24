/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 21:05:32 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 15:48:09 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>

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

static void parse_block_size(t_cli_option* opt, t_ft_ls* data)
{
  char* block_size = opt->value;
  if (ft_strcmp(block_size, "si") == 0)
    data->settings.format.block_size = BLOCK_SIZE_SI;
  else if (ft_strcmp(block_size, "human-readable") == 0)
    data->settings.format.block_size = BLOCK_SIZE_HUMAN_READABLE;
  else if (ft_strcmp(block_size, "kB") == 0)
    data->settings.format.block_size = BLOCK_SIZE_KILOBYTES;
  else if (ft_strcmp(block_size, "k") == 0)
    data->settings.format.block_size = BLOCK_SIZE_KIBIBYTES;
  else if (ft_strcmp(block_size, "MB") == 0)
    data->settings.format.block_size = BLOCK_SIZE_MEGABYTES;
  else if (ft_strcmp(block_size, "M") == 0)
    data->settings.format.block_size = BLOCK_SIZE_MEBIBYTES;
  else if (ft_strcmp(block_size, "GB") == 0)
    data->settings.format.block_size = BLOCK_SIZE_GIGABYTES;
  else if (ft_strcmp(block_size, "G") == 0)
    data->settings.format.block_size = BLOCK_SIZE_GIBIBYTES;
  else if (ft_strcmp(block_size, "TB") == 0)
    data->settings.format.block_size = BLOCK_SIZE_TERABYTES;
  else if (ft_strcmp(block_size, "T") == 0)
    data->settings.format.block_size = BLOCK_SIZE_TEBIBYTES;
  else if (ft_strcmp(block_size, "PB") == 0)
    data->settings.format.block_size = BLOCK_SIZE_PETABYTES;
  else if (ft_strcmp(block_size, "P") == 0)
    data->settings.format.block_size = BLOCK_SIZE_PEBIBYTES;
  else if (ft_strcmp(block_size, "EB") == 0)
    data->settings.format.block_size = BLOCK_SIZE_EXABYTES;
  else if (ft_strcmp(block_size, "E") == 0)
    data->settings.format.block_size = BLOCK_SIZE_EXBIBYTES;
}

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
    ->set_flags(CLI_OPTION_FLAG_REQUIRED)
    ->set_cb((t_cli_option_cb)parse_block_size)
    ->set_description_footer("\
The SIZE argument is an integer and optional unit (example: 10K is 10*1024). \
Units are K,M,G,T,P,E,Z,Y (powers of 1024) or KB,MB,... (powers of 1000). \
Binary prefixes can be used, too: KiB=K, MiB=M, and so on. \
");
  for (uint32_t i = 0; block_size_options[i]; i++)
  {
    char** aliases = ft_split(block_size_options[i], ",");
    opt->add_choice(aliases[0], aliases);
    ft_split_free(aliases);
  }
  opt->end();
  return cli->is_valid();
}
