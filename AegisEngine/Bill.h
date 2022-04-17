#pragma once

#ifndef BILL_H
#define BILL_H

class GameObject;

class BILL : public GameObject {

	OBJECT_TYPE_INFO(BILL)

private:

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
		ar(cereal::base_class<GameObject>(this));
		//ar(Texture);
	}
};

CEREAL_REGISTER_TYPE(BILL)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, BILL)

#endif // !BILL_H