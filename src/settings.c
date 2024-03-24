/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 14:56:45 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 19:38:01 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "settings.h"
#include <libft.h>

void settings_init(t_settings* settings) {
  (void)settings;
}

void settings_print(t_settings* settings) {
  ft_printf("Settings:\n");
  ft_printf("\tfilter:\n");
  ft_printf("\t - all: %b\n", settings->filter.all);
  ft_printf("\t - almost_all: %b\n", settings->filter.almost_all);
  ft_printf("\t - ignore_backups: %b\n", settings->filter.ignore_backups);
  ft_printf("\t - list_directories: %b\n", settings->filter.list_directories);
  ft_printf("\t - dereference_links_cli: %b\n", settings->filter.dereference_links_cli);
  ft_printf("\t - dereference_links: %b\n", settings->filter.dereference_links);
  ft_printf("\t - recursive: %b\n", settings->filter.recursive);
  ft_printf("\tdisplay:\n");
  ft_printf("\t - author: %b\n", settings->display.author);
  ft_printf("\t - omit_owner: %b\n", settings->display.omit_owner);
  ft_printf("\t - omit_group: %b\n", settings->display.omit_group);
  ft_printf("\t - inode: %b\n", settings->display.inode);
  ft_printf("\t - numeric_ids: %b\n", settings->display.numeric_ids);
  ft_printf("\t - block_size: %b\n", settings->display.block_size);
  ft_printf("\t - color: %d\n", settings->display.color);
  ft_printf("\tsort:\n");
  ft_printf("\t - type: %d\n", settings->sort.type);
  ft_printf("\t - reverse: %b\n", settings->sort.reverse);
  ft_printf("\tformat:\n");
  ft_printf("\t - type: %d\n", settings->format.type);
  ft_printf("\t - block_size: %d\n", settings->format.block_size);
  ft_printf("\t - time: %d\n", settings->format.time);
  ft_printf("\t - indicator_style: %d\n", settings->format.indicator_style);
}