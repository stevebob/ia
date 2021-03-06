#ifndef FOV_H
#define FOV_H

#include "cmn_data.hpp"
#include "cmn_types.hpp"

namespace fov
{

bool check_cell(const bool obstructions[MAP_W][MAP_H],
                const Pos& cell_to_check,
                const Pos& origin, const bool IS_AFFECTED_BY_DARKNESS);

void run_player_fov(const bool obstructions[MAP_W][MAP_H], const Pos& origin);

void run_fov_on_array(const bool obstructions[MAP_W][MAP_H], const Pos& origin,
                      bool values[MAP_W][MAP_H],
                      const bool IS_AFFECTED_BY_DARKNESS);

} //fov

#endif
