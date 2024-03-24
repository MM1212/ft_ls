/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:30:59 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 22:46:06 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "settings.h"
#include <file.h>

// DEFAULT
int sort_ascii(t_file *a, t_file *b);
// sort by modficication time, newest first
int sort_time(t_file *a, t_file *b);
// sort by size, largest first
int sort_size(t_file *a, t_file *b);
// reverse order
int sort_reverse(t_file *a, t_file *b);
// https://www.gnu.org/software/coreutils/manual/html_node/Version-sort-overview.html
int sort_version(t_file *a, t_file *b);
