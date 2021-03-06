#include "fov.hpp"

#include "init.hpp"

#include <math.h>
#include <vector>

#include "cmn_types.hpp"
#include "line_calc.hpp"
#include "map.hpp"
#include "utils.hpp"

using namespace std;

namespace fov
{

namespace
{

void check_one_cell_of_many(const bool obstructions[MAP_W][MAP_H],
                            const Pos& cell_to_check,
                            const Pos& origin, bool values[MAP_W][MAP_H],
                            const bool IS_AFFECTED_BY_DARKNESS)
{

    const Pos delta_to_tgt(cell_to_check.x - origin.x, cell_to_check.y - origin.y);

    const vector<Pos>* path_deltas_ptr =
        line_calc::fov_delta_line(delta_to_tgt, FOV_STD_RADI_DB);

    if (!path_deltas_ptr)
    {
        return;
    }

    const vector<Pos>& path_deltas = *path_deltas_ptr;

    const bool TGT_IS_LGT = map::cells[cell_to_check.x][cell_to_check.y].is_lit;

    Pos cur_pos;
    Pos prev_pos;
    const size_t PATH_SIZE = path_deltas.size();

    for (size_t i = 0; i < PATH_SIZE; ++i)
    {
        cur_pos.set(origin + path_deltas[i]);

        if (i > 1)
        {
            prev_pos.set(origin + path_deltas[i - 1]);
            const bool PRE_CELL_IS_DRK = map::cells[prev_pos.x][prev_pos.y].is_dark;
            const bool CUR_CELL_IS_DRK = map::cells[cur_pos.x][cur_pos.y].is_dark;
            const bool CUR_CELL_IS_LGT = map::cells[cur_pos.x][cur_pos.y].is_lit;

            if (
                !CUR_CELL_IS_LGT                      &&
                !TGT_IS_LGT                           &&
                (PRE_CELL_IS_DRK || CUR_CELL_IS_DRK)  &&
                IS_AFFECTED_BY_DARKNESS)
            {
                return;
            }
        }

        if (cur_pos == cell_to_check)
        {
            values[cell_to_check.x][cell_to_check.y] = true;
            return;
        }

        if (i > 0)
        {
            if (obstructions[cur_pos.x][cur_pos.y])
            {
                return;
            }
        }
    }
}

} //namespace

bool check_cell(const bool obstructions[MAP_W][MAP_H], const Pos& cell_to_check,
                const Pos& origin, const bool IS_AFFECTED_BY_DARKNESS)
{

    if (!utils::is_pos_inside_map(cell_to_check)) {return false;}

    if (utils::king_dist(origin, cell_to_check) > FOV_STD_RADI_INT) {return false;}

    const Pos delta_to_tgt(cell_to_check - origin);

    const vector<Pos>* path_deltas_ptr =
        line_calc::fov_delta_line(delta_to_tgt, FOV_STD_RADI_DB);

    if (!path_deltas_ptr)
    {
        return false;
    }

    const vector<Pos>& path_deltas = *path_deltas_ptr;

    const bool TGT_IS_LGT = map::cells[cell_to_check.x][cell_to_check.y].is_lit;

    Pos cur_pos;
    Pos prev_pos;
    const size_t PATH_SIZE = path_deltas.size();

    for (size_t i = 0; i < PATH_SIZE; ++i)
    {
        cur_pos.set(origin + path_deltas[i]);

        if (i > 1)
        {
            prev_pos.set(origin + path_deltas[i - 1]);
            const bool PRE_CELL_IS_DRK = map::cells[prev_pos.x][prev_pos.y].is_dark;
            const bool CUR_CELL_IS_DRK = map::cells[cur_pos.x][cur_pos.y].is_dark;
            const bool CUR_CELL_IS_LGT = map::cells[cur_pos.x][cur_pos.y].is_lit;

            if (
                !CUR_CELL_IS_LGT                      &&
                !TGT_IS_LGT                           &&
                (PRE_CELL_IS_DRK || CUR_CELL_IS_DRK)  &&
                IS_AFFECTED_BY_DARKNESS)
            {
                return false;
            }
        }

        if (cur_pos == cell_to_check) {return true;}

        if (i > 0 && obstructions[cur_pos.x][cur_pos.y]) {return false;}
    }

    return false;
}

void run_fov_on_array(const bool obstructions[MAP_W][MAP_H], const Pos& origin,
                      bool values[MAP_W][MAP_H],
                      const bool IS_AFFECTED_BY_DARKNESS)
{
    utils::reset_array(values, false);

    values[origin.x][origin.y] = true;

    const int check_x_end = min(MAP_W - 1, origin.x + FOV_STD_RADI_INT);
    const int check_y_end = min(MAP_H - 1, origin.y + FOV_STD_RADI_INT);

    int check_x = max(0, origin.x - FOV_STD_RADI_INT);

    while (check_x <= check_x_end)
    {
        int check_y = max(0, origin.y - FOV_STD_RADI_INT);

        while (check_y <= check_y_end)
        {
            check_one_cell_of_many(obstructions, Pos(check_x, check_y), origin, values,
                                   IS_AFFECTED_BY_DARKNESS);
            check_y++;
        }

        check_x++;
    }
}

void run_player_fov(const bool obstructions[MAP_W][MAP_H], const Pos& origin)
{
    bool fov_tmp[MAP_W][MAP_H];

    for (int x = 0; x < MAP_W; ++x)
    {
        for (int y = 0; y < MAP_H; ++y)
        {
            map::cells[x][y].is_seen_by_player = false;
            fov_tmp[x][y]                    = false;
        }
    }

    map::cells[origin.x][origin.y].is_seen_by_player = true;
    fov_tmp[origin.x][origin.y] = true;

    const int R = FOV_STD_RADI_INT;
    const int X0 = constr_in_range(0, origin.x - R, MAP_W - 1);
    const int Y0 = constr_in_range(0, origin.y - R, MAP_H - 1);
    const int X1 = constr_in_range(0, origin.x + R, MAP_W - 1);
    const int Y1 = constr_in_range(0, origin.y + R, MAP_H - 1);

    for (int y = Y0; y <= Y1; ++y)
    {
        for (int x = X0; x <= X1; ++x)
        {
            check_one_cell_of_many(obstructions, Pos(x, y), origin, fov_tmp, true);
            map::cells[x][y].is_seen_by_player = fov_tmp[x][y];
        }
    }
}

} //fov
