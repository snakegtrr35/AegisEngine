#pragma once

#ifndef MATH_H
#define MATH_H

const int Get_Random(const int min, const int max);
bool Rand_Bool(const double probability);

// ���`���
// x1, y1 : �ŏ��l��x,y x2, y2 : �ő�l��x,y tx : ��Ԃ������l
float Lerp(const float x1, const float y1, const float x2, const float y2, const float tx);


short Loop_Plus(short tx, short max);
short Loop_Minus(short tx, short max);

#endif // !MATH_H