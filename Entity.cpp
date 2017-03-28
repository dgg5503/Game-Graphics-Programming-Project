#include "Entity.h"

// --------------------------------------------------------
// Constructor
//
// Creates an entity with a mesh.
//
// mesh - The mesh this entity will represent
// --------------------------------------------------------
Entity::Entity(Mesh* mesh, Material* material) :
	mesh(mesh),
	material(material)
{
	// Ensure we're starting out with a mesh
	assert(mesh != nullptr);

	collider = nullptr;
}

// --------------------------------------------------------
// Destructor
// --------------------------------------------------------
Entity::~Entity()
{
	if (collider != nullptr) delete collider;
}

/*
void Entity::PrepareMaterial(SimpleVertexShader* const vertexShader)
{
	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	//vertexShader->SetMatrix4x4("world", transform.GetWorldMatrix());
	//vertexShader->SetMatrix4x4("inverseTransposeWorld",
	//	transform.GetInverseTransposeWorldMatrix());

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	//vertexShader->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	//vertexShader->SetShader();
	//pixelShader->SetShader();
}
*/

// --------------------------------------------------------
// Set the current mesh of this entity
//
// mesh - pointer to mesh
// --------------------------------------------------------
void Entity::SetMesh(Mesh* mesh)
{
	// Ensure new mesh is not null
	assert(mesh != nullptr);
	this->mesh = mesh;
}


// --------------------------------------------------------
// Set the current material of this entity
//
// material - pointer to material
// --------------------------------------------------------
void Entity::SetMaterial(Material* material)
{
	// TODO: Ensure not in process of rendering at this point, i.e. block until next frame
	// Remove self from renderer current bin
	Renderer::Instance()->UnstageEntity(this);

	// Set new material
	this->material = material;

	// Restage to new location
	Renderer::Instance()->StageEntity(this);
}

// --------------------------------------------------------
// Get a constant pointer to this entities current mesh
// --------------------------------------------------------
Mesh * const Entity::GetMesh() const
{
	return mesh;
}

// --------------------------------------------------------
// Get a constant pointer to this entities current material
// --------------------------------------------------------
Material * const Entity::GetMaterial() const
{
	return material;
}

void Entity::CreateCollider(Collider::ColliderType type)
{
	if (collider != nullptr) {
		return;
	}
	collider = new Collider(type);
	collider->SetParentEntity(this);
}

Collider * const Entity::GetCollider() const
{
	return collider;
}

void Entity::OnCollision(Collision collision)
{
}
