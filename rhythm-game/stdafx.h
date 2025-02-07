#pragma once

#include "targetver.h"
#include "resource.h"
#include <afx.h>
#include <afxdlgs.h>
#include <stdio.h>
#include <tchar.h>
#include <bits/stdc++.h>
#include <conio.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace std;

constexpr int show_lines = 30;
constexpr int max_ini_lines = 2 * show_lines; // init 60 lines when it start
constexpr int show_max_x = 120, show_max_y = (int)(show_lines * 1.2);

class cols {
public:
	cols();
	~cols();
	void push(bool a);
	int hit(bool a, bool force);
	void clear();
	void out(int offset_x, int lines);
	void out_(int offset_x, int lines);
	bool empty();
	friend void input_map2file();
	friend void form_file_ini_(ifstream&);
private:
	deque<int> c;
	void pop();
};

void start();
void start_();
void game();
void game_over();
void menu();
void out_bg();
void out_fg();
void out_note_key();
void out_note_key(int n);
void out_note_key_clear(int n);
void out_info();
void setting();
void switchkey();
void clear_key();
bool keydown(int vKey);
void ini();
int random_ini_();
int random_ini_(int except);
void play_m(int ID);
void stop_m();
void close_m();
void pause_m();
void delete_mp3();
int random(int a, int b);
ostream & operator<<(ostream & os, const wchar_t * wstr);
ostream & operator<<(ostream & os, const wchar_t wstr);
ostream & operator<<(ostream & os, const wstring wstr);
void gotoxy(short x, short y);
void gotoxy(COORD a);
COORD getxy();
template <typename a>void csout(a str, WORD color);
template <typename a>void csout(a str, WORD Tcolor, WORD BGcolor);

void input_map2file();
void form_file_ini_(ifstream&);
void set_key_bind();
