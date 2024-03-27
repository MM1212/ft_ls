/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 14:31:12 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 16:56:08 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "colors.h"
#include <stdlib.h>
#include <libft.h>

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
  return rt;
}

static bool find_match(const char* key, const char* value, const char* search) {
  (void)value;
  return ft_strchr(key, '*') && ft_wildcard_match(key, search);
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
  case FILE_BLOCK_SPECIAL: return reg->get(reg, "bd");
  case FILE_CHARACTER_SPECIAL: return reg->get(reg, "cd");
  case FILE_DIRECTORY: return reg->get(reg, "di");
  case FILE_SYMLINK: {
    if (file->symlinkd && !file->symlink)
      return reg->get(reg, "or");
    return reg->get(reg, "ln");
  };
  case FILE_FIFO: return reg->get(reg, "pi");
  case FILE_SOCKET: return reg->get(reg, "so");
  default: break;
  }
  t_hashtable_item* search;
  if ((search = reg->find(reg, (t_hashtable_find)find_match, file->name)))
    return search->value;
  if (file_stat(file) && file->perms.user[2] == 'x')
    return reg->get(reg, "ex");
  else if (file->type == FILE_REGULAR)
    return reg->get(reg, "fi");
  return reg->get(reg, "no");
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
    return NULL;
  return get_color_for_file(&link, reg, settings);
}
