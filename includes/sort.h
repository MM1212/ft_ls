/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:30:59 by martiper          #+#    #+#             */
/*   Updated: 2024/03/26 20:43:16 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "settings.h"
#include <file.h>
#include <lxt/dst/vector.h>

// DEFAULT
int sort_by_ascii(t_file *a, t_file *b);
// sort by modficication time, newest first
int sort_by_time(t_file *a, t_file *b, t_settings *settings);
// sort by size, largest first
int sort_by_size(t_file *a, t_file *b);
// reverse order
int sort_by_reverse(t_file *a, t_file *b);
// https://www.gnu.org/software/coreutils/manual/html_node/Version-sort-overview.html
int sort_by_version(t_file *a, t_file *b);

void sort_files(t_list *files, t_settings *settings);
void sort_files2(t_vector *files, t_settings *settings);
