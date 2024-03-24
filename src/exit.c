/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 14:45:42 by martiper          #+#    #+#             */
/*   Updated: 2024/03/24 20:04:52 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void ft_ls_destroy(t_ft_ls* data) {
  data->io->free();
  data->colors->destroy(data->colors);
  ft_lstclear(&data->file_entries, (void (*)(void *))file_free);
  ft_lstclear(&data->dir_entries, (void (*)(void *))file_free);
}
void ft_exit(t_ft_ls* data, int status, char* message)
{
  ft_ls_destroy(data);
  if (message)
    ft_fprintf(\
      status == 0 ? STDOUT_FILENO : STDERR_FILENO, \
      "ft_ls: %s\n", message \
    );
  exit(status);
}
