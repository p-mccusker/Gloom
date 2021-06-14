#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#elif _WIN32
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#endif

#include <map>
#include <array>
#include <iostream>
#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <tuple>
#include <cmath>
#include <ctime>
#include <memory>
#include <algorithm>


struct Color {
	Color(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) { r = red; g = green; b = blue; a = alpha; }
	Uint8 r, g, b, a;
};

const Color WHITE { 255, 255, 255, 255 };
const Color BLACK { 0,   0,   0,   255 };
const Color RED   { 255, 0,   0,   255 };
const Color GREEN { 0,   255, 0,   255 };
const Color BLUE  { 0,   0,   255, 255 };

enum TexIndex {
	EMPTY = 0, DWARF_1, DWARF_WARRIOR, HEART, DIAMOND_SHADED, CLUB, TREE, DIAMOND_FILLED, DIAMOND_INVERSE,
	CUBE, CUBE_INVERSE, BOY, GIRL, NOTE_SINGLES, NOTE_DOUBLE, START,
	//Row 2
	SHORT_ARRROW_LEF, SHORT_ARROW_RIGHT, ARROW_UP_DOWN, EXCLAMATION_DOUBLE, PARAGRAPH, NINJA_STAR, UNDERSCORE_THICK,
	ARROR_UP_DOWN_BAR, ARROW_UP, ARROW_DOWN, ARROW_RIGHT, ARROW_LEFT, RETURN, ARROW_LEFT_RIGHT, SHORT_ARROW_UP,
	SHORT_ARROW_DOWN,
	//Row 3
	EMPTY_2, EXCLAMTION, QUTOES_DOUBLE, POUND, DOLLAR, PERCENT, AMPERSTAND, QUOTE_SINGLE, PARENTHESIS_LEFT, PARENTHESIS_RIGHT,
	ASTERISK, PLUS, COMMA, MINUS, PERIOD, SLASH_FORWARD,
	//Row 4
	ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, COLON, SEMICOLON, LESS_THAN, EQUAL,
	GREATER_THAN, QUESTION,
	//Row 5
	AT, A_UP, B_UP, C_UP, D_UP, E_UP, F_UP, G_UP, H_UP, I_UP, J_UP, K_UP, L_UP, M_UP, N_UP, O_UP,
	//Row 6
	P_UP, Q_UP, R_UP, S_UP, T_UP, U_UP, V_UP, W_UP, X_UP, Y_UP, Z_UP, BRACKET_LEFT, SLASH_BACKWARD, BRACKET_RIGHT, CARET, UNDERSCORE,
	//Row 7
	BACKTICK, A_LOW, B_LOW, C_LOW, D_LOW, E_LOW, F_LOW, G_LOW, H_LOW, I_LOW, J_LOW, K_LOW, L_LOW, M_LOW, N_LOW, O_LOW,
	//Row 8
	P_LOW, Q_LOW, R_LOW, S_LOW, T_LOW, U_LOW, V_LOW, W_LOW, X_LOW, Y_LOW, Z_LOW, BRACE_LEFT, PIPE, BRACE_RIGHT, TILDE, PENTAGON,
	//Row 9
	// ADD REST LATER

};

inline double circleDistance(const double& x0, const double& y0, const double& x1, const double& y1) {
	return sqrt(pow((x1 - x0), 2) + pow((y1 - y0), 2));
}

template<typename T>
bool Contains(const std::vector<T>& arr, const T& item) {
	for (int i = 0; i < arr.size(); i++) {
		if (arr[i] == item)
			return true;
	}
	return false;
}

struct Coord {
	int x, y;
	bool operator==(const Coord& coord) const { return x == coord.x && y == coord.y; }
};

inline std::vector<Coord>  bresenham(int x1, int y1, int x2, int y2) {
	std::vector<Coord> coords;
	int m_new = 2 * (y2 - y1);
	int slope_error_new = m_new - (x2 - x1);
	for (int x = x1, y = y1; x <= x2; x++)
	{
		coords.emplace_back(x, y);
		// Add slope to increment angle formed
		slope_error_new += m_new;

		// Slope error reached limit, time to
		// increment y and update slope error.
		if (slope_error_new >= 0)
		{
			y++;
			slope_error_new -= 2 * (x2 - x1);
		}
		
	}
	return coords;
}

inline std::vector<Coord> dda(int x1, int y1, int x2, int y2) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	int xInc = dx / (float)steps;
	int yInc = dy / (float)steps;

	int x = x1, y = y1;
	std::vector<Coord> coords;

	for (int i = 0; i <= steps; i++) {
		Coord c;
		c.x = x;
		c.y = y;
		coords.push_back(c);
		x += xInc;
		y += yInc;

	}
	return coords;
}

struct random {
	time_t timer;
	uint32_t nLehmer;

	random() {
		time(&timer);
		srand(static_cast<long int>(timer));
		nLehmer = rand() % 2048;
	}

	uint32_t Lehmer32()
	{
		nLehmer += 0xe120fc15;
		uint64_t tmp;
		tmp = (uint64_t)nLehmer * 0x4a39b70d;
		uint32_t m1 = (tmp >> 32) ^ tmp;
		tmp = (uint64_t)m1 * 0x12fad5c9;
		uint32_t m2 = (tmp >> 32) ^ tmp;
		return m2;
	}

	int randNum(const int& start, const int& end) {
		if (start != end)
			return start + (Lehmer32() % (end - start));
		else
			return start;
	}

	template<typename ArrayType>
	ArrayType& Choice(std::vector<ArrayType>& arr) {
		int index = randNum(0, arr.size());
		return arr.at(index);
	}
};

inline random GENERATOR;




