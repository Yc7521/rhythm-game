#include "stdafx.h"

HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
const int cols_offset = 4;
int max_cols = 4; // 4 cols
vector<cols> g(max_cols);
vector<int> v_key_hit(max_cols);
vector <pair<unsigned char, clock_t>> v_key_hit_color(max_cols); // first: color, second: time need to set default(7)
int max_hp = 30;
int hp;
constexpr array<int, 4> level_preset{ 20,15,10,6 }; // easy: 20; normal: 15; hard: 10; very hard: 6;
constexpr array<const wchar_t*, 4> level_preset_str{ _T("easy"),_T("normal"),_T("hard"),_T("very hard") };
unsigned int level = 0; // a index in level_preset
bool end_need_random_ini = true;
bool real_music_game = false;
bool can_click_second_note = false;

cols::cols() {
}

cols::~cols() {
}

void cols::push(bool a) {
	if (a) {
		c.push_back(1);
	} else {
		c.push_back(0);
	}
}

// a: is down
// return: 0: right 1: wrong(down on null) 2: wrong(undown)
int cols::hit(bool a, bool force = false) {
	int& t = c.front();
	int& t2 = c[1];

	if (force) {
		if (t == 0) {
			pop();
			return a ? 1 : 0;
		} else if (t == 1) {
			pop();
			return a ? 0 : 2;
		} else {
			// nothing
			return 0;
		}
	} else {
		if (t == 0) {
			if (can_click_second_note && a && t2 == 1) {
				t2 = 0;
			}
			return 0;
		} else if (t == 1) {
			if (a) {
				t = 0; // down
				return 0;
			} else {
				return 2;
			}
		} else {
			// nothing
			return 0;
		}
	}
}

void cols::clear() {
	c.clear();
}

void cols::out(int offset_x, int lines) {
	int x = offset_x, y = lines;
	for (auto i : c) {
		gotoxy(x, y--);
		if (i == 1) {
			cout << _T("██");
		} else if (i == 0) {
			cout << _T("　　");
		}
		if (y < 0) {
			return;
		}
	}
	while (y >= 0) {
		gotoxy(x, y--);
		cout << _T("　　");
	}
}

void cols::out_(int offset_x, int lines) {
	int x = offset_x, y = 0;
	int i = 0;
	for (auto it = c.end() - 1, end = c.begin();; --it) {
		i = *it;
		gotoxy(x, y++);
		if (i == 1) {
			cout << _T("██");
		} else if (i == 0) {
			cout << _T("　　");
		}
		if (y >= lines) {
			return;
		}
		if (it == end)break;
	}
	while (y < lines) {
		gotoxy(x, y++);
		cout << _T("　　");
	}
}

void cols::pop() {
	c.pop_front();
}

bool cols::empty() {
	return c.empty();
}

void start() {
	ini();
	menu();
}

void start_() {
	end_need_random_ini = true;
	// clear
	for (int i = 0; i < max_cols; ++i) {
		g[i].clear();
	}
	// init
	int t = -1;
	for (int i = 0; i < max_ini_lines; ++i) {
		t = random_ini_(t);
	}
	hp = max_hp;
}

void game() {
	system("cls");
	out_bg();
	out_fg();
	out_info();
	CString str = _T("请按空格键开始游戏");
	gotoxy(show_max_x - 1 - str.GetLength() * 2, show_max_y - 3);
	cout << str;
	while (_getch() != 0x20);
	str = _T("                  ");
	gotoxy(show_max_x - 1 - str.GetLength(), show_max_y - 3);
	cout << str;
	clear_key();
	switchkey();
}

void game_over() {
	Sleep(500);
	clear_key();
	system("cls");
	cout << _T("game over") << endl;
	system("pause");
	return;
}

void menu() {
	int key;
	while (true) {
		system("cls");
		cout << _T("1: start game") << endl;
		cout << _T("2: setting") << endl;
		cout << _T("3: exit") << endl;
		key = _getch();
		switch (key) {
		case '1':
			start_();
			game();
			break;
		case '2':
			setting();
			break;
		case '3':case 27:case 3:
			return;
			break;
		}
	}
}

void out_bg() {
	int x1 = 0, x2 = 6 + max_cols * 6, y = 0;
	for (; y <= show_lines; ++y) {
		gotoxy(x1, y);
		cout << _T("│");
		gotoxy(x2, y);
		cout << _T("│");
	}
	gotoxy(x1, y);
	cout << _T("└");
	for (int i = x1 + 2; i < x2; i += 2) {
		gotoxy(i, y);
		cout << _T("┈");
	}
	gotoxy(x2, y++);
	cout << _T("┘");
	out_note_key();
}

void out_fg() {
	int x = cols_offset;
	for (auto i : g) {
		i.out(x, show_lines);
		x += 6;
	}
}

void out_note_key() {
	int x = cols_offset + 2;
	for (int i = 0; i < max_cols; ++i) {
		if (v_key_hit[i] == 32) {
			gotoxy(x - 2, show_lines + 2);
			if (v_key_hit_color[i].second < clock()) {
				v_key_hit_color[i].first = 7;
			}
			csout(_T("space"), v_key_hit_color[i].first);
		} else {
			gotoxy(x, show_lines + 2);
			if (v_key_hit_color[i].second < clock()) {
				v_key_hit_color[i].first = 7;
			}
			csout((wchar_t)v_key_hit[i], v_key_hit_color[i].first);
		}
		x += 6;
	}
}

void out_note_key(int n) {
	v_key_hit_color[n].second = clock() + 250;
	v_key_hit_color[n].first = 3;
	/*
		int x = 2 + cols_offset + 6 * n;
		gotoxy(x, show_lines + 2);
		wstring str;
		if (v_key_hit[n] == 32) {
			gotoxy(x - 2, show_lines + 2);
			str = _T("space");
		} else {
			str = (wchar_t)v_key_hit[n];
		}
		csout(str, 3);
	*/
}

void out_note_key_clear(int n) {
	int x = 2 + cols_offset + 6 * n;
	gotoxy(x, show_lines + 2);
	wstring str;
	if (v_key_hit[n] == 32) {
		gotoxy(x - 2, show_lines + 2);
		str = _T("space");
	} else {
		str = (wchar_t)v_key_hit[n];
	}
	cout << str;
}

void out_info() {
	CString str;
	str.Format(_T("HP: %-2d"), hp);
	gotoxy(show_max_x - 1 - str.GetLength(), 1);
	cout << str;
}

void setting() {
	int key;
	while (true) {
		system("cls");
		cout << boolalpha;
		cout << _T("1: start game with map") << endl;
		cout << _T("2: map edit") << endl;
		cout << _T("3: set real music: ") << real_music_game << endl;
		cout << _T("4: set level: ") << level_preset_str[level] << endl;
		cout << _T("5: set second note hit: ") << can_click_second_note << endl;
		cout << _T("6: back") << endl;
		key = _getch();
		switch (key) {
		case '1': {
			// clear
			for (int i = 0; i < max_cols; ++i) {
				g[i].clear();
			}
			hp = max_hp;
			ifstream f;
			cout << "please input a path to the map file" << endl;
			string str;
			getline(cin, str);
			if (str[0] == '"') {
				str.erase(str.size() - 1);
				str.erase(0);
			}
			f.open(str, ios::in | ios::binary);
			form_file_ini_(f);
			game();
		} break;
		case '2':
			input_map2file();
			break;
		case '3':
			real_music_game = !real_music_game;
			break;
		case '4':
			if (++level >= level_preset_str.size())level = 0;
			break;
		case '5':
			can_click_second_note = !can_click_second_note;
			break;
		case '6':
			return;
		}
	}
}

void switchkey() {
	int temp_last_ini = -1;
	int count_time = 0;
	vector<bool> old_key(max_cols);
	vector<bool> temp_hit(max_cols);
	// 1: no hit, but need refresh out
	// 2: no hit, no need refresh
	// 3: hit
	int no_hit_mark = 1;
	vector<bool> is_down(max_cols);

	auto reset_time = [&count_time] {
		//count_time = 0;
		count_time = clock();
	};
	auto judge_time = [&count_time](clock_t time) {
		//count_time = 0;
		auto now = clock();
		if (now - time > count_time) {
			count_time += time;
			return true;
		}
		return false;
	};
	clock_t down_time = static_cast<clock_t>(level_preset[level] * 50);

	for (auto&& i : old_key) {
		i = 0;
	}

	reset_time();
	while (true) {
		if (_kbhit()) {
			if (_getch() == 27) {
				return;
			}
		}
		if (no_hit_mark == 3) {
			// get click
			for (int i = 0; i < max_cols; ++i) {
				bool&& down_ = keydown(v_key_hit[i]);
				if (down_) {
					if (old_key[i] == 0) {
						out_note_key(i);
						old_key[i] = 1;
					} else {
						down_ = 0;
					}
				} else {
					if (old_key[i] == 1) {
						old_key[i] = 0;
						out_note_key_clear(i);
					}
				}
				temp_hit[i] = down_;
			}
			bool is_hit = false;
			for (auto i : temp_hit) {
				if (i) {
					is_hit = true;
					break;
				}
			}
			// if click then do click else 
			if (is_hit) {
				// all unclick note
				int undown = 0;
				for (int i = 0; i < max_cols; ++i) {
					switch (g[i].hit(temp_hit[i])) {
					case 2:
						++undown;
						break;
					}
				}
				// if all down then clear this line
				if (!real_music_game && undown == 0) {
					for (int i = 0; i < max_cols; ++i) {
						g[i].hit(false, true);
					}
					no_hit_mark = 1;
					reset_time();
				}
			} else {
				// down a line at time > level * 0.4 (default is *0.4)
				if (judge_time(down_time)) {
					int wrong = 0;
					for (int i = 0; i < max_cols; ++i) {
						switch (g[i].hit(false, true)) {
						case 1:case 2:
							++wrong;
							break;
						}
					}
					if (wrong > 0) {
						hp -= 1;
					}
					no_hit_mark = 1;
					out_fg();
					out_info();
					//reset_time();
				}
			}
			if (end_need_random_ini && no_hit_mark == 1)
				temp_last_ini = random_ini_(temp_last_ini);
			else
				no_hit_mark = 1;
			if (g[0].empty()) {
				return;
			}
			out_fg();
			out_info();
			//clear_key(); // useless code
		} else {
			if (no_hit_mark == 1) {
				out_note_key();
				for (auto&& i : old_key) {
					i = 0;
				}
				no_hit_mark = 2;
			}
			no_hit_mark = 3;
		}
		if (hp < 1) {
			game_over();
			return;
		}
		Sleep(15);
	}
}

void clear_key() {
	while (_kbhit()) {
		_getch();
	}
}

bool keydown(int vKey) {
	return (GetAsyncKeyState(vKey) & 0x8000) != 0;
}

int ini_(int col) {
	for (int i = 0; i < max_cols; ++i) {
		if (i == col) {
			g[i].push(true);
		} else {
			g[i].push(false);
		}
	}
	return col;
}

int random_ini_() {
	int&& i = random(0, max_cols - 1);
	return ini_(i);
}

int random_ini_(int except) {
	int&& i = random(0, max_cols - 1);
	while (i == except) {
		i = random(0, max_cols - 1);
	}
	return ini_(i);
}

// need open
void form_file_ini_(ifstream& f) {
	int t = -1;
	int s = 0;
	int j = 0;
	end_need_random_ini = false;
	f >> max_cols >> s;
	while (j++ < s) {
		for (int i = 0; i < max_cols; ++i) {
			f >> t;
			g[i].c.push_back(t);
		}
	}
	//j = 0;
	f.close();
}

void play_m(int ID) {
	auto&& r = FindResourceW(NULL, MAKEINTRESOURCE(ID), _T("MP3"));
	if (r != NULL) {
		auto&& s = SizeofResource(NULL, r);
		if (s > 0) {
			auto&& t = LoadResource(NULL, r);
			if (t != NULL) {
				auto&& b = LockResource(t);
				if (b != NULL) {
					CString str;
					system("@mkdir mp3");
					str.Format(_T("mp3\\%d.mp3"), ID);
					fstream f(str, ios_base::out | ios_base::binary);
					if (f.is_open()) {
						f.write((char*)b, s);
						CString str_ = _T("open ");
						str_ += _T("\"") + str + _T("\"") + _T(" alias sound");
						mciSendString(str_, NULL, 0, NULL);
						mciSendString(_T("play sound"), NULL, 0, NULL);
					}
				}
			}
		}
	}
}

void stop_m() {
	mciSendString(_T("stop sound"), NULL, 0, NULL);
}

void close_m() {
	mciSendString(_T("close sound"), NULL, 0, NULL);
}

void pause_m() {
	mciSendString(_T("pause sound"), NULL, 0, NULL);
}

void delete_mp3() {
	system("rd /s /q mp3");
}

int random(int a, int b) {
	uniform_int_distribution<int> s(a, b);
	static default_random_engine yq((UINT)time(NULL));
	return s(yq);
}

ostream& operator << (ostream& os, const wchar_t* wstr) {
	WriteConsoleW(handle, wstr, (DWORD)wcslen(wstr), NULL, NULL);
	return os;
}

ostream& operator << (ostream& os, const wchar_t wstr) {
	WriteConsoleW(handle, &wstr, 1, NULL, NULL);
	return os;
}

ostream& operator << (ostream& os, const wstring wstr) {
	WriteConsoleW(handle, wstr.c_str(), (DWORD)wcslen(wstr.c_str()), NULL, NULL);
	return os;
}

void gotoxy(short x, short y) {
	COORD coord = { x, y };
	SetConsoleCursorPosition(handle, coord);
}

void gotoxy(COORD a) {
	SetConsoleCursorPosition(handle, a);
}
//获取当前坐标
COORD getxy() {
	CONSOLE_SCREEN_BUFFER_INFO gb;
	GetConsoleScreenBufferInfo(handle, &gb);
	return gb.dwCursorPosition;
}

// 0:黑
// 1:蓝
// 2:绿
// 3:青
// 4:红
// 5:紫
// 6:黄
// 7:白
// +8:高亮
template <typename a>
void csout(a str, WORD color) {
	WORD colorOld;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(handle, &csbi);
	colorOld = csbi.wAttributes;
	SetConsoleTextAttribute(handle, 0x00f0 & colorOld | color);
	cout << str;
	SetConsoleTextAttribute(handle, colorOld);
}

// 0:黑
// 1:蓝
// 2:绿
// 3:青
// 4:红
// 5:紫
// 6:黄
// 7:白
// +8:高亮
template <typename a>
void csout(a str, WORD Tcolor, WORD BGcolor) {
	WORD colorOld;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(handle, &csbi);
	colorOld = csbi.wAttributes;
	Tcolor |= BGcolor << 4;
	SetConsoleTextAttribute(handle, Tcolor);
	cout << str;
	SetConsoleTextAttribute(handle, colorOld);
}

// wstring => string
string WS2S(const wstring& ws) {
	auto old_locale = setlocale(LC_ALL, "");
	const wchar_t* wstr_c = ws.c_str();
	size_t size;
	wcstombs_s(&size, NULL, 0, wstr_c, ws.size());
	char* str_c = new char[size];
	memset(str_c, 0, size);
	wcstombs_s(&size, str_c, size, wstr_c, ws.size());
	string strResult = str_c;
	delete[]str_c;
	setlocale(LC_ALL, old_locale);
	return strResult;
}

// string => wstring
wstring S2WS(const string& s) {
	auto old_locale = setlocale(LC_ALL, "");
	const char* str_c = s.c_str();
	size_t size;
	mbstowcs_s(&size, NULL, 0, str_c, s.size());
	wchar_t* wstr_c = new wchar_t[size];
	wmemset(wstr_c, 0, size);
	mbstowcs_s(&size, wstr_c, size, str_c, s.size());
	wstring wstrResult = wstr_c;
	delete[]wstr_c;
	setlocale(LC_ALL, old_locale);
	return wstrResult;
}

void input_map2file() {
	AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0);
	system("cls");

	out_bg();

	vector<cols> cols(max_cols);
	string filename;
	map<int, function<void()>> my_switch;

	auto show_block = [&] {
		int x = cols_offset;
		for (auto i : cols) {
			i.out_(x, show_lines);
			x += 6;
		}
	};
	auto save = [&] {
		ofstream file(filename, ios::out | ios::binary);
		file << max_cols << '\t' << cols[0].c.size() << '\n';
		auto s = cols[0].c.size();
		for (decltype(s) j = 0; j < s; j++) {
			for (int i = 0; i < max_cols; ++i) {
				auto&& t = cols[i].c[j];
				file << t << '\t';
			}
		}
	};
	auto add = [&](int col) {
		for (int i = 0; i < max_cols; ++i) {
			if (i == col) {
				cols[i].push(true);
			} else {
				cols[i].push(false);
			}
		}
	};
	auto remove_end = [&] {
		for (int i = 0; i < max_cols; ++i) {
			auto&& a = cols[i].c;
			a.erase(a.end() - 1);
		}
	};
	CFileDialog f(0, L"map", L"map", 6, L"map file (*.map)|*.map||");
	auto r = f.DoModal();
	filename = CW2A(f.GetPathName());

	if (r == IDOK) {
		my_switch[3] = my_switch[27] = my_switch[13] = my_switch[10] = save;
		for (int i = 0; i < max_cols; ++i) {
			my_switch[tolower(v_key_hit[i])] = my_switch[v_key_hit[i]] = [now = i, &add] {
				add(now);
			};
		}
		my_switch[127] = my_switch[8] = remove_end;

		while (true) {
			try {
				int key = _getch();
				my_switch.at(key)();
				if (key == 3 || key == 27 || key == 10 || key == 13)return;
				show_block();
			} catch (const std::exception&) {
			}
		}
	}
}