#pragma once

#ifdef __linux__
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#elif _WIN32
#include <SDL.h>
#include <SDL_image.h>
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
    EMPTY_2, EXCLAMTION, QUTOES_DOUBLE, POUND, DOLLAR, PERCENT, AMPERSTAND, QUTOE_SINGLE, PARENTHESIS_LEFT, PARENTHESIS_RIGHT,
    ASTERISK, PLUS, COMMA, MINUS, PERIOD, SLASH_FORWARD,
    //Row 4
    ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, COLON, SEMICOLON, BIG_SHORT_ARROW_LEFT, EQUAL,
    BIG_SHORT_ARROW_RIGHT, QUESTION,
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

double circleDistance(const double& x0, const double& y0, const double& x1, const double& y1);



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
        return start + (Lehmer32() % (end - start));
    }

    template<typename ArrayType>
    ArrayType Choice(ArrayType* arr, const unsigned int& size) {
        int index = randNum(0, size);
        
        auto item = (arr + index);
        return *item;
    }
};




