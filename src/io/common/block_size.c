/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 21:05:32 by martiper          #+#    #+#             */
/*   Updated: 2024/03/31 16:10:14 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <ft_ls.h>
#include <block_size.h>

static const char* block_size_options[] = {
  "si",
  "human-readable",
  "*kB",
  "*k,*K,*KiB",
  "*MB",
  "*M,*MiB",
  "*GB",
  "*G,*GiB",
  "*TB",
  "*T,*TiB",
  "*PB",
  "*P,*PiB",
  "*EB",
  "*E,*EiB",
  NULL
};
static const char* block_size_first_set = "kKMGTPE";

static bool validate(t_cli_option* opt, char* arg) {
  char* unit = arg;
  while (*unit && !ft_strchr(block_size_first_set, *unit))
    unit++;
  if (!*unit)
    return false;
  char store = *unit;
  *unit = '\0';
  size_t scalar = 0;
  if (unit != arg && (!ft_isnbr(arg, false, false) || (scalar = ft_atol(arg)) == 0) ) {
    *unit = store;
    return false;
  }
  *unit = store;
  const t_power* power = match_power(unit);
  if (power == NULL)
    return false;
  opt->data = (void*)power;
  t_ft_ls* main = opt->_handle->data;
  main->settings.format.block_size_scalar = scalar;
  main->settings.format.block_size_one_letter = ft_strlen(unit) == 1;
  return true;
}

static void parse_block_size(t_cli_option* opt, t_ft_ls* data) {
  data->settings.format.block_size_power = ((const t_power*)opt->data);
  data->settings.format.block_size = data->settings.format.block_size_power->unit;
}

static void human_readable_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.format.block_size = BLOCK_SIZE_HUMAN_READABLE;
}

static void si_cb(t_cli_option* opt, t_ft_ls* data) {
  (void)opt;
  data->settings.format.block_size = BLOCK_SIZE_SI;
}

static bool describe_human_readable(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "human-readable",
    "with -l and -s, print sizes like 1K 234M 2G etc.",
    true
  );
  if (!opt)
    return false;
  opt->add_switch('h')->set_cb((t_cli_option_cb)human_readable_cb);
  return opt->end();
}

static bool describe_si(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(\
    "si",
    "likewise, but use powers of 1000 not 1024",
    true
  );
  if (!opt)
    return false;
  opt->set_cb((t_cli_option_cb)si_cb);
  return opt->end();
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
    ->set_flags(CLI_OPTION_FLAG_REQUIRED | CLI_OPTION_FLAG_FUZZY)
    ->set_cb((t_cli_option_cb)parse_block_size)
    ->set_custom_validator((t_cli_option_validator)validate)
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
  return describe_human_readable(cli) && describe_si(cli);
}
