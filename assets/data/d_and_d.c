#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint64_t board;
typedef bool map[64];
typedef char constraint[8];

struct problem {
    board monsters;
    board treasures;
    constraint rows;
    constraint columns;
};
typedef struct problem problem;

problem p1 = {
	.monsters = 0x8400010000000020,
	.treasures = 0x20000,
	.rows = { 7,3,4,1,7,1,6,3 },
	.columns = { 4,4,2,6,2,3,4,7 }
};

problem p2 = {
	.monsters = 0xa00008000000000,
	.treasures = 0x2800,
	.rows = { 5,2,2,3,6,0,6,1 },
	.columns = { 5,3,4,3,5,1,3,1 }
};

problem p3 = {
	.monsters = 0x8000008001000081,
	.treasures = 0x4000000000000,
	.rows = { 6,0,4,1,5,3,3,4 },
	.columns = { 4,4,1,3,6,3,4,1 }
};

problem p4 = {
	.monsters = 0x8000800000000100,
	.treasures = 0x2000,
	.rows = { 2,3,4,3,7,0,4,0 },
	.columns = { 1,4,1,5,1,3,2,6 }
};

problem p5 = {
	.monsters = 0x20001,
	.treasures = 0x80000,
	.rows = { 0,7,3,3,3,5,2,5 },
	.columns = { 7,2,6,1,3,2,6,1 }
};

problem p6 = {
	.monsters = 0x802080208020,
	.treasures = 0x40000,
	.rows = { 6,3,3,3,5,3,4,3 },
	.columns = { 5,3,2,4,6,4,1,5 }
};

problem p7 = {
	.monsters = 0x80010008,
	.treasures = 0x80000000000000,
	.rows = { 1,4,2,6,2,3,3,1 },
	.columns = { 1,5,2,2,6,1,4,1 }
};

problem p8 = {
	.monsters = 0x4200801000800042,
	.treasures = 0x0,
	.rows = { 3,6,0,5,4,0,6,3 },
	.columns = { 4,2,4,2,3,4,2,6 }
};

problem p9 = {
	.monsters = 0x40080,
	.treasures = 0x200000000000000,
	.rows = { 4,1,4,2,6,2,3,2 },
	.columns = { 5,2,1,2,5,4,2,3 }
};

board map_to_board(map m)
{
}

void board_to_map(board b, map m)
{
}

int popcount_slow(uint64_t v)
{
}

int popcount(uint64_t v)
{
}

void print_board(problem *p, board b)
{
}

int enum_popcount_byte[256];

uint64_t bit(int pos)
{
    return (uint64_t) 1 << pos;
}

uint64_t mask_column(int i)
{
}

bool test_columns_overload(problem *p, board b)
{
}

bool test_columns_underload(problem *p, board b)
{
}

bool empty(problem *p, board b, int pos)
{
}

bool treasure(problem *p, int pos)
{
}

bool monster(problem *p, int pos)
{
}

uint64_t neighbours(int pos)
{
}

int count_non_wall(problem *p, board b, int pos)
{
}

bool test_no_deadend_upto_row(problem *p, board b, int row)
{
}

bool test_trapped_monster(problem *p, board b)
{
}

bool test_deadends(problem *p, board b)
{
}

bool test_empty_squares(problem *p, board b)
{
}

bool test_treasure_rooms(problem *p, board *b)
{
}

void solve(problem *p, int row, board b)
{
    // Unsolvable from this board
    if (test_columns_overload(p, b))
        return;
    if (test_no_deadend_upto_row(p, b, row))
        return;
    if (test_trapped_monster(p, b))
        return;

    // Board is filled
    if(row == 8)
    {
        // Validate columns count
        if (test_columns_underload(p, b))
            return;

        // Full board with monsters
        board fb = b | p->monsters;

        if (test_treasure_rooms(p, &fb))
            return;
        // Every treasure room is now filled in fb

        if (test_empty_squares(p, fb))
            return;

        printf("Solution %lx\n", b); 
        print_board(p, b);

        return;
    }

    for(int i = 0; i < 256; i++)
    {
        if (enum_popcount_byte[i] == p->rows[row])
        {
            uint64_t move = (uint64_t)i << (8*row);

            // ensure there's no overlap with walls/monsters/treasures
            if ((move & (p->monsters | p->treasures)) != 0)
                continue;

            solve(p, row+1, b + move);
        }
    }
}

int main(void)
{
    problem *problems[] = {
        &p1, &p2, &p3, 
        &p4, &p5, &p6, 
        &p7, &p8, &p9
    };

    for (int i = 0; i < 9; i++)
    {
        printf("Solving problem %d\n", i+1);
        solve(problems[i], 0, 0);
    }
}
