// Sudoku.cpp: 定义应用程序的入口点。
//

#include "Sudoku.h"
#include <cryptominisat5/cryptominisat.h>
#include <assert.h>
#include <vector>
#include <cmath>
using namespace std;
using namespace CMSat;

int main()
{
	SATSolver solver;
	solver.set_num_threads(8);
	vector<Lit> clause;

	solver.new_vars(9 * 9 * 4); // 每个数用4个比特表示

	// 元素取值范围为[1, 9]
	for (int row = 0; row < 9; row++) {
		for (int col = 0; col < 9; col++) {
			clause.push_back(Lit(row * 9 * 4 + col * 4 + 0, true));
			clause.push_back(Lit(row * 9 * 4 + col * 4 + 1, true));
			solver.add_clause(clause);
			clause.clear();
			clause.push_back(Lit(row * 9 * 4 + col * 4 + 0, true));
			clause.push_back(Lit(row * 9 * 4 + col * 4 + 2, true));
			solver.add_clause(clause);
			clause.clear();
			clause.push_back(Lit(row * 9 * 4 + col * 4 + 0, false));
			clause.push_back(Lit(row * 9 * 4 + col * 4 + 1, false));
			clause.push_back(Lit(row * 9 * 4 + col * 4 + 2, false));
			clause.push_back(Lit(row * 9 * 4 + col * 4 + 3, false));
			solver.add_clause(clause);
			clause.clear();
		}
	}

	int varNums = 9 * 9 * 4;
	vector<unsigned int> xor_clause;
	// 所有行内数字不同
	for (int row = 0; row < 9; row++) {
		for (int i = 0; i < 9; i++) {
			for (int j = i + 1; j < 9; j++) {
				solver.new_vars(4);
				for (int k = 0; k < 4; k++) {
					xor_clause.push_back(row * 9 * 4 + i * 4 + k);
					xor_clause.push_back(row * 9 * 4 + j * 4 + k);
					xor_clause.push_back(varNums + k);
					solver.add_xor_clause(xor_clause, false);
					xor_clause.clear();
					clause.push_back(Lit(varNums + k, false));
				}
				solver.add_clause(clause);
				clause.clear();
				varNums += 4;
			}
		}
	}
	// 所有列内数字不同
	for (int col = 0; col < 9; col++) {
		for (int i = 0; i < 9; i++) {
			for (int j = i + 1; j < 9; j++) {
				solver.new_vars(4);
				for (int k = 0; k < 4; k++) {
					xor_clause.push_back(i * 9 * 4 + col * 4 + k);
					xor_clause.push_back(j * 9 * 4 + col * 4 + k);
					xor_clause.push_back(varNums + k);
					solver.add_xor_clause(xor_clause, false);
					xor_clause.clear();
					clause.push_back(Lit(varNums + k, false));
				}
				solver.add_clause(clause);
				clause.clear();
				varNums += 4;
			}
		}
	}
	// 所有宫内数字不同
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			for (int i = 0; i < 9; i++) {
				for (int j = i + 1; j < 9; j++) {
					int i_x = i % 3, i_y = i / 3, j_x = j % 3, j_y = j / 3;
					solver.new_vars(4);
					for (int k = 0; k < 4; k++) {
						xor_clause.push_back(y * 3 * 9 * 4 + x * 3 * 4 + i_y * 9 * 4 + i_x * 4 + k);
						xor_clause.push_back(y * 3 * 9 * 4 + x * 3 * 4 + j_y * 9 * 4 + j_x * 4 + k);
						xor_clause.push_back(varNums + k);
						solver.add_xor_clause(xor_clause, false);
						xor_clause.clear();
						clause.push_back(Lit(varNums + k, false));
					}
					solver.add_clause(clause);
					clause.clear();
					varNums += 4;
				}
			}
		}
	}

	lbool ret = solver.solve();
	if (ret == l_True) {
		for (int row = 0; row < 9; row++) {
			for (int col = 0; col < 9; col++) {
				int num = 0;
				for (int k = 0; k < 4; k++) {
					if (solver.get_model()[row * 9 * 4 + col * 4 + k] == l_True) {
						num += pow(2, 3 - k);
					}
				}
				cout << num << " ";
			}
			cout << endl;
		}
	}
	else {
		cout << "Infeasible" << endl;
	}

	return 0;
}
