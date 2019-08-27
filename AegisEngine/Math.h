#pragma once

#ifndef MATH_H
#define MATH_H

const int Get_Random(const int min, const int max);
bool Rand_Bool(const double probability);

// 線形補間
// x1, y1 : 最小値のx,y x2, y2 : 最大値のx,y tx : 補間したい値
float Lerp(const float x1, const float y1, const float x2, const float y2, const float tx);


short Loop_Plus(short tx, short max);
short Loop_Minus(short tx, short max);

#endif // !MATH_H