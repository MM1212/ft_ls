/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   date.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 20:56:45 by martiper          #+#    #+#             */
/*   Updated: 2024/03/29 13:25:29 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <lxt/date.h>
#include <stddef.h>
#include <settings.h>

char* date_get_verbose_date(t_file* file, t_settings* settings);
