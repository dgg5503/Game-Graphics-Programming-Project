#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <AK/SoundEngine/Common/AkTypes.h>

using namespace DirectX;

// Defines
#define IS_DIRTY_WVM	0x000F // is world view matrix dirty
#define IS_DIRTY_RUF	0x00F0 // is RUF dirty
#define IS_DIRTY_IVT	0x0F00 // inverse transpose dirty
#define IS_DIRTY_ALL	0x0FFF // all dirty

// Asserts
// Including some static asserts to be super duper sure XMFLOAT3 can cast to
// an exact array of 3 floats
static_assert(offsetof(XMFLOAT3, y) == sizeof(float)
	&& offsetof(XMFLOAT3, z) == sizeof(float) * 2,
	"XMFLOAT3 layout must be compatible with float[3]");

class Transform
{
public:
	// Empty constructor, will initialize to 0'd position, rotation and
	// 1,1,1 scale.
	Transform();
	
	// Initialize with given vars
	Transform(XMFLOAT3 &position,
		XMFLOAT3 &scale,
		XMFLOAT4 &rotation);

	// Initialize with given vars
	Transform(float position[3],
		float scale[3],
		float rotation[4]);
	~Transform();

	operator const AkTransform&();

	// Move in some direction
	void Move(float dx, float dy, float dz);

	// Move in some direction relative to right, up and forward directions
	void MoveForward(float dright, float dup, float dfwd);

	// Getters and setters for transform information
	void SetPosition(XMFLOAT3 position);
	void SetPosition(float x, float y, float z);
	void SetScale(XMFLOAT3 scale);
	void SetScale(float x, float y, float z);
	void SetRotation(XMFLOAT4 rotation);
	// --------------------------------------------------------
	// Set the rotation of this transform by providing an axis
	// and angle
	//
	// x		- x axis direction
	// y		- y axis direction
	// z		- z axis direction
	// theta	- angle to rotate by
	// --------------------------------------------------------
	void SetRotation(float x, float y, float z, float theta)
	{
		// Perform angle axis since thats easier to work with
		XMFLOAT3 axis(x, y, z);
		XMStoreFloat4(
			&rotation,
			XMQuaternionRotationAxis(
				XMLoadFloat3(&axis), theta));

		// setting dirty since next time we ask for world, we need to recalc
		//isRDirty = true;
		isDirty |= IS_DIRTY_ALL;
	}

	const XMFLOAT3* const GetPosition() const;
	const XMFLOAT3* const GetScale() const;
	const XMFLOAT4* const GetRotation() const;
	const XMFLOAT3* const GetRight();
	const XMFLOAT3* const GetUp();
	const XMFLOAT3* const GetForward();

	// Return whether or not transform is currently dirty
	const unsigned short IsDirty() const;
	
	// return reference or copy???
	const XMFLOAT4X4& GetWorldMatrix();
	const XMFLOAT4X4& GetInverseTransposeWorldMatrix();

private:
	// Perform this call internally so unnecessary calculations are not made
	void CalculateWorldMatrix();
	void CalculateRightUpForward();
	void CalculateInverseTransposeWorldMatrix();

	// Sound transform information
	AkTransform akTransform;

	// Basic positional information
	// in order Right, Up, Forward
	// --- DO NOT MOVE THIS CHUNK ---
	XMFLOAT3 right; // vec3
	XMFLOAT3 up; // vec3
	XMFLOAT3 forward; // vec3
	// --- DO NOT MOVE THIS CHUNK ---

	XMFLOAT3 position; // vec3
	XMFLOAT3 scale; // vec3
	XMFLOAT4 rotation; // quat
	XMFLOAT4X4 world; // world matrix
	XMFLOAT4X4 worldInverseTranspose;

	// whenever an update to isSTDirty or isRDirty occurs, world mat will be recalc'd
	unsigned short isDirty;
};

