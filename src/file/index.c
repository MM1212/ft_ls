/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 16:18:11 by martiper          #+#    #+#             */
/*   Updated: 2024/03/26 16:11:44 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define __USE_MISC
#define __USE_XOPEN_EXTENDED
#define __USE_XOPEN2K
#include "file.h"
#include <stdint.h>
#include <libft.h>
#include <unistd.h>
#include <linux/stat.h>
#include <ft_ls.h>

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

void file_build_permissions(t_file* file) {
  struct s_file_permissions* perms = &file->perms;
  perms->user[0] = (file->stat.st_mode & S_IRUSR) ? 'r' : '-';
  perms->user[1] = (file->stat.st_mode & S_IWUSR) ? 'w' : '-';
  perms->user[2] = (file->stat.st_mode & S_IXUSR) ? 'x' : '-';
  perms->group[0] = (file->stat.st_mode & S_IRGRP) ? 'r' : '-';
  perms->group[1] = (file->stat.st_mode & S_IWGRP) ? 'w' : '-';
  perms->group[2] = (file->stat.st_mode & S_IXGRP) ? 'x' : '-';
  perms->other[0] = (file->stat.st_mode & S_IROTH) ? 'r' : '-';
  perms->other[1] = (file->stat.st_mode & S_IWOTH) ? 'w' : '-';
  perms->other[2] = (file->stat.st_mode & S_IXOTH) ? 'x' : '-';
}

static void file_readlink(t_file* file) {
  char buf[4098];
  ssize_t len = readlink(file->full_path, buf, 4098);
  if (len == -1) {
    ft_show_error(g_ls_data, EXIT_MINOR, false, true, "cannot read symbolic link '%s'", file->full_path);
    return;
  }
  buf[len] = '\0';
  char* link = ft_strdup(buf);
  char* link_path = ft_str_startswith(buf, "/") ? ft_strdup(buf) : ft_strjoin(2, file->path, buf);
  if (!link || !link_path) {
    free(link);
    free(link_path);
    ft_show_error(g_ls_data, EXIT_FATAL, false, false, "cannot allocate memory");
    return;
  }
  file->symlinkd = true;
  file->symlink = link;
  if (stat(link_path, &file->lstat) == -1) {
    ft_show_error(g_ls_data, EXIT_MINOR, false, true, "cannot access '%s'", link);
    free(file->symlink);
    free(link_path);
    file->symlink = NULL;
    file->symlinkd = false;
    return;
  }
  free(link_path);
  file->ltype = get_file_type_by_dirent(file->lstat.st_mode);
}

static t_file* new_file(
  const char* input,
  const char* name,
  const char* path,
  size_t id,
  t_file_type type
) {
  t_file* file = ft_calloc(1, sizeof(t_file));
  if (!file)
    return NULL;
  file->id = id;
  file->input = ft_strdup(input);
  file->name = ft_strdup(name);
  file->path = ft_strdup(path);
  if (!ft_str_endswith(file->path, "/")) {
    char* tmp = ft_strjoin(2, file->path, "/");
    free(file->path);
    file->path = tmp;
  }
  file->full_path = ft_strjoin(2, file->path, file->name);
  if (!file->full_path || !file->name || !file->path) {
    file_free(file);
    return NULL;
  }
  file->type = type;
  if (file->type == FILE_SYMLINK)
    file_readlink(file);
  return file;
}

t_file* file_from_dir_entry(
  const char* name,
  const char* path,
  struct dirent* entry
) {
  return new_file(
    name,
    name, path,
    entry->d_ino,
    get_file_type_by_dirent(entry->d_type)
  );
}

t_file* file_from_stat(const char* full_path) {
  struct stat stat;
  if (lstat(full_path, &stat) == -1) {
    ft_show_error(g_ls_data, EXIT_MINOR, false, true, "cannot access '%s'", full_path);
    return NULL;
  }
  const char* folder_slash = ft_strrchr(full_path, '/');
  const char* name = folder_slash && (*(folder_slash + 1) != '\0') ? folder_slash + 1 : full_path;
  char* path = folder_slash && (*(folder_slash + 1) != '\0') ? ft_substr(full_path, 0, folder_slash - full_path) : ft_strdup("./");
  t_file* file = new_file(
    full_path,
    name, path,
    stat.st_ino,
    get_file_type_by_stat(stat.st_mode)
  );
  free(path);
  if (!file)
    return NULL;
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
  if (lstat(file->full_path, &file->stat) == -1)
    return false;
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
  link->type = get_file_type_by_stat(file->lstat.st_mode);
  file_build_permissions(link);
  return link;
}

t_file* file_from_symlink(t_file* file) {
  if (!file->symlinkd)
    return NULL;
  char* full_path;

  if (*file->symlink == '/')
    full_path = ft_strdup(file->symlink);
  else
    full_path = ft_strjoin(2, file->path, file->symlink);
  if (!full_path)
    return NULL;
  t_file* link = file_from_stat(full_path);
  if (!link) {
    free(full_path);
    return NULL;
  }
  free(link->name);
  link->name = ft_strdup(file->name);
  free(full_path);
  return link;
}


void file_cleanup(t_file* file) {
  free(file->input);
  free(file->name);
  free(file->path);
  free(file->full_path);
  free(file->symlink);
  free(file->owner);
  free(file->group);
}

void file_debug_print(t_file* file) {
  ft_printf("%s:\n", file->name);
  ft_printf(" -id: %u\n", file->id);
  ft_printf(" -name: %s\n", file->name);
  ft_printf(" -path: %s\n", file->path);
  ft_printf(" -full_path: %s\n", file->full_path);
  ft_printf(" -input: %s\n", file->input);
  ft_printf(" -type: %c\n", file->type);
  if (file->statd) {
    ft_printf(" -owner: %s\n", file->owner);
    ft_printf(" -group: %s\n", file->group);
    ft_printf(" -size: %u\n", file->size);
    ft_printf(" -mode: %o\n", file->stat.st_mode);
    ft_printf(" -uid: %u\n", file->stat.st_uid);
    ft_printf(" -gid: %u\n", file->stat.st_gid);
    ft_printf(" -atime: %d\n", file->stat.st_atime);
    ft_printf(" -mtime: %d\n", file->stat.st_mtime);
    ft_printf(" -ctime: %d\n", file->stat.st_ctime);
    ft_printf(" -perms: %.3s%.3s%.3s\n", file->perms.user, file->perms.group, file->perms.other);
  }
  if (file->symlinkd) {
    ft_printf(" -symlink: %s\n", file->symlink);
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

