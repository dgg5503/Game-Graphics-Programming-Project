#include "Collider.h"



Collider::Collider() : 
	colType(OBB),
	offset(0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 1.0f),
	rotation(0.0f, 0.0f, 0.0f, 1.0f)
{}

Collider::Collider(ColliderType type) :
	colType(type),
	offset(0.0f, 0.0f, 0.0f),
	scale(1.0f, 1.0f, 1.0f),
	rotation(0.0f, 0.0f, 0.0f, 1.0f)
{}

Collider::Collider(ColliderType type, XMFLOAT3 & offset, XMFLOAT3 & scale, XMFLOAT4 & rotation) :
	colType(type),
	offset(offset),
	scale(scale),
	rotation(rotation)
{}


Collider::Collider(ColliderType type, float offset[3], float scale[3], float rotation[4]) :
	colType(type),
	offset(offset),
	scale(scale),
	rotation(rotation)
{}

Collider::Collider(ColliderType type, Mesh * mesh) :
	colType(type)
{
	// Ensure we're starting out with a mesh
	assert(mesh != nullptr);

	//calculate offset, scale, and rotation using the mesh data
}

Collider::~Collider()
{
}

// --------------------------------------------------------
// Get the current offset as a an array of floats
// --------------------------------------------------------
const XMFLOAT3 * const Collider::GetOffset() const
{
	return &offset;
}

// --------------------------------------------------------
// Get the current scale as a an array of floats
// --------------------------------------------------------
const XMFLOAT3 * const Collider::GetScale() const
{
	return &scale;
}

// --------------------------------------------------------
// Get the maximum scale value
// --------------------------------------------------------
float const Collider::GetMaxScale() const
{
	return max(abs(scale.x), max(abs(scale.y), abs(scale.z)));
}

// --------------------------------------------------------
// Get the current rotation quaternion as a an array of floats
// --------------------------------------------------------
const XMFLOAT4 * const Collider::GetRotation() const
{
	return &rotation;
}

const Collider::ColliderType const Collider::GetType() const
{
	return colType;
}

XMFLOAT4X4 Collider::GetRotationMatrix() const
{
	XMFLOAT4 trans = GetEntityRotation();
	XMFLOAT4 curr = *GetRotation();
	XMVECTOR transQ = XMLoadFloat4(&trans);
	XMVECTOR currQ = XMLoadFloat4(&curr);

	transQ = XMQuaternionMultiply(transQ, currQ);

	//XMStoreFloat4(&trans, transQ);
	XMMATRIX rotMat = XMMatrixRotationQuaternion(transQ);
	XMFLOAT4X4 rot;
	XMStoreFloat4x4(&rot, rotMat);
	return rot;
}

XMFLOAT3 const Collider::GetPosition() const
{
	//TODO: apply rotation of transform to the offset
	XMVECTOR parentLocation = XMLoadFloat3(parentEntity->transform.GetPosition());
	XMVECTOR colliderOffset = XMLoadFloat3(&offset);

	XMFLOAT3 position;
	XMStoreFloat3(&position, parentLocation + colliderOffset);
	return position;
}

//col or row, not sure which yet
XMFLOAT4 const Collider::GetRotationColumn(int col) const
{
	if (col < 0 || col > 3) {
		//out of index
		return XMFLOAT4();
	}

	XMFLOAT4X4 rotMat = this->GetRotationMatrix();
	XMFLOAT4 rot;
	switch (col) {
	case 0: 
		rot = XMFLOAT4(rotMat._11, rotMat._21, rotMat._31, rotMat._41);
		break;
	case 1:
		rot = XMFLOAT4(rotMat._12, rotMat._22, rotMat._32, rotMat._42);
		break;
	case 2:
		rot = XMFLOAT4(rotMat._13, rotMat._23, rotMat._33, rotMat._43);
		break;
	case 3:
		rot = XMFLOAT4(rotMat._14, rotMat._24, rotMat._34, rotMat._44);
		break;
	}
	return rot;
}

XMFLOAT4 Collider::GetEntityRotation() const
{
	XMFLOAT4 rot = *(parentEntity->transform.GetRotation());
	return rot;
}







////////////////////////////////////////
// Will be moved into some sort of collision manager soon
////////////////////////////////////////
typedef bool(*collisionFunction)(const Collider&, const Collider&);
typedef std::pair<Collider::ColliderType, Collider::ColliderType> collisionPair;
typedef XMFLOAT3 radialVector(const Collider& a, const XMFLOAT3& axis);
//		Collider-pair jump table
//std::map<std::pair<Collider::Type, Collider::Type>, bool(Engine::*)(const Object&, const Object&)> jumpTable;
//std::map<collisionPair, collisionFunction> collisionTable;
struct ColliderHasher {
	template <typename C>
	std::size_t operator()(const std::pair<C, C>& pair) const {
		return pair.first * 4 + pair.second;
		//4 collider types
	}
};
std::unordered_map<collisionPair, collisionFunction, ColliderHasher> collisionTable;

//		radial projection jump table
std::unordered_map<Collider::ColliderType, XMFLOAT3(*)(const Collider&, const XMFLOAT3&)> radialProjections;

//		radial projection functions
XMFLOAT3 radialSphere(const Collider& a, const XMFLOAT3& axis) {
	XMVECTOR axisV = XMLoadFloat3(&axis);
	XMFLOAT3 rad;
	XMStoreFloat3(&rad, a.GetMaxScale()*axisV);
	return rad;
}

XMFLOAT3 radialAABB(const Collider& a, const XMFLOAT3& axis) {
	XMFLOAT3 L = axis;
	L.x < 0 ? L.x = -1 : L.x = 1;
	L.y < 0 ? L.y = -1 : L.y = 1;
	L.z < 0 ? L.z = -1 : L.z = 1;
	XMVECTOR radVec = XMLoadFloat3(&L);
	XMVECTOR scaleVec = XMLoadFloat3(a.GetScale());
	XMStoreFloat3(&L, scaleVec*radVec);
	return L;
}

XMFLOAT3 radialOBB(const Collider& a, const XMFLOAT3& axis) {
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

XMFLOAT3 radialHalfVol(const Collider& a, const XMFLOAT3& axis) {
	return XMFLOAT3(0, 0, 0);
}

//		testAxis function
bool testAxis(const XMFLOAT3& aCenter, const XMFLOAT3& aRad, const XMFLOAT3& bCenter, const XMFLOAT3& bRad, const XMFLOAT3& axis) {
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

bool testAxis(const Collider& a, const Collider& b, XMFLOAT3 axis) {
	XMVECTOR axisVec = XMLoadFloat3(&axis);

	axisVec = DirectX::XMVector3Normalize(axisVec);
	XMStoreFloat3(&axis, axisVec);

	XMFLOAT3 aRad = radialProjections[a.GetType()](a, axis);
	XMFLOAT3 bRad = radialProjections[b.GetType()](b, axis);

	return testAxis(a.GetPosition(), aRad, b.GetPosition(), bRad, axis);
}

//		calc nearest point on OBB
XMFLOAT3 nearPtOBB(const Collider & obb, XMFLOAT3 axisToC) {
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

XMFLOAT3 nearPtAABB(const Collider& aabb, XMFLOAT3 axis) {
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

XMFLOAT3 nearPtPlane(const Collider& plane, const Collider& other) {
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

bool Engine::collides(const Collider& a, const Collider& b)
{
	// Use object a and object b's collider type
	//		to get a function pointer from the jump table and call the function
	if (collisionTable[{a.GetType(), b.GetType()}]) {
		return (*collisionTable[{a.GetType(), b.GetType()}])(a, b);
	}

	return false;
}
bool collidesNone(const Collider & a, const Collider & b)
{
	return false;
}

// ICE 1: Replace this innards of this function with a call test axis
bool collidesAABBvAABB(const Collider & a, const Collider & b)
{
	XMFLOAT3 axis = XMFLOAT3(0, 0, 1);//z
	if (testAxis(a, b, axis)) return false;
	axis = XMFLOAT3(0, 1, 0);//y
	if (testAxis(a, b, axis)) return false;
	axis = XMFLOAT3(1, 0, 0);//x
	if (testAxis(a, b, axis)) return false;

	return true;
}

// ICE 1: Replace this innards of this function with a call test axis
bool collidesSpherevSphere(const Collider & a, const Collider & b)
{
	XMVECTOR aPos = XMLoadFloat3(&a.GetPosition());
	XMVECTOR bPos = XMLoadFloat3(&b.GetPosition());
	XMFLOAT3 axis;
	XMStoreFloat3(&axis, aPos - bPos);
	if (testAxis(a, b, axis)) return false;

	return true;
}

// ICE 1 Challenge: Replace this innards of this function with a call test axis
bool collidesAABBvSphere(const Collider & a, const Collider & b)
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
	XMFLOAT3 bRad = radialProjections[b.GetType()](b, axis);

	if (testAxis(aNearest, XMFLOAT3(0, 0, 0), b.GetPosition(), bRad, axis)) return false;

	return true;
}

bool collidesSpherevAABB(const Collider & a, const Collider & b)
{
	return collidesAABBvSphere(b, a);
}

// ICE 2
bool collidesOBBvOBB(const Collider & a, const Collider & b) {
	XMFLOAT3 axis;

	for (int i = 0; i < 3; i++) {

		axis = ((XMFLOAT3X3)a.transform.getRotMat())[i];
		if (testAxis(a, b, axis)) return false;

		axis = ((XMFLOAT3X3)b.transform.getRotMat())[i];
		if (testAxis(a, b, axis)) return false;

		for (int j = 0; j < 3; j++) {
			//cross product axes
			axis = glm::cross(((XMFLOAT3X3)a.transform.getRotMat())[i], ((XMFLOAT3X3)b.transform.getRotMat())[j]);
			if (testAxis(a, b, axis)) return false;
		}
	}

	return true;
}

bool collidesOBBvSphere(const Collider & a, const Collider & b)
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
	XMFLOAT3 bRad = radialProjections[b.GetType()](b, axis);

	if (testAxis(aNearest, XMFLOAT3(0, 0, 0), b.GetPosition(), bRad, axis)) return false;

	return true;
}

bool collidesSpherevOBB(const Collider & a, const Collider & b)
{
	return collidesOBBvSphere(b, a);
}

bool collidesOBBvAABB(const Collider & a, const Collider & b)
{
	return collidesOBBvOBB(a, b);
}

bool collidesAABBvOBB(const Collider & a, const Collider & b)
{
	return collidesOBBvAABB(b, a);
}

bool collidesHalfvolvCollider(const Collider & a, const Collider & b)
{
	XMFLOAT3 axis = ((XMFLOAT3X3)a.transform.getRotMat())[2];//z-axis
	XMVECTOR axisVec = XMLoadFloat3(&axis);
	axisVec = XMVector3Normalize(axisVec);
	XMStoreFloat3(&axis, axisVec);

	XMFLOAT3 aRad = radialProjections[a.GetType()](a, axis);
	XMFLOAT3 bRad = radialProjections[b.GetType()](b, axis);
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

bool collidesCollidervHalfvol(const Collider & a, const Collider & b)
{
	return collidesHalfvolvCollider(b, a);
}
/**/

