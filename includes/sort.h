/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 22:30:59 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 23:45:32 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "settings.h"
#include <file.h>
#include <lxt/dst/vector.h>

t_list* sort_files(t_list* files, t_settings* settings);
t_vector* sort_files2(t_vector* files, t_settings* settings);
