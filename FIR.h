#include <tuple>
#include <map>
#include <algorithm>
#include <functional>
#include <iostream>
#include <cstdio>
#include <cassert>

#include "TicTacToe.h"

using namespace std;

template <int SIZE = 13>
struct FIR_T : TicTacToe_T<SIZE>
{
	// 4 = 3m29.908s => 0m49.850s
	const static int MAX_LEVEL = 4;
	const static int MAX_BATTLE_RANGE = 2;

	char isRow() {
		for (int i = 0; i < SIZE; ++i) {
			int s[] = {0,0};
			for (int j = 0; j < SIZE; ++j)
			{
				char stone = this->cb[i][j];
				int x = stone == 'X' ? 1 : 0;
				if (stone != ' ') {
					s[x]++;
					if (s[x] >= 5) return stone;
				} else {
					s[0] = s[1] = 0;
				}
			}
		}
		return '-';
	}
	char isCol() {
		for (int j = 0; j < SIZE; ++j)
		{
			int s[] = {0,0};
			for (int i = 0; i < SIZE; ++i)
			{
				char stone = this->cb[i][j];
				int x = stone == 'X' ? 1 : 0;
				if (stone != ' ') {
					s[x]++;
					if (s[x] >= 5) {
						return stone;
					}
				} else {
					s[0] = s[1] = 0;
				}
			}
		}
		return '-';
	}
	char isX() {
		// is /, the left part
		for (int i = 0; i < SIZE; ++i)
		{
			int s[] = {0,0};
			for (int k = 0; k < SIZE && i-k >= 0; ++k)
			{
				char stone = this->cb[k][i-k];
				// printf("(%d,%d) ", k,i-k);
				if (stone != ' ')
				{
					int x = stone == 'X' ? 1 : 0;
					s[x]++;
					if (s[x] == 5)
					{
						// printf("/ left part\n");
						return stone;
					}
				} else {
					s[0] = s[1] = 0;
				}
			}
			// cout<<endl;
		}
		// is /, the right part
		for (int i = 0; i <= SIZE - 5; ++i)
		{
			int s[] = {0,0};
			for (int k = 0; k < SIZE && i+k < SIZE; ++k)
			{
				char stone = this->cb[i+k][SIZE-k-1];
				// printf("(%d,%d) ", i+k,SIZE-k-1);
				if (stone != ' ')
				{
					int x = stone == 'X' ? 1 : 0;
					s[x]++;
					if (s[x] == 5)
					{
						// printf("/ right part\n");
						return stone;
					}
				}
			}
			// cout<<endl;
		}
		// is \, the right part
		for (int i = 0; i < SIZE; ++i)
		{
			int s[] = {0,0};
			for (int k = 0; k < SIZE && i+k < SIZE; ++k)
			{
				char stone = this->cb[k][i+k];
				if (stone != ' ')
				{
					int x = stone == 'X' ? 1 : 0;
					s[x]++;
					if (s[x] == 5)
					{
						// printf("\\ right\n");
						return stone;
					}
				} else {
					s[0] = s[1] = 0;
				}
			}
		}
		// is \, the left part
		for (int i = 0; i < SIZE; ++i)
		{
			int s[] = {0,0};
			for (int k = 0; k < SIZE && i+k < SIZE; ++k)
			{
				char stone = this->cb[i+k][k];
				if (stone != ' ')
				{
					int x = stone == 'X' ? 1 : 0;
					s[x]++;
					if (s[x] == 5)
					{
						// printf("\\ left \n");
						return stone;
					}
				} else {
					s[0] = s[1] = 0;
				}
			}
		}
		return '-';
	}
	// bool is_left_down_5(int i, int j)
	// {
	// 	char s = this->cb[i][j];
	// 	for (int k = 1; k < 5 && 0 <= i-k && j+k < SIZE; ++k)
	// 	{
	// 		if (this->cb[i-k][j+k] != s) {
	// 			return false;
	// 		}
	// 	}
	// 	return true;
	// }
	// bool is_right_down_5(int i, int j)
	// {
	// 	char s = this->cb[i][j];
	// 	for (int k = 1; k < 5 && i+k < SIZE && j+k < SIZE; ++k)
	// 	{
	// 		if (this->cb[i+k][j+k] != s) {
	// 			return false;
	// 		}
	// 	}
	// 	return true;
	// }
	// 返回胜利/失败的概率
	double getNodeValue(char p, int level = 0) // [-1,1]
	{
		char op = p == 'X' ? 'O' : 'X';
		// if (this->isGameOver()) {
		// 	char w = this->whoWins();
		// 	this->print();
		// 	printf("%c wins\n", w);
		// 	if (w == '-') return 0;
		// 	return (w == p) ? 1 : -1;
		// }
		if (level >= MAX_LEVEL)
		{
			// printf("level reached\n");
			return 0.0;
		}
		int _x = this->x, _y = this->y;
		vector<double> v;
		char next_p = this->isCurPlayer(op)? p : op;
		int child_i = 0;
		this->walk_empty([&](int i, int j) {
			// 不会离已有的棋子太远 距离2
			if (!in_battle_field(i, j)) {
				return;
			}
			this->play(i, j, next_p);
			this->print();
			char winner = win_by();
			double val;
			if (winner == '-')
			{
				val = getNodeValue(p, level+1);
			} else {
				val = winner == p ? 1.0 : -1.0;
			}
			// printf("level=%d, (%d,%d), %f, child_i:%dth\n", level, i,j,val,child_i++);
			// if (val > 0.51 || val < -0.51)
			// {
			// 	this->print();
			// 	printf("not 0! %c wins\n", this->whoWins());
			// 	exit(0);
			// }
			v.push_back(val);
			this->cb[i][j] = ' ';
		});
		this->x = _x; this->y = _y;
		assert(v.size() != 0);
		double sum_of_elems = 0.0;
		for (double val : v)
		    sum_of_elems += val;
		return sum_of_elems / v.size();
	}
	char win_by()
	{
		int i = this->x, j = this->y;
		char stone = this->cb[i][j];
		int s = 0;
		// row
		for (int jj = max(0, j-5+1); jj < min(SIZE,j+5); ++jj)
		{
			if (stone == this->cb[i][jj])
			{
				s++;
				if (s == 5)
				{
					return stone;
				}
			} else {
				s = 0;
			}
		}
		// col
		for (int ii = max(0,i-5+1), s = 0; ii < min(SIZE,i+5); ++ii)
		{
			if (stone == this->cb[ii][j])
			{
				s++;
				if (s == 5)
				{
					return stone;
				}
			} else {
				s = 0;
			}
		}
		// /,
		for (int k = max(max(-5, i-SIZE),-j),s=0; k < 5 && k <= i && k < SIZE-j; ++k)
		{
			if (stone == this->cb[i-k][j+k])
			{
				s++;
				if (s == 5)
				{
					return stone;
				}
			} else {
				s = 0;
			}
		}
		// \,
		for (int k = max(max(-5, -i),-j),s=0; k < 5 && k < SIZE - i && k < SIZE-j; ++k)
		{
			if (stone == this->cb[i+k][j+k])
			{
				s++;
				if (s == 5)
				{
					return stone;
				}
			} else {
				s = 0;
			}
		}
		return '-';
	}
	bool in_battle_field(int i, int j) {
		for (int ii = -2; ii <= 2; ++ii)
		{
			if (!in_board(i+ii))
			{
				continue;
			}
			for (int jj = -2; jj < 2 && in_board(j+jj); ++jj)
			{
				if (!in_board(j+jj))
				{
					continue;
				}
				if (this->cb[i+ii][j+jj] != ' ')
				{
					return true;
				}
			}
		}
		return false;
	}
	bool in_board(int i)
	{
		return 0 <= i && i < SIZE;
	}
	bool AI_MoveX()
	{
		if (this->x == -1)
		{
			this->play(6,6,'X');
			return true;
		}
		int _x = this->x, _y = this->y;
		map<double,tuple<int,int>> m;
		this->walk_empty([&](int i, int j) {
			// 不会离已有的棋子太远 距离2
			// printf("(%d,%d) in_battle_field=%d\n", i,j,in_battle_field);
			if (!in_battle_field(i, j)) {
				return;
			}
			this->play(i, j, 'X');
			m[getNodeValue('X')] = make_tuple(i, j);
			this->cb[i][j] = ' ';
		});
		this->x = _x; this->y = _y;
		auto maxp = m.rbegin();
		if (maxp->first < 0)
		{
			return false;
		}
		int i, j, v;
		tie(i,j) = maxp->second;
		this->play(i,j,'X');
		return true;
	}
};
typedef FIR_T<13> FIR;
