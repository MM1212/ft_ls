/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 11:26:12 by martiper          #+#    #+#             */
/*   Updated: 2024/03/23 16:11:19 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct s_settings t_settings;
typedef struct s_settings_filter t_settings_filter;
typedef struct s_settings_display t_settings_display;
typedef struct s_settings_sort t_settings_sort;
typedef struct s_settings_format t_settings_format;

typedef enum e_settings_block_size t_settings_block_size;
typedef enum e_settings_time t_settings_time;
typedef enum e_settings_sort_type t_settings_sort_type;
typedef enum e_settings_format_type t_settings_format_type;
typedef enum e_settings_indicator_style t_settings_indicator_style;

struct s_settings_filter {
  bool all;
  bool almost_all;
  bool ignore_backups;
  bool list_directories;
  bool dereference_links_cli;
  bool dereference_links;
  bool recursive;
};
/*
  Options:

 */


enum e_settings_block_size {
  BLOCK_SIZE_BYTES,
  BLOCK_SIZE_HUMAN_READABLE,
  BLOCK_SIZE_SI,
  BLOCK_SIZE_KILOBYTES,
  BLOCK_SIZE_KIBIBYTES,
  BLOCK_SIZE_MEGABYTES,
  BLOCK_SIZE_MEBIBYTES,
  BLOCK_SIZE_GIGABYTES,
  BLOCK_SIZE_GIBIBYTES,
  BLOCK_SIZE_TERABYTES,
  BLOCK_SIZE_TEBIBYTES,
  BLOCK_SIZE_PETABYTES,
  BLOCK_SIZE_PEBIBYTES,
  BLOCK_SIZE_EXABYTES,
  BLOCK_SIZE_EXBIBYTES,
  BLOCK_SIZE_ZETTABYTES,
  BLOCK_SIZE_ZEBIBYTES,
};

enum e_settings_time {
  TIME_MODIFIED,
  TIME_STATUS_CHANGED,
  TIME_ACCESSED,
  TIME_CREATED
};

struct s_settings_display {
  bool author;
  bool omit_owner;
  bool omit_group;
  bool inode;
  bool numeric_ids;
  bool size;
};

/*
  Options:
  --author [author]: with -l, print the author of each file


*/

enum e_settings_sort_type {
  SORT_NONE,
  SORT_TIME,
  SORT_SIZE,
  SORT_EXTENSION,
  SORT_VERSION,
  SORT_WIDTH
};

struct s_settings_sort {
  t_settings_sort_type type;
  bool reverse;
};

enum e_settings_format_type {
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

struct s_settings_format {
  t_settings_format_type type;
  t_settings_block_size block_size;
  t_settings_time time;
  t_settings_indicator_style indicator_style;
};

struct s_settings {
  t_settings_filter filter;
  t_settings_display display;
  t_settings_sort sort;
  t_settings_format format;
};
