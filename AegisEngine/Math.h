/**
 * @file Math.h
 * @brief 算術関係のヘッダーファイル
 */

#pragma once

#ifndef MATH_H
#define MATH_H

namespace Math
{
	// 乱数関係の関数

	// 一様乱数
	const int Get_Random(const int min, const int max);
	const float Get_Random(const float min, const float max);
	const double Get_Random(const double min, const double max);
	// trueとfalseという2つの値を確率分布させる関数
	// 確率( probability )でtrueが生成され、確率( 1.0 - probability )でfalseが生成される
	const bool Random_Bool(const float probability);

	// 線形補間
	// x1, y1 : 最小値のx,y x2, y2 : 最大値のx,y tx : 補間したい値
	float Lerp(const float x1, const float y1, const float x2, const float y2, const float tx);

	// 変数のループ処理
	short Loop_Plus(short tx, short max);
	short Loop_Minus(short tx, short max);
}

using namespace Math;

#endif // !MATH_H