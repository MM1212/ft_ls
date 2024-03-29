/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 15:08:34 by martiper          #+#    #+#             */
/*   Updated: 2024/03/29 15:51:22 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include <dirent.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <libft.h>

typedef struct s_file t_file;
typedef struct s_file_simple t_file_simple;
typedef enum e_file_type t_file_type;

enum e_file_type
{
  FILE_REGULAR = '-',
  FILE_BLOCK_SPECIAL = 'b',
  FILE_CHARACTER_SPECIAL = 'c',
  FILE_DIRECTORY = 'd',
  FILE_SYMLINK = 'l',
  FILE_FIFO = 'p',
  FILE_SOCKET = 's',
  FILE_OTHER = '?'
};

struct s_file_permissions
{
  char  user[3];
  char  group[3];
  char  other[3];
};

struct s_file
{
  size_t id;
  char* name; // file name
  char* display_path; // display path (dir only)
  char* path; // relative path to file
  char* full_path; // full path to file (path + name)
  t_file_type type;

  // stat, only used in long format
  bool statd;
  struct stat stat;
  size_t size;
  char* owner;
  char* group;
  struct s_file_permissions perms;
  // followd a symlink
  bool from_link;
  bool symlinkd;
  char* symlink;
  char* symlink_path;
  struct stat lstat;
  t_file_type ltype;

  t_file* parent;

  size_t name_width;
  size_t bsize_width;
  size_t inode_width;
};

t_file* file_from_dir_entry(
  t_file* directory,
  struct dirent* entry,
  t_file* ref
);
t_file* file_from_stat(const char* path, t_file* ref, bool follow_symlink);
t_file* file_from_symlink_view(t_file* dst, t_file* src);
bool file_stat(t_file* file);
void file_cleanup(t_file* file);
void file_debug_print(t_file* file);
void file_free(t_file* file);
bool file_is_dir(t_file* file);
void file_build_permissions(t_file* file);
t_file_type  get_file_type_by_dirent(uint8_t flag);
t_file_type get_file_type_by_stat(uint32_t mode);
char* get_file_owner_name(t_file* file);
char* get_file_group_name(t_file* file);
char* resolve_path(size_t count, ...);
bool is_file_executable(t_file* file);
bool is_file_a_broken_link(t_file* file);
