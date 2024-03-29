/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 14:31:12 by martiper          #+#    #+#             */
/*   Updated: 2024/03/29 22:05:10 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.h"
#include <stdlib.h>
#include <libft.h>

struct s_default_color {
  char* key;
  char* code;
};

static const struct s_default_color default_color_indicators[] =
{
  { "di", "01;34" },		/* di: Directory: bright blue */
  { "ln", "01;36" },		/* ln: Symlink: bright cyan */
  { "pi", "33" },		/* pi: Pipe: yellow/brown */
  { "so", "01;35" },		/* so: Socket: bright magenta */
  { "bd", "01;33" },		/* bd: Block device: bright yellow */
  { "cd", "01;33" },		/* cd: Char device: bright yellow */
  { "ex", "01;32" },		/* ex: Executable: bright green */
  { "do", "01;35" },		/* do: Door: bright magenta */
  { "su", "37;41" },		/* su: setuid: white on red */
  { "sg", "30;43" },		/* sg: setgid: black on yellow */
  { "st", "37;44" },		/* st: sticky: black on blue */
  { "ow", "34;42" },		/* ow: other-writable: blue on green */
  { "tw", "30;42" },		/* tw: ow w/ sticky: black on green */
  { "ca", "30;41" },		/* ca: file with capability: black on red */
  { "mh", "01;35" },		/* mh: hardlink: bright magenta */
  { "mi", "01;31" },		/* mi: missing file: bright red */
  { "no", "0" },		/* no: Normal */
  { "rs", "0" },		/* rs: Reset */
  { "lc", "\33[" }, /* lc: Left color */
  { "rc", "m" } /* rc: Right color */
};
static const size_t color_indicator_size = sizeof(default_color_indicators) / sizeof(default_color_indicators[0]);


struct s_common_color {
  char* key;
  char* value;
};

static struct s_common_color common_colors[] = {
  { "no", NULL }, // COLOR_NORM
  { "fi", NULL }, // COLOR_FILE
  { "di", NULL }, // COLOR_DIR
  { "ln", NULL }, // COLOR_LINK
  { "pi", NULL }, // COLOR_FIFO
  { "do", NULL }, // COLOR_DOOR
  { "bd", NULL }, // COLOR_BLOCK
  { "cd", NULL }, // COLOR_CHAR
  { "or", NULL }, // COLOR_ORPHAN
  { "so", NULL }, // COLOR_SOCK
  { "su", NULL }, // COLOR_SETUID
  { "sg", NULL }, // COLOR_SETGID
  { "tw", NULL }, // COLOR_STICKY_OTHER_WRITABLE
  { "ow", NULL }, // COLOR_OTHER_WRITABLE
  { "st", NULL }, // COLOR_STICKY
  { "ex", NULL }, // COLOR_EXEC
  { "mi", NULL }, // COLOR_MISSING
  { "ca", NULL }, // COLOR_CAPABILITY
  { "mh", NULL }, // COLOR_MULTIHARDLINK
  { "rs", NULL }, // COLOR_RESET
  { "lc", NULL }, // COLOR_LEFT
  { "rc", NULL } // COLOR_RIGHT
};

static void build_common_colors(t_colors_registry* reg) {
  size_t common_colors_amount = sizeof(common_colors) / sizeof(common_colors[0]);
  for (size_t i = 0; i < common_colors_amount; i++) {
    common_colors[i].value = reg->get(reg, common_colors[i].key);
  }
}

t_colors_registry* colors_registry_create(char** env)
{
  t_colors_registry* rt = hashtable_create(\
    256, (t_hashtable_hash)hashtable_joaat_hash, \
    (t_hashtable_cmp)ft_strcmp, \
    NULL \
  );
  if (!rt)
    return NULL;
  char* ls_colors = NULL;
  for (size_t i = 0; env[i]; i++) {
    if (ft_strncmp(env[i], "LS_COLORS=", 10) == 0) {
      ls_colors = env[i] + 10;
      break;
    }
  }
  if (ls_colors) {
    char* cursor = ls_colors;
    while (*cursor) {
      char* key = cursor;
      char* value = ft_strchr(cursor, '=');
      if (!value)
        break;
      *value = '\0';
      value++;
      cursor = value;
      while (*cursor && *cursor != ':')
        cursor++;
      *cursor = '\0';
      cursor++;
      rt->add(rt, key, value);
    }
  }
  for (size_t i = 0; i < color_indicator_size; i++)
    rt->add(rt, default_color_indicators[i].key, default_color_indicators[i].code);
  build_common_colors(rt);
  return rt;
}

static bool find_match(const char* key, const char* value, const char* search) {
  (void)value;
  return *key == '*' && ft_strcmp(key + 1, search) == 0;
}

# define GET_COMMON_COLOR(id) (common_colors[id].value)
# define IS_COLORED(id) (GET_COMMON_COLOR(id) != NULL)

static char* get_file_special_colors(t_file* file) {
  mode_t mode = file->stat.st_mode;

  if (file->type == FILE_DIRECTORY) {
    if (!file_stat(file))
      return NULL;
    if (mode & S_ISVTX && mode & S_IWOTH && IS_COLORED(COLOR_STICKY_OTHER_WRITABLE))
      return GET_COMMON_COLOR(COLOR_STICKY_OTHER_WRITABLE);
    else if (mode & S_IWOTH && IS_COLORED(COLOR_STICKY_OTHER_WRITABLE))
      return GET_COMMON_COLOR(COLOR_OTHER_WRITABLE);
    else if (mode & S_ISVTX && IS_COLORED(COLOR_STICKY_OTHER_WRITABLE))
      return GET_COMMON_COLOR(COLOR_STICKY);
    return GET_COMMON_COLOR(COLOR_DIR);
  }
  else if (file->type == FILE_REGULAR) {
    if (!file_stat(file))
      return NULL;
    else if (mode & S_ISUID && IS_COLORED(COLOR_SETUID))
      return GET_COMMON_COLOR(COLOR_SETUID);
    else if (mode & S_ISGID && IS_COLORED(COLOR_SETUID))
      return GET_COMMON_COLOR(COLOR_SETGID);
    else if (is_file_executable(file) && IS_COLORED(COLOR_SETUID))
      return GET_COMMON_COLOR(COLOR_EXEC);
    else if (file->stat.st_nlink >= 1 && IS_COLORED(COLOR_SETUID))
      return GET_COMMON_COLOR(COLOR_MULTIHARDLINK);
    return GET_COMMON_COLOR(COLOR_FILE);
  }
  return NULL;
}

static char* choose_color(
  t_file* file,
  t_colors_registry* reg,
  t_settings* settings
) {
  if (
    settings->display.color == SETTINGS_COLOR_NEVER ||
    (settings->display.color == SETTINGS_COLOR_AUTO && !settings->is_tty))
    return NULL;

  switch (file->type) {
  case FILE_BLOCK_SPECIAL: return GET_COMMON_COLOR(COLOR_BLOCK);
  case FILE_CHARACTER_SPECIAL: return GET_COMMON_COLOR(COLOR_CHAR);
  case FILE_DIRECTORY: return get_file_special_colors(file);
  case FILE_SYMLINK: {
    if (!file->symlinkd)
      return GET_COMMON_COLOR(COLOR_ORPHAN);
    return GET_COMMON_COLOR(COLOR_LINK);
  };
  case FILE_FIFO: return GET_COMMON_COLOR(COLOR_FIFO);
  case FILE_SOCKET: return GET_COMMON_COLOR(COLOR_SOCK);
  default: break;
  }
  char* special_color = get_file_special_colors(file);
  if (special_color)
    return special_color;

  t_hashtable_item* search;
  char* ext = ft_strrchr(file->name, '.');
  if (!ext)
    ext = file->name;
  if ((search = reg->find(reg, (t_hashtable_find)find_match, ext)))
    return search->value;
  return GET_COMMON_COLOR(COLOR_NORM);
}

char* get_color_for_file(
  t_file* file,
  t_colors_registry* reg,
  t_settings* settings
) {
  static char buf[256];
  char* color = choose_color(file, reg, settings);
  if (!color)
    return NULL;
  ft_sprintf(buf, 256, "%s%s%s", GET_COMMON_COLOR(COLOR_LEFTCODE), color, GET_COMMON_COLOR(COLOR_RIGHTCODE));
  return buf;
}

char* get_color_for_symlink(
  t_file* file,
  t_colors_registry* reg,
  t_settings* settings
) {
  if (!file->symlinkd)
    return NULL;
  t_file link;

  if (!file_from_symlink_view(&link, file))
    return GET_COMMON_COLOR(COLOR_ORPHAN);
  return get_color_for_file(&link, reg, settings);
}
