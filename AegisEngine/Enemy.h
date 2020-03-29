#pragma once

#ifndef ENEMY_H
#define ENEMY_H

class GAME_OBJECT;
class CMODEL;

class ENEMY : public GAME_OBJECT {
private:
	CMODEL* Model;

	bool Fire_Flag;
	float Time;

public:
	ENEMY();
	~ENEMY();

	void Init() override;
	void Draw() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	void SetPosition(const XMFLOAT3& position);					// �|�W�V�����̐ݒ�
	void SetRotation(const XMFLOAT3& position);					// ��]�̐ݒ�
	void SetScaling(const XMFLOAT3& scaling);					// �g��k���̒l�̐ݒ�

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
	}
};

CEREAL_REGISTER_TYPE(ENEMY)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, ENEMY)

#endif // !ENEMY_H