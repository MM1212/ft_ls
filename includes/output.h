/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 18:00:16 by martiper          #+#    #+#             */
/*   Updated: 2024/03/26 15:45:13 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <ft_ls.h>
#include <file.h>

void file_print(t_file* file, t_ft_ls* data);
t_list* get_files_from_dir(t_file* dir, t_settings* settings);
char* resolve_path(size_t count, ...);
