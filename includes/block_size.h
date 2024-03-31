/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_size.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 10:19:27 by martiper          #+#    #+#             */
/*   Updated: 2024/03/31 15:52:16 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum e_settings_block_size t_settings_block_size;

enum e_settings_block_size {
  BLOCK_SIZE_BYTES,
  BLOCK_SIZE_HUMAN_READABLE,
  BLOCK_SIZE_SI,
  BLOCK_SIZE_KILOBYTES,
  BLOCK_SIZE_KIBIBYTES,
  BLOCK_SIZE_MEGABYTES,
  BLOCK_SIZE_MEBIBYTES,
  BLOCK_SIZE_GIGABYTES,
  BLOCK_SIZE_GIBIBYTES,
  BLOCK_SIZE_TERABYTES,
  BLOCK_SIZE_TEBIBYTES,
  BLOCK_SIZE_PETABYTES,
  BLOCK_SIZE_PEBIBYTES,
  BLOCK_SIZE_EXABYTES,
  BLOCK_SIZE_EXBIBYTES,
};

typedef enum e_powers_of {
  POWER_METRIC = 1000,
  POWER_BINARY = 1024
} t_powers_of;

typedef struct e_power {
  t_powers_of base;
  t_settings_block_size unit;
  size_t exp;
  size_t floor;
  char* suffix;
  char* suffix_letter;
  char* match;
  size_t suffix_len;
} t_power;

const t_power* get_powers(void);
size_t get_powers_size(void);
void block_size_compute(void);
const t_power* match_power(const char* str);
struct s_settings;
char* get_block_size(size_t size, struct s_settings* settings, size_t multiplier);
