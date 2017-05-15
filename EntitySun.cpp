#include "EntitySun.h"




EntitySun::EntitySun(EntityFactory * entityFactory, std::string name, Mesh * mesh, Material * material) :
	Entity(entityFactory, name, mesh, material)
{
	// Create emitter
	emitter = Renderer::Instance()->CreateContinuousParticleEmitter("sun_emitter", 10, 1.0f);
	emitter->SetDirectionRange(XMFLOAT3(-1, -1, -1), XMFLOAT3(1, 1, 1));
	emitter->SetAge(2.0f);
	emitter->SetEndSize(XMFLOAT2(0, 0));
	emitter->SetSize(XMFLOAT2(1.0f, 1.0f));
}

EntitySun::~EntitySun()
{
}
