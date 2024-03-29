/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scontext.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 17:04:59 by martiper          #+#    #+#             */
/*   Updated: 2024/03/29 18:06:19 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file.h"
#include <ft_ls.h>

#define ACL_KEYS "system.posix_acl_default:system.posix_acl_access:system.nfs4_acl"

void file_setup_security_context(t_file* file) {
  if (!g_ls_data->settings.display.scontext && g_ls_data->settings.format.type != FORMAT_LONG)
    return;
  if (file->type == FILE_SYMLINK)
    return;
  ssize_t buflen = listxattr(file->full_path, NULL, 0);
  if (buflen <= 0)
    return;
  char list[buflen + 1];
  char* p = list;
  buflen = listxattr(file->full_path, list, buflen);
  if (buflen <= 0)
    return;
  list[buflen] = '\0';
  file->acl = ACL_SCTX;
  // ft_printf("[%s] list: %s\n", file->name, list);
  while (*p) {
    char* key = p;
    if (ft_strnstr(ACL_KEYS, key, sizeof(ACL_KEYS)) != 0) {
      p += ft_strlen(p) + 1;
      file->acl = ACL_YES;
      continue;
    }
    size_t value_len = getxattr(file->full_path, key, NULL, 0);
    if (value_len <= 0) {
      p += ft_strlen(p) + 1;
      continue;
    }
    char value[value_len + 1];
    if (getxattr(file->full_path, key, value, value_len) < 0) {
      p += ft_strlen(p) + 1;
      continue;
    }
    value[value_len] = '\0';
    if (!file->scontext)
      file->scontext = ft_strdup(value);
    else
      file->scontext = ft_strjoin(2, file->scontext, value);
    p += ft_strlen(p) + 1;
  }
}
