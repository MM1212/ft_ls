/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 14:45:42 by martiper          #+#    #+#             */
/*   Updated: 2024/03/26 11:52:18 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void ft_ls_destroy(t_ft_ls* data) {
  data->io->free();
  data->colors->destroy(data->colors);
  ft_lstclear(&data->file_entries, (void (*)(void*))file_free);
  ft_lstclear(&data->dir_entries, (void (*)(void*))file_free);
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

void ft_show_error(
  t_ft_ls* data,
  t_exit_status status, bool quit, bool sys_call,
  char* message, ...
) {
  char buf[2048];
  ft_sprintf(buf, 2048, "ft_ls: ");
  va_list args;
  va_start(args, message);
  ft_vsprintf(buf + 7, 2048 - 7, message, args);
  va_end(args);
  if (sys_call)
    perror(buf);
  else
    ft_fprintf(2, "%s\n", buf);
  if ((data->exit_status == EXIT_OK || status != EXIT_OK))
    data->exit_status = status;
  if (quit)
    ft_exit(data, status, NULL);
}
