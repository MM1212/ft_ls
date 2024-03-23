/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 11:16:47 by martiper          #+#    #+#             */
/*   Updated: 2024/03/23 12:29:05 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <libft.h>
#include <io.h>

int main(int ac, char **av) {
  int status;
  bool exit_request;
  if ((status = io_run(ac, av, &exit_request)) != 0)
    return status;
  if (exit_request)
    return 0;
  return 0;
}
