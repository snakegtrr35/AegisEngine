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
	/**
	* @brief 確率分布
	* @param probability 確率( probability )
	* @return bool true 又は false
	* @details 確率( probability )でtrueが生成され、確率( 1.0 - probability )でfalseが生成される関数
	*/
	const bool Random_Bool(const float probability);

	/**
	* @brief 線形補間
	* @param y1 最小値の時の値
	* @param y2 最大値の時の値
	* @param tx 補間したい値
	* @param x1 線形の最小値(デフォルトで0.0f)
	* @param x2 線形の最大値(デフォルトで1.0f)
	* @return float 補間した値
	* @details 線形補間を行い関数
	*/
	float Lerp(const float y1, const float y2, const float tx, const float x1 = 0.f, const float x2 = 1.0f);

	// 変数のループ処理
	short Loop_Plus(short tx, short max);
	short Loop_Minus(short tx, short max);
}

using namespace Math;

#endif // !MATH_H