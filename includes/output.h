/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 18:00:16 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 19:57:02 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ft_ls.h>
#include <file.h>

#define DATE_SIZE 13

void file_print(t_file* file, t_ft_ls* data);
t_list* get_files_from_dir(t_file* dir, t_settings* settings);
char* get_file_verbose_time(t_file* file, char buf[DATE_SIZE]);
