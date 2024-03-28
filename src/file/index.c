/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 16:18:11 by martiper          #+#    #+#             */
/*   Updated: 2024/03/28 00:21:08 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "file.h"
#include <stdint.h>
#include <libft.h>
#include <unistd.h>
#include <linux/stat.h>
#include <ft_ls.h>
#include <time.h>
#define _GNU_SOURCE
#include <string.h>

t_file_type  get_file_type_by_dirent(uint8_t flag) {
  switch (flag) {
  case DT_REG: return FILE_REGULAR;
  case DT_BLK: return FILE_BLOCK_SPECIAL;
  case DT_CHR: return FILE_CHARACTER_SPECIAL;
  case DT_DIR: return FILE_DIRECTORY;
  case DT_LNK: return FILE_SYMLINK;
  case DT_FIFO: return FILE_FIFO;
  case DT_SOCK: return FILE_SOCKET;
  default: return FILE_OTHER;
  }
}

t_file_type get_file_type_by_stat(uint32_t mode) {
  switch (mode & S_IFMT) {
  case S_IFREG: return FILE_REGULAR;
  case S_IFBLK: return FILE_BLOCK_SPECIAL;
  case S_IFCHR: return FILE_CHARACTER_SPECIAL;
  case S_IFDIR: return FILE_DIRECTORY;
  case S_IFLNK: return FILE_SYMLINK;
  case S_IFIFO: return FILE_FIFO;
  case S_IFSOCK: return FILE_SOCKET;
  default: return FILE_OTHER;
  }
}

enum e_perm_set {
  PERM_SET_USER,
  PERM_SET_GROUP,
  PERM_SET_OTHER
};

static char handle_perms_third_set(t_file* file, uint8_t mode, enum e_perm_set set) {
  (void)file;
  //  If the character is part of the owner permissions and the file is not exe‐
  //  cutable or the directory is not searchable by the owner, and the set-user-
  //  id bit is set.
  if (set == PERM_SET_USER && (mode & S_ISUID) && !(mode & S_IXUSR))
    return 'S';
  if (set == PERM_SET_GROUP && (mode & S_ISGID) && !(mode & S_IXGRP))
    return 'S';
  // If the character is part of the other permissions and the file is not exe‐
  //          cutable or the directory is not searchable by others, and the ``sticky''
  //          (S_ISVTX) bit is set.
  if (set == PERM_SET_OTHER && (mode & S_ISVTX) && !(mode & S_IXOTH))
    return 'T';
  //  If the character is part of the owner permissions and the file is exe‐
  //          cutable or the directory searchable by the owner, and the set-user-id bit
  //          is set.
  if (set == PERM_SET_USER && (mode & S_ISUID) && (mode & S_IXUSR))
    return 's';
  if (set == PERM_SET_GROUP && (mode & S_ISGID) && (mode & S_IXGRP))
    return 's';

  switch (set) {
  case PERM_SET_USER:
    return (mode & S_IXUSR) ? 'x' : '-';
  case PERM_SET_GROUP:
    return (mode & S_IXGRP) ? 'x' : '-';
  case PERM_SET_OTHER:
    return (mode & S_IXOTH) ? 'x' : '-';
  }
  return '-';
}

void file_build_permissions(t_file* file) {
  struct s_file_permissions* perms = &file->perms;
  perms->user[0] = (file->stat.st_mode & S_IRUSR) ? 'r' : '-';
  perms->user[1] = (file->stat.st_mode & S_IWUSR) ? 'w' : '-';
  perms->user[2] = handle_perms_third_set(file, file->stat.st_mode, PERM_SET_USER);
  perms->group[0] = (file->stat.st_mode & S_IRGRP) ? 'r' : '-';
  perms->group[1] = (file->stat.st_mode & S_IWGRP) ? 'w' : '-';
  perms->group[2] = handle_perms_third_set(file, file->stat.st_mode, PERM_SET_GROUP);
  perms->other[0] = (file->stat.st_mode & S_IROTH) ? 'r' : '-';
  perms->other[1] = (file->stat.st_mode & S_IWOTH) ? 'w' : '-';
  perms->other[2] = handle_perms_third_set(file, file->stat.st_mode, PERM_SET_OTHER);
}

static void file_readlink(t_file* file) {
  char buf[4098];
  ssize_t len = readlink(file->full_path, buf, 4098);
  if (len == -1) {
    ft_show_error(EXIT_MINOR, false, true, "cannot read symbolic link '%s'", file->full_path);
    return;
  }
  buf[len] = '\0';
  char* link = ft_strdup(buf);
  char* link_path = resolve_path(2, file->path, buf);
  if (!link || !link_path) {
    free(link);
    free(link_path);
    ft_show_error(EXIT_FATAL, false, false, "cannot allocate memory");
    return;
  }
  if (g_ls_data->settings.filter.dereference_links || g_ls_data->settings.filter.dereference_links_cli) {
    if (stat(link_path, &file->stat) == -1) {
      ft_show_error(EXIT_MINOR, false, true, "cannot access '%s'", file->display_path);
      free(link);
      free(link_path);
      file->symlinkd = true;
      file->symlink = NULL;
      return;
    }
    free(file->full_path);
    file->id = file->stat.st_ino;
    file->full_path = link_path;
    file->statd = true;
    file->size = file->stat.st_size;
    file->type = get_file_type_by_stat(file->stat.st_mode);
    free(file->owner);
    free(file->group);
    file->owner = get_file_owner_name(file);
    file->group = get_file_group_name(file);
    file_build_permissions(file);
    free(link);
  }
  else {
    file->symlinkd = true;
    file->symlink = link;
    file->symlink_path = link_path;
    if (stat(link_path, &file->lstat) == -1) {
      ft_show_error(EXIT_MINOR, false, true, "cannot access '%s'", file->display_path);
      free(file->symlink);
      free(file->symlink_path);
      file->symlink = NULL;
      file->symlink_path = NULL;
      file->symlinkd = false;
      return;
    }
    file->ltype = get_file_type_by_dirent(file->lstat.st_mode);
  }
}

static t_file* new_file(
  t_file* directory,
  const char* name,
  const char* full_path,
  size_t id,
  t_file_type type,
  t_file* ref,
  bool resolve_symlink
) {
  t_file* file = ref ? ref : ft_calloc(1, sizeof(t_file));
  if (!file)
    return NULL;
  ft_bzero(file, sizeof(t_file));
  file->id = id;
  file->name = ft_strdup(name);
  file->full_path = ft_strdup(full_path);
  file->type = type;
  file->display_path = !directory ? ft_strdup(name) : resolve_path(2, directory->display_path, name);
  if (!full_path)
    file->full_path = resolve_path(2, directory->full_path, name);
  if (!file->name || !file->full_path || !file->display_path) {
    file_free(file);
    ft_show_error(EXIT_FATAL, true, false, "cannot allocate memory");
  }
  file->path = ft_strrchr(file->full_path, '/');
  if (!file->path)
    file->path = ft_strdup("./");
  else
    file->path = ft_substr(file->full_path, 0, file->path - file->full_path + 1);
  if (!file->path) {
    file_free(file);
    ft_show_error(EXIT_FATAL, true, false, "cannot allocate memory");
    return NULL;
  }
  if (type == FILE_SYMLINK && resolve_symlink)
    file_readlink(file);
  return file;
}


t_file* file_from_dir_entry(
  t_file* directory,
  struct dirent* entry,
  t_file* ref
) {
  return new_file(
    directory,
    entry->d_name, NULL,
    entry->d_ino,
    get_file_type_by_dirent(entry->d_type),
    ref,
    true
  );
}

t_file* file_from_stat(const char* full_path, t_file* ref, bool resolve_symlink) {
  struct stat stat;
  if (lstat(full_path, &stat) == -1) {
    ft_show_error(EXIT_MINOR, false, true, "cannot access '%s'", full_path);
    return NULL;
  }
  t_file* file = new_file(
    NULL,
    full_path, full_path,
    stat.st_ino,
    get_file_type_by_stat(stat.st_mode),
    ref,
    resolve_symlink
  );
  if (!file)
    return NULL;
  if (file->statd)
    return file;
  file->statd = true;
  file->stat = stat;
  file->size = stat.st_size;
  file->owner = get_file_owner_name(file);
  file->group = get_file_group_name(file);
  file_build_permissions(file);
  return file;
}

bool file_stat(t_file* file)
{
  if (file->statd)
    return true;
  if (lstat(file->full_path, &file->stat) == -1) {
    ft_show_error(EXIT_MINOR, false, true, "cannot access '%s'", file->from_link, file->full_path);
    return false;
  }
  file->statd = true;
  file->size = file->stat.st_size;
  file->owner = get_file_owner_name(file);
  file->group = get_file_group_name(file);
  file_build_permissions(file);
  return true;
}

t_file* file_from_symlink_view(t_file* link, t_file* file) {
  if (!file->symlinkd)
    return NULL;

  ft_bzero(link, sizeof(t_file));
  link->stat = file->lstat;
  link->statd = true;
  link->size = file->lstat.st_size;
  link->name = file->symlink;
  link->display_path = file->display_path;
  link->type = get_file_type_by_stat(file->lstat.st_mode);
  file_build_permissions(link);
  return link;
}

void file_cleanup(t_file* file) {
  free(file->display_path);
  free(file->name);
  free(file->path);
  free(file->full_path);
  free(file->symlink);
  free(file->symlink_path);
  free(file->owner);
  free(file->group);
}

void file_debug_print(t_file* file) {
  ft_printf("%s:\n", file->display_path);
  ft_printf(" -id: %u\n", file->id);
  ft_printf(" -name: %s\n", file->name);
  ft_printf(" -path: %s\n", file->path);
  ft_printf(" -full_path: %s\n", file->full_path);
  ft_printf(" -input: %s\n", file->display_path);
  ft_printf(" -type: %c\n", file->type);
  if (file->statd) {
    ft_printf(" -owner: %s\n", file->owner);
    ft_printf(" -group: %s\n", file->group);
    ft_printf(" -size: %u\n", file->size);
    ft_printf(" -mode: %o\n", file->stat.st_mode);
    ft_printf(" -uid: %u\n", file->stat.st_uid);
    ft_printf(" -gid: %u\n", file->stat.st_gid);
    ft_printf(" -perms: %.3s%.3s%.3s\n", file->perms.user, file->perms.group, file->perms.other);
  }
  if (file->symlinkd) {
    ft_printf(" -symlink: %s\n", file->symlink);
    ft_printf(" -symlink_path: %s\n", file->symlink_path);
    ft_printf(" -ltype: %c\n", file->ltype);
    ft_printf(" -lmode: %o\n", file->lstat.st_mode);
    ft_printf(" -luid: %u\n", file->lstat.st_uid);
    ft_printf(" -lgid: %u\n", file->lstat.st_gid);
    ft_printf(" -latime: %d\n", file->lstat.st_atime);
    ft_printf(" -lmtime: %d\n", file->lstat.st_mtime);
    ft_printf(" -lctime: %d\n", file->lstat.st_ctime);
  }
}

void file_free(t_file* file) {
  file_cleanup(file);
  free(file);
}
