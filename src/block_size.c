/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_size.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: martiper <martiper@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 10:13:11 by martiper          #+#    #+#             */
/*   Updated: 2024/03/31 16:11:27 by martiper         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "block_size.h"
#include <utils.h>
#include <libft.h>
#include <settings.h>
#include <math.h>

/*
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
 */

static t_power g_powers[] = {
  {POWER_METRIC, BLOCK_SIZE_BYTES, 0, 0, "", "", "", 0},
  {POWER_METRIC, BLOCK_SIZE_KILOBYTES, 1, 0, "kB", "K", "kB", 0},
  {POWER_METRIC, BLOCK_SIZE_MEGABYTES, 2, 0, "MB", "M", "MB", 0},
  {POWER_METRIC, BLOCK_SIZE_GIGABYTES, 3, 0, "GB", "G", "GB", 0},
  {POWER_METRIC, BLOCK_SIZE_TERABYTES, 4, 0, "TB", "T", "TB", 0},
  {POWER_METRIC, BLOCK_SIZE_PETABYTES, 5, 0, "PB", "P", "PB", 0},
  {POWER_METRIC, BLOCK_SIZE_EXABYTES, 6, 0, "EB", "E", "EB", 0},
  {POWER_BINARY, BLOCK_SIZE_BYTES, 0, 0, "", "", "", 0},
  {POWER_BINARY, BLOCK_SIZE_KIBIBYTES, 1, 0, "KiB", "K", "k,KiB", 0},
  {POWER_BINARY, BLOCK_SIZE_MEBIBYTES, 2, 0, "MiB", "M", "M,MiB", 0},
  {POWER_BINARY, BLOCK_SIZE_GIBIBYTES, 3, 0, "GiB", "G", "G,GiB", 0},
  {POWER_BINARY, BLOCK_SIZE_TEBIBYTES, 4, 0, "TiB", "T", "T,TiB", 0},
  {POWER_BINARY, BLOCK_SIZE_PEBIBYTES, 5, 0, "PiB", "P", "P,PiB", 0},
  {POWER_BINARY, BLOCK_SIZE_EXBIBYTES, 6, 0, "EiB", "E", "E,EiB", 0},
};

static const size_t g_powers_size = sizeof(g_powers) / sizeof(t_power);

const t_power* get_powers(void) {
  return g_powers;
}
size_t get_powers_size(void) {
  return g_powers_size;
}

/* static size_t powl(size_t base, size_t exp) {
  size_t result = 1;
  for (size_t i = 0; i < exp; i++) {
    result *= base;
  }
  return result;
} */

void block_size_compute(void) {
  for (uint32_t i = 0; i < g_powers_size; i++) {
    t_power* power = (t_power*)&g_powers[i];
    if (power->exp == 0)
      power->floor = 0;
    else
      power->floor = ft_pow(power->base, power->exp);
    power->suffix_len = ft_strlen(power->suffix);
  }
}

const t_power* match_power(const char* str) {
  for (uint32_t i = 0; i < g_powers_size; i++) {
    const t_power* power = &g_powers[i];
    char* match = power->match;
    while (*match) {
      char* next = ft_strchr(match, ',');
      size_t len = next ? next - match : ft_strlen(match);
      size_t match_len = FT_MIN(len, ft_strlen(match));
      bool matches;
      if (ft_strlen(str) > match_len)
        matches = ft_wildcard_match2(match, str, (t_wildcard_flags) { 0, match_len, true });
      else
        matches = ft_wildcard_match2(str, match, (t_wildcard_flags) { match_len, 0, true });
      if (matches)
        return power;
      if (!next)
        break;
      match += len + 1;
    }
  }
  return NULL;
}

const t_power* get_closest_power(size_t size, t_powers_of base) {
  for (uint32_t i = g_powers_size - 1; i >= 0; i--) {
    const t_power* power = &g_powers[i];
    if (power->base == base && size >= power->floor)
      return power;
  }
  return NULL;
}

char* get_block_size(size_t size, t_settings* settings, size_t multiplier) {
  static char buffer[256];
  if (settings->format.block_size == BLOCK_SIZE_BYTES) {
    ft_sprintf(buffer, 256, "%u", size);
    return buffer;
  }
  size *= multiplier;
  const t_power* power = NULL;
  bool human_readable = false;
  switch (settings->format.block_size) {
  case BLOCK_SIZE_SI:
  case BLOCK_SIZE_HUMAN_READABLE: {
    bool is_si = settings->format.block_size == BLOCK_SIZE_SI;
    power = get_closest_power(size, is_si ? POWER_METRIC : POWER_BINARY);
    human_readable = true;
    break;
  }
  default:
    power = settings->format.block_size_power;
    break;
  }
  if (!power)
    return buffer;
  double value = ((double)size / FT_MAX(1, power->floor));
  if (value < 1)
    value = 1;
  if (ft_unbrlen_base(value, 10) > 1 || power->floor == 0 || !human_readable)
    ft_sprintf(buffer, 256, "%u", (long)ceil(value), value);
  else
    ft_sprintf(buffer, 256, "%.1f", value);
  if (human_readable)
    ft_strlcpy(buffer + ft_strlen(buffer), power->suffix, 2);
  else if (settings->format.block_size_one_letter)
    ft_strlcat(buffer, power->suffix_letter, 256);
  else
    ft_strlcat(buffer, power->suffix, 256);
  return buffer;
}
