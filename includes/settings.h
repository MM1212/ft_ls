/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 11:26:12 by martiper          #+#    #+#             */
/*   Updated: 2024/03/31 15:51:28 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <file.h>
#include <stdbool.h>
#include <stdint.h>
#include <block_size.h>

#define DEFAULT_TERMINAL_WIDTH 80

typedef struct s_settings t_settings;
typedef struct s_settings_filter t_settings_filter;
typedef struct s_settings_display t_settings_display;
typedef struct s_settings_sort t_settings_sort;
typedef struct s_settings_format t_settings_format;

typedef enum e_settings_color t_settings_color;
typedef enum e_settings_time t_settings_time;
typedef enum e_settings_sort_type t_settings_sort_type;
typedef enum e_settings_format_type t_settings_format_type;
typedef enum e_settings_indicator_style t_settings_indicator_style;
typedef enum e_settings_time_style t_settings_time_style;

typedef int (*t_settings_sort_cmp)(t_file*, t_file*, t_settings*);

struct s_settings_filter {
  bool all;
  bool almost_all;
  bool ignore_backups;
  bool list_directories;
  bool dereference_links_cli;
  bool dereference_links;
  bool recursive;
  char* ignore_pattern;
};
/*
  Options:

 */


enum e_settings_time {
  TIME_MODIFIED,
  TIME_STATUS_CHANGED,
  TIME_ACCESSED,
  TIME_CREATED
};

enum e_settings_color {
  SETTINGS_COLOR_AUTO,
  SETTINGS_COLOR_ALWAYS,
  SETTINGS_COLOR_NEVER
};

struct s_settings_display {
  bool author;
  bool omit_owner;
  bool omit_group;
  bool inode;
  bool block_size;
  bool numeric_ids;
  bool scontext;
  t_settings_color color;
};

/*
  Options:
  --author [author]: with -l, print the author of each file


*/

enum e_settings_sort_type {
  SORT_ASCII,
  SORT_NONE,
  SORT_TIME,
  SORT_SIZE,
  SORT_VERSION
};

struct s_settings_sort {
  t_settings_sort_type type;
  bool reverse;
  t_settings_sort_cmp cmp;
};

enum e_settings_format_type {
  FORMAT_NONE,
  FORMAT_HORIZONTAL,
  FORMAT_LONG,
  FORMAT_COMMAS,
  FORMAT_VERTICAL,
  FORMAT_SINGLE_COLUMN
};

enum e_settings_indicator_style {
  INDICATOR_STYLE_NONE,
  INDICATOR_STYLE_SLASH,
  INDICATOR_STYLE_FILE_TYPE,
  INDICATOR_STYLE_CLASSIFY
};

enum e_settings_time_style {
  TIME_STYLE_LOCALE,
  TIME_STYLE_FULL,
  TIME_STYLE_LONG,
  TIME_STYLE_ISO,
  TIME_STYLE_FORMAT
};

struct s_settings_format {
  t_settings_format_type type;
  t_settings_block_size block_size;
  const t_power* block_size_power;
  bool block_size_one_letter;
  ssize_t block_size_scalar;
  t_settings_time time;
  t_settings_indicator_style indicator_style;
  t_settings_time_style time_style;
  char* time_format;
};

struct s_settings {
  t_settings_filter filter;
  t_settings_display display;
  t_settings_sort sort;
  t_settings_format format;
  bool print_dir_name;
  bool is_tty;
  ssize_t terminal_width;
  size_t tab_size;
};

void settings_print(t_settings* settings);
