/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 14:31:12 by martiper          #+#    #+#             */
/*   Updated: 2024/03/29 15:21:05 by martiper         ###   ########.fr       */
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
  { "di", "\33[01;34m" },		/* di: Directory: bright blue */
  { "ln", "\33[01;36m" },		/* ln: Symlink: bright cyan */
  { "pi", "\33[33m" },		/* pi: Pipe: yellow/brown */
  { "so", "\33[01;35m" },		/* so: Socket: bright magenta */
  { "bd", "\33[01;33m" },		/* bd: Block device: bright yellow */
  { "cd", "\33[01;33m" },		/* cd: Char device: bright yellow */
  { "ex", "\33[01;32m" },		/* ex: Executable: bright green */
  { "do", "\33[01;35m" },		/* do: Door: bright magenta */
  { "su", "\33[37;41m" },		/* su: setuid: white on red */
  { "sg", "\33[30;43m" },		/* sg: setgid: black on yellow */
  { "st", "\33[37;44m" },		/* st: sticky: black on blue */
  { "ow", "\33[34;42m" },		/* ow: other-writable: blue on green */
  { "tw", "\33[30;42m" },		/* tw: ow w/ sticky: black on green */
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
};

static void build_common_colors(t_colors_registry* reg) {
  size_t common_colors_amount = sizeof(common_colors) / sizeof(common_colors[0]);
  for (size_t i = 0; i < common_colors_amount; i++) {
    common_colors[i].value = reg->get(reg, common_colors[i].key);
  }
}

static void double_free(void* a, void* b) {
  free(a);
  free(b);
}

t_colors_registry* colors_registry_create(char** env)
{
  char* ls_colors = NULL;
  for (size_t i = 0; env[i]; i++) {
    if (ft_strncmp(env[i], "LS_COLORS=", 10) == 0) {
      ls_colors = env[i] + 10;
      break;
    }
  }
  if (!ls_colors)
    return NULL;
  t_colors_registry* rt = hashtable_create(\
    256, (t_hashtable_hash)hashtable_joaat_hash, \
    (t_hashtable_cmp)ft_strcmp, \
    (t_hashtable_delete)double_free \
  );
  if (!rt)
    return NULL;
  char** colors = ft_split(ls_colors, ":");
  if (!colors) {
    rt->destroy(rt);
    return NULL;
  }
  for (size_t i = 0; colors[i]; i++) {
    char** color = ft_split(colors[i], "=");
    if (!color) {
      rt->destroy(rt);
      ft_split_free(colors);
      return NULL;
    }
    char* code = ft_strjoin(3, "\033[", color[1], "m");
    if (!code) {
      rt->destroy(rt);
      ft_split_free(color);
      ft_split_free(colors);
      return NULL;
    }
    free(color[1]);
    rt->add(rt, color[0], code);
    free(color);
  }
  ft_split_free(colors);
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

char* get_color_for_file(
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
