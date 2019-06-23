#include "stdafx.h"

extern HANDLE handle;
extern const int cols_offset;
extern int max_cols; // 4 cols
extern vector<cols> g;
extern vector<int> v_key_hit;
extern int max_hp;
extern int hp;

void ini() {
	CString s;
	s.Format(L"mode con cols=%d lines=%d", show_max_x, show_max_y);
	system(CW2A(s));
	CONSOLE_CURSOR_INFO cci = { 1, 0 };
	SetConsoleCursorInfo(handle, &cci);
	set_key_bind();
}

void set_key_bind() {
	switch (max_cols) {
	case 1:
		v_key_hit[0] = ' ';
		break;
	case 2:
		v_key_hit[0] = 'D';
		v_key_hit[1] = 'J';
		break;
	case 3:
		v_key_hit[0] = 'S';
		v_key_hit[1] = 'D';
		v_key_hit[2] = 'J';
		break;
	case 4:
		v_key_hit[0] = 'S';
		v_key_hit[1] = 'D';
		v_key_hit[2] = 'J';
		v_key_hit[3] = 'K';
		break;
	case 5:
		v_key_hit[0] = 'A';
		v_key_hit[1] = 'S';
		v_key_hit[2] = 'D';
		v_key_hit[3] = 'J';
		v_key_hit[4] = 'K';
		break;
	case 6:
		v_key_hit[0] = 'A';
		v_key_hit[1] = 'S';
		v_key_hit[2] = 'D';
		v_key_hit[3] = 'J';
		v_key_hit[4] = 'K';
		v_key_hit[5] = 'L';
		break;
	case 7:
		v_key_hit[0] = 'A';
		v_key_hit[1] = 'S';
		v_key_hit[2] = 'D';
		v_key_hit[3] = ' ';
		v_key_hit[4] = 'J';
		v_key_hit[5] = 'K';
		v_key_hit[6] = 'L';
		break;

	}


}