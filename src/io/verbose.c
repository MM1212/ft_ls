/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verbose.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:14:33 by martiper          #+#    #+#             */
/*   Updated: 2024/03/25 22:49:59 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"
#include <colors.h>
#include <utils.h>

#ifndef VERSION
# define VERSION "N/A"
#endif

#define HELP_SWITCHES_SIZE 30
#define HELP_DESCRIPTION_SIZE 50
#define HELP_FOOTER_SIZE 76

static void print_version(t_cli_option* opt) {
  (void)opt;
  ft_printf("ft_ls v%s\n", VERSION);
  opt->_handle->settings.should_exit = true;
  opt->_handle->settings.run_cb_only_once = true;
}

static bool add_version(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(
    "version",
    "output version information and exit",
    true
  );
  if (opt == NULL)
    return false;
  opt->set_cb((t_cli_option_cb)print_version);
  return opt->end() != NULL;
}

static void help_print_option_description(char* desc, size_t size, bool pad) {
  desc = ft_strtrim(desc, " \n\t\v\f\r");
  if (!desc)
    return;
  char* whitespace;
  while ((whitespace = ft_strchr(desc, '\1')))
    *whitespace = ' ';
  size_t len = ft_strlen(desc);
  if (len <= size) {
    ft_printf("%s\n", desc);
    free(desc);
    return;
  }
  size_t idx = len - 1;
  if (idx > size)
    idx = size;
  char* new_line;
  if ((new_line = ft_strchr(desc, '\n')) && new_line < desc + size)
    idx = new_line - desc;
  if (new_line)
    idx--;
  bool dont_indent_next = new_line && (new_line - desc) > 1 && *(new_line - 1) == '\r';
  if (dont_indent_next)
    idx--;
  size_t last_space = idx;
  while (last_space > 0 && !ft_isspace(desc[last_space]))
      last_space--;
  size_t post_space = idx;
  while (post_space < len && !ft_isspace(desc[post_space]))
      post_space++;
  if ((post_space - idx) < (idx - last_space))
    idx = post_space;
  else
    idx = last_space;
  if (idx == 0)
    idx = size;
  ft_printf("%.*s\n", idx, desc);
  char* next = desc + idx;
  if (*desc == '\0')
  {
    free(desc);
    return;
  }
  if (pad)
    ft_printf("%*s", HELP_SWITCHES_SIZE + (!dont_indent_next * 2), "");
  help_print_option_description(next, size, pad);
  free(desc);
}

static bool help_print_option(t_cli_option* opt) {
  char* switches_and_flags = NULL;
  bool output_switches = false;
  for (uint32_t i = 0; i < opt->_switches_size; i++) {
    output_switches = true;
    char switch_value[] = { opt->_switches[i].letter, '\0' };
    char* tmp = ft_strjoin(
      4,
      switches_and_flags,
      switches_and_flags != NULL ? ", " : "",
      "-", switch_value
    );
    free(switches_and_flags);
    if (tmp == NULL)
      return false;
    switches_and_flags = tmp;
  }
  for (uint32_t i = 0; i < opt->_flags_size; i++) {
    char* tmp = ft_strjoin(
      4,
      switches_and_flags,
      switches_and_flags != NULL ? ", " : "",
      "--", opt->_flags[i].name
    );
    free(switches_and_flags);
    if (tmp == NULL)
      return false;
    switches_and_flags = tmp;
    char value[256];
    ft_bzero(value, sizeof(value));
    if (opt->flags & CLI_OPTION_FLAG_REQUIRED)
      ft_sprintf(value, 256, "=%s", opt->variable_hint ? opt->variable_hint : "WORD");
    else if (opt->flags & CLI_OPTION_FLAG_OPTIONAL)
      ft_sprintf(value, 256, "[=%s]", opt->variable_hint ? opt->variable_hint : "WORD");
    tmp = ft_strjoin2((char* []) { switches_and_flags, value, NULL });
    free(switches_and_flags);
    if (tmp == NULL)
      return false;
    switches_and_flags = tmp;
  }
  if (opt->aliases != NULL) {
    // aliases
    char** aliases = ft_split(opt->aliases, ",");
    if (aliases == NULL)
    {
      free(switches_and_flags);
      return false;
    }
    for (uint32_t i = 0; aliases[i]; i++) {
      char* tmp = ft_strjoin(
        3,
        switches_and_flags,
        switches_and_flags != NULL ? ", " : "",
        aliases[i]
      );
      free(switches_and_flags);
      if (tmp == NULL) {
        ft_split_free(aliases);
        return false;
      }
      switches_and_flags = tmp;
    }
    ft_split_free(aliases);
  }
  size_t len = ft_printf("%*s%-*s", !output_switches ? 6 : 2, "", HELP_SWITCHES_SIZE - (output_switches ? 2 : 6), switches_and_flags);
  free(switches_and_flags);
  if (len > HELP_SWITCHES_SIZE)
    ft_printf("\n%*s", HELP_SWITCHES_SIZE, "");
  else if (ft_strlen(switches_and_flags) == HELP_SWITCHES_SIZE)
    ft_printf("  ");
  help_print_option_description(opt->description, HELP_DESCRIPTION_SIZE, true);
  return true;
}

static void help_print_desc_footer(char* desc) {
  desc = ft_strtrim(desc, " \n\t\v\f\r");
  if (!desc)
    return;
  help_print_option_description(desc, HELP_FOOTER_SIZE, false);
  ft_printf("\n");
  free(desc);
}

static void print_help(t_cli_option* opt) {
  ft_printf("Usage: ft_ls [OPTION]... [FILE]...\n");
  ft_printf("List information about the FILEs (the current directory by default).\n");
  ft_printf("Sort entries alphabetically if none of -cftuvSUX nor --sort is specified.\n");
  ft_printf("\n");
  ft_printf("Mandatory arguments to long options are mandatory for short options too.\n");
  ft_printf("%d available options:\n", opt->_handle->options_size);
  for (uint32_t i = 0; i < opt->_handle->options_size; i++)
    help_print_option(&opt->_handle->options[i]);
  ft_printf("\n");
  for (uint32_t i = 0; i < opt->_handle->options_size; i++)
    if (opt->_handle->options[i].description_footer != NULL)
      help_print_desc_footer(opt->_handle->options[i].description_footer);
  opt->_handle->settings.should_exit = true;
  opt->_handle->settings.run_cb_only_once = true;
}

static bool add_help(t_cli_handle* cli) {
  t_cli_option_builder* opt = cli->new_option(
    "help",
    "display this help and exit",
    true
  );
  if (opt == NULL)
    return false;
  opt->set_cb((t_cli_option_cb)print_help);
  return opt->end() != NULL;
}

bool io_describe_verbose_options(t_cli_handle* cli) {
  if (!add_version(cli))
    return false;
  if (!add_help(cli))
    return false;
  return true;
}
