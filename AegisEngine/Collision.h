#pragma once

#ifndef COLLISION_H
#define	COLLISION_H

// 円
typedef	struct {
	XMFLOAT2 Position;		// 中心座標
	float Radius;			// 半径
}CIRCLE;

// 球
typedef	struct {
	XMFLOAT3 Position;		// 中心座標
	float Radius;			// 半径
}SPHERE;

// カプセル
typedef	struct {
	XMFLOAT3 Position;		// 中心座標
	float Radius;			// 半径
}CAPSILE;

// AABB
typedef struct {
	XMFLOAT3 Position;		// 中心座標
	XMFLOAT3 Radius;		// 半径
}AABB;

// OBB
typedef struct {
	XMFLOAT3 Position;		// 中心座標
	XMFLOAT3 Radius;		// 半径

}OBB;

// コリジョンクラス
class COLLISION  {
private:

protected:
	  string Name;
	  string ParentName;
	  map<string, COLLISION*> Childlen;

public:
	COLLISION() {
		Name = "Collision";
		ParentName = "None";
	};
	virtual ~COLLISION() {};



	void Set_Name(const string& name) {
		Name = name;
	};

	string& const Get_Name() {
		return Name;
	};



	void Set_Parent_Name(const string& parent_name) {
		ParentName = parent_name;
	};

	string& const Get_Parent_Name() {
		return ParentName;
	};



	//void Set_Childt_Name(const string& child_name) {
	//	ChildlenName.push_back(child_name);
	//};

	//vector<string> const Get_Childlen_Name() {
	//	return ChildlenName;
	//};
};

// コリジョン(球)
class COLLISIION_SPHERE : public COLLISION {
private:
	SPHERE Sphere;

protected:

public:
	COLLISIION_SPHERE() {};
	~COLLISIION_SPHERE() {};

	void Set_Sphper(const SPHERE& sphere) {
		Sphere = sphere;
	};

	SPHERE* const Get_Sphper() {
		return &Sphere;
	};

	void Set_Position(const XMFLOAT3& position) {
		Sphere.Position = position;
	};

	void Set_Radius(const float radius) {
		Sphere.Radius = radius;
	};
};

bool Collision_HitCircle(CIRCLE* const pA, CIRCLE* const pB);
//bool Collision_HitCircle(const CIRCLE* pA, const CIRCLE* pB, short cnt);

bool Collision_HitSphere(COLLISION* const pA, COLLISION* const pB);
//bool Collision_HitSphere(const SPHERE* pA, const SPHERE* pB, short cnt);

// コリジョン(AABB)
class COLLISIION_AABB : public COLLISION {
private:
	AABB Aabb;

protected:

public:
	COLLISIION_AABB() {};
	~COLLISIION_AABB() {};

	void Set_AABB(const AABB& aabb) {
		Aabb = aabb;
	};

	AABB* const Get_AABB() {
		return &Aabb;
	};

	void Set_Position(const XMFLOAT3& position) {
		Aabb.Position = position;
	};

	void Set_Radius(const XMFLOAT3& radius) {
		Aabb.Radius = radius;
	};
};

bool Collision_HitAABB(COLLISION* const pA, COLLISION* const pB);

#endif // !COLLISION_H