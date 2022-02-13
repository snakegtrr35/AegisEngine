#pragma once

#ifndef COLLISION_H
#define	COLLISION_H

// 円
struct CIRCLE {
	aegis::Vector2 Position;		// 中心座標
	float Radius;			// 半径
};

// 球
struct SPHERE {
	aegis::Vector3 Position;		// 中心座標
	float Radius;			// 半径
};

// カプセル
struct CAPSULE {
	aegis::Vector3 Start_Position;	// 線分の開始点
	aegis::Vector3 End_Position;		// 線分の終了点
	float Radius;				// 半径

	aegis::Vector3 Rotation;			// 回転量

	CAPSULE() : Radius(0.5f), Start_Position(aegis::Vector3(0.f, -0.5f, 0.f)), End_Position(aegis::Vector3(0.f, 0.5f, 0.f)), Rotation(aegis::Vector3(0.f, 0.f, 0.f)) {}

	void Transform(const aegis::Vector3& position, const float height) {
		XMVECTOR vector;
		XMMATRIX matrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));

		{
			Start_Position.x = position.x;
			Start_Position.y = position.y - height;
			Start_Position.z = position.z;

			// 元の座標に戻す
			vector = XMLoadFloat3(&Start_Position);

			XMStoreFloat3(&Start_Position, XMVector3Transform(vector, matrix));
		}

		{
			End_Position.x = position.x;
			End_Position.y = position.y + height;
			End_Position.z = position.z;

			// 元の座標に戻す
			vector = XMLoadFloat3(&End_Position);

			XMStoreFloat3(&End_Position, XMVector3Transform(vector, matrix));
		}
	}

	void Rotate(const aegis::Vector3& rotation) {
		XMVECTOR vector;
		XMMATRIX matrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(rotation.x - Rotation.x), XMConvertToRadians(rotation.y - Rotation.y), XMConvertToRadians(rotation.z - Rotation.z));

		{
			vector = XMLoadFloat3(&Start_Position);

			XMStoreFloat3(&Start_Position, XMVector3Transform(vector, matrix));
		}

		{
			vector = XMLoadFloat3(&End_Position);

			XMStoreFloat3(&End_Position, XMVector3Transform(vector, matrix));
		}

		Rotation = rotation;
	}

	void Set_Height(const float height) {
		
		XMVECTOR vector;
		XMMATRIX matrix = XMMatrixRotationRollPitchYaw(XMConvertToRadians(Rotation.x), XMConvertToRadians(Rotation.y), XMConvertToRadians(Rotation.z));
		XMMATRIX invers = XMMatrixRotationRollPitchYaw(XMConvertToRadians(-Rotation.x), XMConvertToRadians(-Rotation.y), XMConvertToRadians(-Rotation.z));

		{
			// 初期の座標に戻して計算
			vector = XMLoadFloat3(&Start_Position);

			XMStoreFloat3(&Start_Position, XMVector3Transform(vector, invers));

			Start_Position.y -= height;

			// 元の座標に戻す
			vector = XMLoadFloat3(&Start_Position);

			XMStoreFloat3(&Start_Position, XMVector3Transform(vector, matrix));
		}

		{
			// 初期の座標に戻して計算
			vector = XMLoadFloat3(&End_Position);

			XMStoreFloat3(&End_Position, XMVector3Transform(vector, invers));

			End_Position.y += height;

			// 元の座標に戻す
			vector = XMLoadFloat3(&End_Position);

			XMStoreFloat3(&End_Position, XMVector3Transform(vector, matrix));
		}
	}
};

// AABB
struct AABB {
	aegis::Vector3 Position;		// 中心座標
	aegis::Vector3 Radius;		// 半径
};

// OBB
struct OBB {
	aegis::Vector3 Position;		// 中心座標
	aegis::Vector3 Radius;		// 半径
	aegis::Vector3 Rotation;		// 回転量

};

// コリジョンクラス
class COLLISION  {
protected:
	
	std::string Name;
	std::string ParentName;
	aegis::unordered_map<std::string, COLLISION*> Childlen;

public:
	COLLISION() : Name("Collision"), ParentName("None") {}
	virtual ~COLLISION() {}

	virtual void Update(float delta_time) = 0;
	virtual void Draw() = 0;

	void Set_Name(const std::string& name) {
		Name = name;
	}

	std::string& const Get_Name() {
		return Name;
	}

	void Set_Parent_Name(const std::string& parent_name) {
		ParentName = parent_name;
	}

	std::string& const Get_Parent_Name() {
		return ParentName;
	}



	//void Set_Childt_Name(const std::string& child_name) {
	//	ChildlenName.push_back(child_name);
	//};

	//vector<std::string> const Get_Childlen_Name() {
	//	return ChildlenName;
	//};
};

// コリジョン(球)
class COLLISIION_SPHERE : public COLLISION {
private:
	SPHERE Sphere;

protected:

public:
	COLLISIION_SPHERE() {}
	~COLLISIION_SPHERE() {}

	void Update(float delta_time) override;
	void Draw() override;

	void Set_Sphper(const SPHERE& sphere) {
		Sphere = sphere;
	}

	SPHERE* const Get_Sphper() {
		return &Sphere;
	}

	void Set_Position(const aegis::Vector3& position) {
		Sphere.Position = position;
	}

	void Set_Radius(const float radius) {
		Sphere.Radius = radius;
	}
};

bool Collision_HitCircle(CIRCLE* const pA, CIRCLE* const pB);
//bool Collision_HitCircle(const CIRCLE* pA, const CIRCLE* pB, short cnt);

bool Collision_HitSphere(COLLISION* const pA, COLLISION* const pB);
//bool Collision_HitSphere(const SPHERE* pA, const SPHERE* pB, short cnt);

// コリジョン(AABB)
class COLLISIION_AABB : public COLLISION {
private:
	AABB Aabb;

public:
	COLLISIION_AABB() {};
	~COLLISIION_AABB() {};

	void Update(float delta_time) override {};
	void Draw() override {};

	void Set_AABB(const AABB& aabb) {
		Aabb = aabb;
	};

	AABB* const Get_AABB() {
		return &Aabb;
	};

	void Set_Position(const aegis::Vector3& position) {
		Aabb.Position = position;
	};

	void Set_Radius(const aegis::Vector3& radius) {
		Aabb.Radius = radius;
	};
};

bool Collision_HitAABB(COLLISION* const pA, COLLISION* const pB);

// コリジョン(CAPSULE)
class COLLISIION_CAPSULE : public COLLISION {
private:
	CAPSULE Capsule;
	//BOUNDING_CAPSULE Bounding_Capsule;
	float Height;

public:

	COLLISIION_CAPSULE() : Height(1.0f) {}

	COLLISIION_CAPSULE(aegis::Vector3 position, float radius, float height) : Height(height) {
		Capsule.Radius = radius;

		Capsule.Start_Position.x = position.x;
		Capsule.Start_Position.y = position.y - Height;
		Capsule.Start_Position.z = position.z;

		Capsule.End_Position.x = position.x;
		Capsule.End_Position.y = position.y + Height;
		Capsule.End_Position.z = position.z;
	}

	~COLLISIION_CAPSULE() {};

	void Update(float delta_time) override;
	void Draw() override;

	CAPSULE* const Get_CAPSULE() {
		return &Capsule;
	};

	void Set_Radius(const float radius) {
		Capsule.Radius = radius;
	};

	float Get_Radius() {
		return Capsule.Radius;
	};

	void Set_Height(const float height) {
		Height = height;

		Capsule.Set_Height(Height);
	};

	float Get_Height() {
		return Height;
	};

	void Set_Rotation(const aegis::Vector3& rotation) {
		Capsule.Rotate(rotation);
	}

	aegis::Vector3 Get_Rotation() {
		return Capsule.Rotation;
	}

	void Set_Position(const aegis::Vector3& position) {
		Capsule.Transform(position, Height);
	}

	aegis::Vector3 Get_Position() {
		aegis::Vector3 pos = Capsule.Start_Position;

		pos.y += Height;

		return pos;
	}

};

bool Collision_HitCAPSULE(CAPSULE* const pA, CAPSULE* const pB);

#endif // !COLLISION_H