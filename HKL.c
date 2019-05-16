/*
	HKL clustering algorithm
	A implememtation with an example.
	
	Author: arena
*/
#include <stdio.h>

void print_matrix();
int row_sum(int r, int cs, int ce);
int col_sum(int c, int rs, int re);
void sort_rows();
void sort_cols();
int row_group_size();
int col_group_size();

#define ROW 12
#define COL 9

int R = ROW / 2;
int C = COL / 2;

int t[ROW][COL] = {{0,1,0,0,0,1,0,0,0},
                   {0,0,1,0,1,0,0,1,1},
                   {0,0,1,0,1,0,0,0,0},
                   {1,0,0,0,0,0,0,0,0},
                   {0,0,0,1,0,0,0,1,1},
                   {0,1,0,0,0,1,0,0,0},
                   {0,0,0,1,0,0,0,0,0},
                   {1,0,0,0,0,1,0,0,1},
                   {0,0,1,0,1,0,0,0,0},
                   {0,0,0,1,0,0,0,1,0},
                   {0,1,0,0,1,0,1,0,0},
                   {0,0,1,0,0,1,0,0,1}};

int row_idx[ROW], row_group[ROW];
int col_idx[COL], col_group[COL];


void init()
{	
    for(int r = 0; r < ROW; r ++) {
    	row_idx[r] = r;
    	row_group[r] = 1;
    }
    
    for(int c = 0; c < COL; c ++) {
    	col_idx[c] = c;
    	col_group[c] = 1;
    }
}

int row_group_size()
{
	int size = 0;
	for(int r = 0; r < ROW; r++) {
		if (row_group[r] == 1)
			size += 1;
	}
	return size;
}

int col_group_size()
{
	int size = 0;
	for (int c = 0; c < COL; c ++) {
		if (col_group[c] == 1)
			size += 1;
	}
	return size;
}



void print_matrix()
{
	printf("     ");
	for(int c = 0; c < COL; c ++) {
		if (c == C) {
			printf("C%d   ", C);
		}
		printf("T%-2d ", col_idx[c] + 1); 

	}
	putchar('\n');
	
	for(int r = 0; r < ROW; r ++) {
		if (r == R) {
		printf("R%-2d:----------------------------------------\n", R);
		}
		printf("F%-2d: ", row_idx[r] + 1); 
		for(int c = 0; c < COL; c ++) {
			if (c == C) {
				printf(" |   ");
			}
			printf(" %-3d", t[r][c]);

		}
		putchar('\n');
	}
	putchar('\n');
}


// ----------------------------------------------------------------------
int main()
{

	int row_ok, col_ok;	
	int iter = 1;
	
	init();
	print_matrix();	
	do { 
		printf("\n************ Iteration %d *************\n", iter);
		row_ok = 1;
		col_ok = 1;


		for(int r = 0; r < ROW; r ++) {
			int row_in_1 = row_sum(r, 0, C);
			int row_in_2 = row_sum(r, C, COL);

			if (row_in_1 > row_in_2) {
				if (row_group[r] != 1) row_ok = 0;

				row_group[r] = 1;
			} else if (row_in_1 < row_in_2) {
				if (row_group[r] != 2) row_ok = 0;
				row_group[r] = 2;
			} else {
				if ( R <= ROW / 2) {
					if (row_group[r] != 1) row_ok = 0;
					row_group[r] = 1;
				} else {
					if (row_group[r] != 2) row_ok = 0;
					row_group[r] = 2;
				}
			}
		}
		
		sort_rows();
		R = row_group_size();
		print_matrix();


		for(int c = 0; c < COL; c ++) {
			int col_in_1 = col_sum(c, 0, R);
			int col_in_2 = col_sum(c, R, ROW);
			if (col_in_1 > col_in_2) {
				if (col_group[c] != 1) col_ok = 0;
				col_group[c] = 1;
			} else if (col_in_1 < col_in_2) {
				if (col_group[c] != 2) col_ok = 0;
				col_group[c] = 2;
			} else {
				if ( C <= COL / 2) {
					if (col_group[c] != 1) col_ok = 0;
					col_group[c] = 1;
				} else {
					if (col_group[c] != 2) col_ok = 0;
					col_group[c] = 2;
				}
			}
		}
		
		sort_cols();
		C = col_group_size();
		print_matrix();	

		iter ++;
	} while (!(row_ok && col_ok));
	
    return 0;
}




void sort_rows()
{
	int group_size = row_group_size();
	int temp_rows[ROW - group_size][ROW];
	int temp_idx[ROW - group_size];
	// save rows in group 2 in temp_rows
	for(int r = 0, i = 0; r < ROW; r++) {
		if (row_group[r] == 2) {
			for(int c = 0; c < COL; c++) {
				temp_rows[i][c] = t[r][c];
			}
			temp_idx[i] = row_idx[r];
			i ++;
		}
	}
	// reorder rows in matrix for group 1
	for(int r = 0, i = 0; r < ROW; r++) {
		if (row_group[r] == 1) {
			for(int c = 0; c < COL; c ++) {
				t[i][c] = t[r][c];
			}
			row_idx[i] = row_idx[r];
			row_group[i] = 1;
			i ++;
		} 
	}
	// append rows in temp to matrix after group 1
	for(int r = 0; r < ROW - group_size; r ++) {
		for(int c = 0; c < COL; c ++) {
			t[group_size + r][c] = temp_rows[r][c];
		}
		row_idx[group_size + r] = temp_idx[r];
		row_group[group_size + r] = 2;
	}
}

void sort_cols()
{
	int group_size = col_group_size();
	int temp_cols[COL - group_size][ROW];
	int temp_idx[COL - group_size];
	// save cols in group 2 in temp_cols
	for(int c = 0, i = 0; c < COL; c ++) {
		if (col_group[c] == 2) {
			for(int r = 0; r < ROW; r++) {
				temp_cols[i][r] = t[r][c];
			}
			temp_idx[i] = col_idx[c];
			i ++;
		}
	}
	
	// reorder cols in matrix for group 1
	for(int c = 0, i = 0; c < COL; c ++) {
		if (col_group[c] == 1) {
			for(int r = 0; r < ROW; r ++ ) {
				t[r][i] = t[r][c];
			}
			col_idx[i] = col_idx[c];
			col_group[i] = 1;
			i ++;
		}
	}

	
	// append cols in temp to matrix after group 1
	for(int c = 0; c < COL - group_size; c++) {
		for(int r = 0; r < ROW; r++ ) {
			t[r][group_size + c] = temp_cols[c][r];
		}
		col_idx[group_size + c] = temp_idx[c];
		col_group[group_size + c] = 2;
	}
	
}



int row_sum(int r, int cs, int ce)
{
	int sum = 0;
	int c = cs;
	while (c < ce) {
		sum += t[r][c];	
		c ++;
	}
	return sum;
}

int col_sum(int c, int rs, int re)
{
	int sum = 0;
	int r = rs;
	while (r < re){
		sum += t[r][c];
		r ++;
	}
	return sum;
}



