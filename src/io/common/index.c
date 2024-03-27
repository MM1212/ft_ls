/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 12:29:22 by martiper          #+#    #+#             */
/*   Updated: 2024/03/27 22:29:05 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "io.h"

bool io_describe_common_options(t_cli_handle* cli) {
  (void)cli;
  // if (!io_describe_block_size(cli))
  //   return false;
  return true;
}
