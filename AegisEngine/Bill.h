#pragma once

#ifndef BILL_H
#define BILL_H

#include "GameObject.h"

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

	template<class Archive>
	void save(Archive& archive) const
	{
		archive(cereal::make_nvp("GameObject", cereal::base_class<GameObject>(this)));
	}

	template<class Archive>
	void load(Archive& archive)
	{
		archive(cereal::make_nvp("GameObject", cereal::base_class<GameObject>(this)));
	}
};

CEREAL_REGISTER_TYPE(BILL)
CEREAL_REGISTER_POLYMORPHIC_RELATION(GameObject, BILL)

#endif // !BILL_H