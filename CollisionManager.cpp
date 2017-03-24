#include "CollisionManager.h"



// Initialize instance to null
CollisionManager* CollisionManager::instance = nullptr;


CollisionManager * const CollisionManager::Initialize()
{
	// Ensure not already initialized
	assert(instance == nullptr);

	// Initialize renderer
	instance = new CollisionManager();

	// return instance after init
	return instance;
}

CollisionManager * const CollisionManager::Instance()
{
	// Ensure initialized
	assert(instance != nullptr);

	// Return our instance
	return instance;
}

void CollisionManager::Shutdown()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void CollisionManager::StageCollider(Collider * const c)
{
	colliderVector.push_back(c);
}

void CollisionManager::UnstageCollider(Collider * const c)
{
	//remove from whatever list is being used
	for (int i = colliderVector.size - 1; i >= 0; i--) {
		if (colliderVector[i] == c) {
			//colliderVector.erase(c);
		}
	}
}

void CollisionManager::CollisionUpdate()
{
	//add to grid
	grid.clear();
	for (int i = 0; i < colliderVector.size(); i++) {
		//Collider* obj = (Collider*)collidableTags[i];
		Collider* obj = colliderVector[i];
		grid.insert(obj->GetPosition(), *obj->GetScale(), obj);
	}

	//check for collisions
	std::unordered_map<int, std::list<void*>>& map = grid.getMapRef();
	//grid iterator
	for (auto iterl = map.begin(); iterl != map.end(); ++iterl) {

		std::list<void*>& bin = (*iterl).second;//list

												//list iterator
		for (auto iteri = bin.begin(); iteri != bin.end(); iteri) {

			Collider* obji = (Collider*)(*iteri);//1st object

			++iteri;
			//list iterator 2
			for (auto iterj = iteri; iterj != bin.end(); ++iterj) {

				Collider* objj = (Collider*)(*iterj);//2nd object
				if (collides(*obji, *objj))
				{
					//create a call to a collision function: in Entity or pass a call somehow to decide how to deal with the collision

					/*
					if (obji->collisionScriptMap.count(objj->tag))
						obji->collisionScriptMap[objj->tag](this, obji, objj);

					if (objj->collisionScriptMap.count(obji->tag))
						objj->collisionScriptMap[obji->tag](this, objj, obji);
					/**/
					printf("objects colliding");
				}
			}
		}
	}
}

CollisionManager::CollisionManager()
{
	//eh...
	grid = Grid(1, XMFLOAT3(1,1,1));
}

CollisionManager::CollisionManager(float maxScale, XMFLOAT3 gridHalfWidth)
{
	//instantiate grid
	grid = Grid(maxScale, gridHalfWidth);
}


CollisionManager::~CollisionManager()
{
}

void CollisionManager::CollisionInit()
{
	collisionTable[{Collider::SPHERE, Collider::SPHERE}] = &collidesSpherevSphere;
	collisionTable[{Collider::SPHERE, Collider::AABB}] = &collidesSpherevAABB;
	collisionTable[{Collider::AABB, Collider::SPHERE}] = &collidesAABBvSphere;
	collisionTable[{Collider::AABB, Collider::AABB}] = &collidesAABBvAABB;
	collisionTable[{Collider::OBB, Collider::OBB}] = &collidesOBBvOBB;
	collisionTable[{Collider::OBB, Collider::SPHERE}] = &collidesOBBvSphere;
	collisionTable[{Collider::SPHERE, Collider::OBB}] = &collidesSpherevOBB;
	collisionTable[{Collider::OBB, Collider::AABB}] = &collidesOBBvAABB;
	collisionTable[{Collider::AABB, Collider::OBB}] = &collidesAABBvOBB;
	collisionTable[{Collider::HALFVOL, Collider::AABB}] = &collidesHalfvolvCollider;
	collisionTable[{Collider::HALFVOL, Collider::OBB}] = &collidesHalfvolvCollider;
	collisionTable[{Collider::HALFVOL, Collider::SPHERE}] = &collidesHalfvolvCollider;
	collisionTable[{Collider::AABB, Collider::HALFVOL}] = &collidesCollidervHalfvol;
	collisionTable[{Collider::OBB, Collider::HALFVOL}] = &collidesCollidervHalfvol;
	collisionTable[{Collider::SPHERE, Collider::HALFVOL}] = &collidesCollidervHalfvol;

	radialProjections[Collider::SPHERE] = &radialSphere;
	radialProjections[Collider::AABB] = &radialAABB;
	radialProjections[Collider::OBB] = &radialOBB;
	radialProjections[Collider::HALFVOL] = &radialHalfVol;
}

XMFLOAT3 CollisionManager::radialSphere(const Collider & a, const XMFLOAT3 & axis)
{
	XMVECTOR axisV = XMLoadFloat3(&axis);
	XMFLOAT3 rad;
	XMStoreFloat3(&rad, a.GetMaxScale()*axisV);
	return rad;
}

XMFLOAT3 CollisionManager::radialAABB(const Collider & a, const XMFLOAT3 & axis)
{
	XMFLOAT3 L = axis;
	L.x < 0 ? L.x = -1 : L.x = 1;
	L.y < 0 ? L.y = -1 : L.y = 1;
	L.z < 0 ? L.z = -1 : L.z = 1;
	XMVECTOR radVec = XMLoadFloat3(&L);
	XMVECTOR scaleVec = XMLoadFloat3(a.GetScale());
	XMStoreFloat3(&L, scaleVec*radVec);
	return L;
}

XMFLOAT3 CollisionManager::radialOBB(const Collider & a, const XMFLOAT3 & axis)
{
	XMVECTOR axisVec = XMLoadFloat3(&axis);

	//transform axis into obb space
	XMFLOAT4X4 rot = a.GetRotationMatrix();
	XMMATRIX rotMat = XMLoadFloat4x4(&rot);

	rotMat = XMMatrixInverse(&XMMatrixDeterminant(rotMat), rotMat);
	//XMFLOAT3X3 rot3x3;
	//XMStoreFloat3x3(&rot3x3, rotMat);
	axisVec = XMVector3Transform(axisVec, rotMat);

	XMFLOAT3 L;
	XMStoreFloat3(&L, axisVec);
	//find signs of axis
	L.x < 0 ? L.x = -1 : L.x = 1;
	L.y < 0 ? L.y = -1 : L.y = 1;
	L.z < 0 ? L.z = -1 : L.z = 1;
	//multiply by signs
	XMVECTOR radVec = XMLoadFloat3(&L);
	XMVECTOR scaleVec = XMLoadFloat3(a.GetScale());
	radVec *= scaleVec;
	//XMFLOAT3 radial = L*a.GetScale();
	//undo obb rotation back in to world space
	//radial = (XMFLOAT3X3)rotMat*radial;
	radVec = DirectX::XMVector3Transform(radVec, rotMat);
	XMStoreFloat3(&L, radVec);
	return L;
}

XMFLOAT3 CollisionManager::radialHalfVol(const Collider & a, const XMFLOAT3 & axis)
{
	return XMFLOAT3(0, 0, 0);
}

bool CollisionManager::testAxis(const XMFLOAT3 & aCenter, const XMFLOAT3 & aRad, const XMFLOAT3 & bCenter, const XMFLOAT3 & bRad, const XMFLOAT3 & axis)
{
	//vec3 L = glm::normalize(axis);

	XMVECTOR axisVec = XMLoadFloat3(&axis);

	XMVECTOR ac = XMLoadFloat3(&aCenter);
	XMVECTOR ar = XMLoadFloat3(&aRad);
	XMVECTOR bc = XMLoadFloat3(&bCenter);
	XMVECTOR br = XMLoadFloat3(&bRad);

	float greater, less, plus;
	XMStoreFloat(&greater, DirectX::XMVectorAbs(DirectX::XMVector3Dot(axisVec, (ac - bc))));
	XMStoreFloat(&less, DirectX::XMVectorAbs(DirectX::XMVector3Dot(axisVec, ar)));
	XMStoreFloat(&plus, DirectX::XMVectorAbs(DirectX::XMVector3Dot(axisVec, br)));

	if (greater > less + plus) {
		return true;
	}
	return false;
}

bool CollisionManager::testAxis(const Collider & a, const Collider & b, XMFLOAT3 axis)
{
	XMVECTOR axisVec = XMLoadFloat3(&axis);

	axisVec = DirectX::XMVector3Normalize(axisVec);
	XMStoreFloat3(&axis, axisVec);

	XMFLOAT3 aRad = (this->*radialProjections[a.GetType()])(a, axis);
	XMFLOAT3 bRad = (this->*radialProjections[b.GetType()])(b, axis);

	return testAxis(a.GetPosition(), aRad, b.GetPosition(), bRad, axis);
}

XMFLOAT3 CollisionManager::nearPtOBB(const Collider & obb, XMFLOAT3 axisToC)
{
	//transform into OBB space
	XMVECTOR axisToCVec = XMLoadFloat3(&axisToC);

	XMMATRIX rotMat = XMLoadFloat4x4(&obb.GetRotationMatrix());
	axisToCVec = DirectX::XMVector3Transform(axisToCVec, DirectX::XMMatrixInverse(&XMMatrixDeterminant(rotMat), rotMat));

	//clamp to halfwidths
	XMVECTOR scaleVec = XMLoadFloat3(obb.GetScale());
	axisToCVec = XMVector3ClampLengthV(axisToCVec, -scaleVec, scaleVec);

	//back to world coordinates
	axisToCVec = DirectX::XMVector3Transform(axisToCVec, rotMat);

	//add center location
	XMVECTOR pos = XMLoadFloat3(&obb.GetPosition());
	axisToCVec += pos;

	XMStoreFloat3(&axisToC, axisToCVec);
	return axisToC;
}

XMFLOAT3 CollisionManager::nearPtAABB(const Collider & aabb, XMFLOAT3 axis)
{
	XMVECTOR axisVec = XMLoadFloat3(&axis);

	//clamp to halfwidths
	XMVECTOR scale = XMLoadFloat3(aabb.GetScale());
	axisVec = XMVector3ClampLengthV(axisVec, -scale, scale);

	//add center loc
	XMVECTOR pos = XMLoadFloat3(&aabb.GetPosition());
	axisVec += pos;
	XMStoreFloat3(&axis, axisVec);
	return axis;
}

XMFLOAT3 CollisionManager::nearPtPlane(const Collider & plane, const Collider & other)
{
	//XMFLOAT3 normal = ((XMFLOAT3X3)plane.transform.getRotMat())[2];
	XMFLOAT4X4 rot = plane.GetRotationMatrix();
	XMFLOAT3 normal = XMFLOAT3(rot._31, rot._32, rot._33);

	XMVECTOR nor = XMLoadFloat3(&normal);
	XMVECTOR planePos = XMLoadFloat3(&plane.GetPosition());
	XMVECTOR otherPos = XMLoadFloat3(&other.GetPosition());

	nor = otherPos - DirectX::XMVector3Dot(nor, (otherPos - planePos))*nor;

	XMFLOAT3 p;
	XMStoreFloat3(&p, nor);
	return p;
}

bool CollisionManager::collidesAABBvAABB(const Collider & a, const Collider & b)
{
	XMFLOAT3 axis = XMFLOAT3(0, 0, 1);//z
	if (testAxis(a, b, axis)) return false;
	axis = XMFLOAT3(0, 1, 0);//y
	if (testAxis(a, b, axis)) return false;
	axis = XMFLOAT3(1, 0, 0);//x
	if (testAxis(a, b, axis)) return false;

	return true;
}

bool CollisionManager::collidesSpherevSphere(const Collider & a, const Collider & b)
{
	XMVECTOR aPos = XMLoadFloat3(&a.GetPosition());
	XMVECTOR bPos = XMLoadFloat3(&b.GetPosition());
	XMFLOAT3 axis;
	XMStoreFloat3(&axis, aPos - bPos);
	if (testAxis(a, b, axis)) return false;

	return true;
}

bool CollisionManager::collidesAABBvSphere(const Collider & a, const Collider & b)
{
	//find nearest point on box
	XMVECTOR aPos = XMLoadFloat3(&a.GetPosition());
	XMVECTOR bPos = XMLoadFloat3(&b.GetPosition());
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, bPos - aPos);
	XMFLOAT3 aNearest = nearPtAABB(a, pos);
	XMVECTOR aNear = XMLoadFloat3(&aNearest);

	//check distance from nearest point to center of sphere
	XMFLOAT3 axis;
	XMStoreFloat3(&axis, XMVector3Normalize(aNear - bPos));
	XMFLOAT3 bRad = (this->*radialProjections[b.GetType()])(b, axis);

	if (testAxis(aNearest, XMFLOAT3(0, 0, 0), b.GetPosition(), bRad, axis)) return false;

	return true;
}

bool CollisionManager::collidesSpherevAABB(const Collider & a, const Collider & b)
{
	return collidesAABBvSphere(b, a);
}

bool CollisionManager::collidesOBBvOBB(const Collider & a, const Collider & b) {
	XMFLOAT3 axis;
	XMVECTOR axisVec;

	for (int i = 0; i < 3; i++) {

		axisVec = XMLoadFloat4(&a.GetRotationColumn(i));
		XMStoreFloat3(&axis, axisVec);
		if (testAxis(a, b, axis)) return false;

		axisVec = XMLoadFloat4(&b.GetRotationColumn(i));
		XMStoreFloat3(&axis, axisVec);
		if (testAxis(a, b, axis)) return false;

		for (int j = 0; j < 3; j++) {
			//cross product axes
			DirectX::XMVector3Cross(XMLoadFloat4(&a.GetRotationColumn(i)), XMLoadFloat4(&b.GetRotationColumn(j)));
			XMStoreFloat3(&axis, axisVec);
			//axis = glm::cross(((XMFLOAT3X3)a.transform.getRotMat())[i], ((XMFLOAT3X3)b.transform.getRotMat())[j]);
			if (testAxis(a, b, axis)) return false;
		}
	}

	return true;
}

bool CollisionManager::collidesOBBvSphere(const Collider & a, const Collider & b)
{
	//calc nearest point to sphere
	XMVECTOR aPos = XMLoadFloat3(&a.GetPosition());
	XMVECTOR bPos = XMLoadFloat3(&b.GetPosition());
	XMFLOAT3 pos;
	XMStoreFloat3(&pos, bPos - aPos);
	XMFLOAT3 aNearest = nearPtOBB(a, pos);
	XMVECTOR aNear = XMLoadFloat3(&aNearest);

	//test axis from point to sphere center
	XMFLOAT3 axis;
	XMStoreFloat3(&axis, XMVector3Normalize(bPos - aNear));
	XMFLOAT3 bRad = (this->*radialProjections[b.GetType()])(b, axis);

	if (testAxis(aNearest, XMFLOAT3(0, 0, 0), b.GetPosition(), bRad, axis)) return false;

	return true;
}

bool CollisionManager::collidesSpherevOBB(const Collider & a, const Collider & b)
{
	return collidesOBBvSphere(b, a);
}

bool CollisionManager::collidesOBBvAABB(const Collider & a, const Collider & b)
{
	return collidesOBBvOBB(a, b);
}

bool CollisionManager::collidesAABBvOBB(const Collider & a, const Collider & b)
{
	return collidesOBBvAABB(b, a);
}

bool CollisionManager::collidesHalfvolvCollider(const Collider & a, const Collider & b)
{
	XMFLOAT3 axis;
	XMVECTOR axisVec = XMLoadFloat4(&a.GetRotationColumn(2));
	axisVec = XMVector4Normalize(axisVec);
	XMStoreFloat3(&axis, axisVec);

	XMFLOAT3 aRad = (this->*radialProjections[a.GetType()])(a, axis);
	XMFLOAT3 bRad = (this->*radialProjections[b.GetType()])(b, axis);
	XMFLOAT3 aNearest = nearPtPlane(a, b);//nearest point on plane

										  //test collision
	if (testAxis(aNearest, aRad, b.GetPosition(), bRad, axis)) {
		//test half
		XMVECTOR aPos = XMLoadFloat3(&a.GetPosition());
		XMVECTOR bPos = XMLoadFloat3(&b.GetPosition());
		float dot;
		XMStoreFloat(&dot, XMVector3Dot(bPos - aPos, axisVec));
		if (dot > 0) return true;
		return false;
	}

	return true;
}

bool CollisionManager::collidesCollidervHalfvol(const Collider & a, const Collider & b)
{
	return collidesHalfvolvCollider(b, a);
}

bool CollisionManager::collides(const Collider & a, const Collider & b)
{
	//no collisions when belonging to the same base entity -> unity children colliders do not collide with parent colliders
	if (a.GetBaseEntity() == b.GetBaseEntity()) return false;

	// Use object a and object b's collider type
	//		to get a function pointer from the jump table and call the function
	if (collisionTable[{a.GetType(), b.GetType()}]) {
		return (this->*collisionTable[{a.GetType(), b.GetType()}])(a, b);
	}

	return false;
}

Grid::Grid()
{
}

Grid::Grid(float maxScale, XMFLOAT3 halfWidth)
{
	this->halfWidth = halfWidth;
	XMVECTOR hwVec = XMLoadFloat3(&halfWidth);
	XMFLOAT3 temp;
	XMStoreFloat3(&temp, hwVec / maxScale);
	cols = temp.x;
}

Grid::~Grid()
{
}

std::unordered_map<int, std::list<void(*)>>& Grid::getMapRef()
{
	// TODO: insert return statement here
	return grid;
}

void Grid::clear()
{
	grid.clear();
}

void Grid::insert(XMFLOAT3 colLoc, XMFLOAT3 colHalf, void(*colAddress))
{
	//calc min and max ijk
	XMVECTOR hwVec = XMLoadFloat3(&halfWidth);
	XMVECTOR clVec = XMLoadFloat3(&colLoc);
	XMVECTOR chVec = XMLoadFloat3(&colHalf);
	XMFLOAT3 ijkMin, ijkMax;
	XMStoreFloat3(&ijkMin, (clVec - chVec + hwVec)* XMLoadFloat3(&XMFLOAT3(cols, cols, cols)) / (2.0f * hwVec));
	XMStoreFloat3(&ijkMax, (clVec + chVec + hwVec)*XMLoadFloat3(&XMFLOAT3(cols, cols, cols)) / (2.0f * hwVec));

	//calc hash
	for (int i = ijkMin.x; i <= ijkMax.x; i++) {
		for (int j = ijkMin.y; j <= ijkMax.y; j++) {
			for (int k = ijkMin.z; k <= ijkMax.z; k++) {
				int h = i + cols*j + cols*cols*k;
				//insert
				grid[h].push_front(colAddress);
			}
		}
	}
}
