#pragma once

#ifndef BILL_H
#define BILL_H

class GAME_OBJECT;
class CMODEL;

class BILL : public GAME_OBJECT {
private:

	unique_ptr<CMODEL> Model;

public:
	BILL();
	~BILL();

	void Init() override;
	void Draw() override;
	void Draw_Shadow() override;
	void Draw_DPP() override;
	void Update(float delta_time) override;
	void Uninit() override;

	template<typename Archive>
	void serialize(Archive& ar)
	{
		ar(cereal::base_class<GAME_OBJECT>(this));
		//ar(Texture);
	}
};

CEREAL_REGISTER_TYPE(BILL)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GAME_OBJECT, BILL)

#endif // !BILL_H