/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 12:58:29 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 20:06:39 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cli.h>
#include <settings.h>
#include <colors.h>
#include <io.h>
#include <padding.h>

typedef struct s_ls t_ft_ls;
struct s_ls {
  t_list* file_entries;
  t_list* dir_entries;
  t_cli_handle* io;
  t_settings settings;
  t_colors_registry* colors;
  t_l_fmt_padding padding;
  bool first_batch_print;
};

void ft_ls_destroy(t_ft_ls* data);
void ft_exit(t_ft_ls* data, int status, char* message);
void ft_ls_run(t_ft_ls* data);
