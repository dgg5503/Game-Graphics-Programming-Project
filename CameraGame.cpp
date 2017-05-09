#include "CameraGame.h"
#include "MemoryDebug.h"

CameraGame::CameraGame()
{
}


CameraGame::~CameraGame()
{
}

void CameraGame::UpdateProjectionMatrix(float aspectRatio)
{
	// Update our projection matrix for whatever reason
	XMMATRIX P = XMMatrixOrthographicLH(
		aspectRatio * 4.0f,		// View Width
		4.0f,		// View Height
		0.01f,		// Near Plane
		1000.0f		// Far Plane
		);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}
