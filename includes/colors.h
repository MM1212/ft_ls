/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 14:36:05 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:30:37 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <lxt/dst/hashtable.h>
#include <file.h>
#include <settings.h>

#ifndef COLORS_RED
# define COLORS_RED ""
#endif

#ifndef COLORS_GREEN
# define COLORS_GREEN ""
#endif

#ifndef COLORS_YELLOW
# define COLORS_YELLOW ""
#endif

#ifndef COLORS_BLUE
# define COLORS_BLUE ""
#endif

#ifndef COLORS_MAGENTA
# define COLORS_MAGENTA ""
#endif

#ifndef COLORS_CYAN
# define COLORS_CYAN ""
#endif

#ifndef COLORS_ORANGE
# define COLORS_ORANGE ""
#endif

#ifndef COLORS_RESET
# define COLORS_RESET ""
#endif

typedef t_hashtable t_colors_registry;
typedef enum e_color_codes t_color_codes;

enum e_color_codes
{
  COLOR_NORM,/* =no */
  COLOR_FILE, /* =fi */
  COLOR_DIR, /* =di */
  COLOR_LINK, /* =ln */
  COLOR_FIFO, /* =pi */
  COLOR_DOOR, /* =do */
  COLOR_BLOCK, /* =bd */
  COLOR_CHAR, /* =cd */
  COLOR_ORPHAN, /* =or */
  COLOR_SOCK, /* =so */
  COLOR_SETUID, /* =su */
  COLOR_SETGID, /* =sg */
  COLOR_STICKY_OTHER_WRITABLE, /* =tw */
  COLOR_OTHER_WRITABLE, /* =ow */
  COLOR_STICKY, /* =st */
  COLOR_EXEC, /* =ex */
  COLOR_MISSING, /* =mi */
  COLOR_LEFT, /* =lc */
  COLOR_RIGHT, /* =rc */
  COLOR_END, /* =ec */
  COLOR_EXT /* =*.ext */
};

t_colors_registry* colors_registry_create(char** env);
char* get_color_for_file(
  t_file* file,
  t_colors_registry* colors,
  t_settings* settings
);
char* get_color_for_symlink(
  t_file* file,
  t_colors_registry* reg,
  t_settings* settings
);
