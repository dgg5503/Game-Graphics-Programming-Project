#include "EntityTest.h"

EntityTest::EntityTest(Mesh* mesh, Material* material) : Entity(mesh, material)
{
}

EntityTest::~EntityTest()
{
}

void EntityTest::Update(float deltaTime, float totalTime)
{
	// Per entity update
	/*
	float scale[3] = { (XMScalarSin(totalTime) + 1) / 2, (XMScalarSin(totalTime) + 1) / 2, 0.0f };

	// test an XYZ function
	transform.SetPosition(XMScalarSin(totalTime), XMScalarCos(totalTime), 0.0f);

	// test a memcpy
	transform.SetScale(scale);

	// test rotatoes
	transform.SetRotation(0.0f, 0.0f, 1.0f, (XMScalarSin(totalTime) + 1) * XM_PI);
	*/
}

void EntityTest::OnCollision(Collision collision)
{
	printf("test");
}
