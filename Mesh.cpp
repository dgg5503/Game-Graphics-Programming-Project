#include "Mesh.h"


// --------------------------------------------------------
// Constructor
//
// Creates a mesh by uploading supplied data to the GPU.
//
// vertices - array of vertices to upload
// numVerts - number of vertices in the array
// indices	- array of indices to upload, will be used for indexed rendering
// numIndices - number of indices in the array
// device	- device to upload our data to
// --------------------------------------------------------
Mesh::Mesh(
	Vertex* const vertices,
	const int numVerts,
	unsigned int* const indices,
	const int numIndices,
	ID3D11Device* const device)
{
	MeshParameters params = { vertices, indices, numVerts, numIndices };
	if (!UploadModel(params, device))
		fprintf(stderr, "ERROR: Failed to upload model from ctor!\n");
}


// --------------------------------------------------------
// Constructor
//
// Creates a mesh by loading in an OBJ file and uploading 
// its vertices and other information to the GPU
//
// file		- file path to obj file
// device	- device to upload our data to
// --------------------------------------------------------
Mesh::Mesh(const char * const file, ID3D11Device * const device)
{
	// Load our obj file.
	LoadOBJ(file, device);
}

// --------------------------------------------------------
// Destructor
// --------------------------------------------------------
Mesh::~Mesh()
{
	// Free DX resources
	if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }
}

// --------------------------------------------------------
// Get a constant pointer to the vertex buffer of this mesh
// --------------------------------------------------------
ID3D11Buffer * const Mesh::GetVertexBuffer() const
{
	return vertexBuffer;
}

// --------------------------------------------------------
// Get a constant pointer to the index buffer of this mesh
// --------------------------------------------------------
ID3D11Buffer * const Mesh::GetIndexBuffer() const
{
	return indexBuffer;
}

// --------------------------------------------------------
// Get the number of indices in the index buffer for this mesh
// --------------------------------------------------------
const int Mesh::GetIndexCount() const
{
	return numIndices;
}


// --------------------------------------------------------
// Loads an OBJ file onto the stack then uploads the model
// data to the GPU.
//
// objFile	- file path to obj file
// device	- device to upload our data to
// --------------------------------------------------------
void Mesh::LoadOBJ(const char * const objFile, ID3D11Device* const device)
{
	// Basic precondition checks
	assert(objFile != nullptr);
	assert(device != nullptr);

	// Init params as 0
	Mesh::MeshParameters meshParams = {};

	// File input object
	std::ifstream obj(objFile);

	// Check for successful open
	if (!obj.is_open())
	{
		fprintf(stderr, "ERROR: Model not found %s\n", objFile);
		return;
	}

	// Variables used while reading the file
	std::vector<DirectX::XMFLOAT3> positions;     // Positions from the file
	std::vector<DirectX::XMFLOAT3> normals;       // Normals from the file
	std::vector<DirectX::XMFLOAT2> uvs;           // UVs from the file
	std::vector<Vertex> verts;           // Verts we're assembling
	std::vector<UINT> indices;           // Indices of these verts
	unsigned int vertCounter = 0;        // Count of vertices/indices
	char chars[100];                     // String for line reading

										 // Still have data left?
	while (obj.good())
	{
		// Get the line (100 characters should be more than enough)
		obj.getline(chars, 100);

		// Check the type of line
		if (chars[0] == 'v' && chars[1] == 'n')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			DirectX::XMFLOAT3 norm;
			sscanf_s(
				chars,
				"vn %f %f %f",
				&norm.x, &norm.y, &norm.z);

			// Add to the list of normals
			normals.push_back(norm);
		}
		else if (chars[0] == 'v' && chars[1] == 't')
		{
			// Read the 2 numbers directly into an XMFLOAT2
			DirectX::XMFLOAT2 uv;
			sscanf_s(
				chars,
				"vt %f %f",
				&uv.x, &uv.y);

			// Add to the list of uv's
			uvs.push_back(uv);
		}
		else if (chars[0] == 'v')
		{
			// Read the 3 numbers directly into an XMFLOAT3
			DirectX::XMFLOAT3 pos;
			sscanf_s(
				chars,
				"v %f %f %f",
				&pos.x, &pos.y, &pos.z);

			// Add to the positions
			positions.push_back(pos);
		}
		else if (chars[0] == 'f')
		{
			// Read the face indices into an array
			unsigned int i[12];
			int facesRead = sscanf_s(
				chars,
				"f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
				&i[0], &i[1], &i[2],
				&i[3], &i[4], &i[5],
				&i[6], &i[7], &i[8],
				&i[9], &i[10], &i[11]);

			// - Create the verts by looking up
			//    corresponding data from vectors
			// - OBJ File indices are 1-based, so
			//    they need to be adusted
			Vertex v1;
			v1.Position = positions[i[0] - 1];
			v1.UV = uvs[i[1] - 1];
			v1.Normal = normals[i[2] - 1];

			Vertex v2;
			v2.Position = positions[i[3] - 1];
			v2.UV = uvs[i[4] - 1];
			v2.Normal = normals[i[5] - 1];

			Vertex v3;
			v3.Position = positions[i[6] - 1];
			v3.UV = uvs[i[7] - 1];
			v3.Normal = normals[i[8] - 1];

			// The model is most likely in a right-handed space,
			// especially if it came from Maya.  We want to convert
			// to a left-handed space for DirectX.  This means we 
			// need to:
			//  - Invert the Z position
			//  - Invert the normal's Z
			//  - Flip the winding order
			// We also need to flip the UV coordinate since DirectX
			// defines (0,0) as the top left of the texture, and many
			// 3D modeling packages use the bottom left as (0,0)

			// Flip the UV's since they're probably "upside down"
			v1.UV.y = 1.0f - v1.UV.y;
			v2.UV.y = 1.0f - v2.UV.y;
			v3.UV.y = 1.0f - v3.UV.y;

			// Flip Z (LH vs. RH)
			v1.Position.z *= -1.0f;
			v2.Position.z *= -1.0f;
			v3.Position.z *= -1.0f;

			// Flip normal Z
			v1.Normal.z *= -1.0f;
			v2.Normal.z *= -1.0f;
			v3.Normal.z *= -1.0f;

			// Add the verts to the vector (flipping the winding order)
			verts.push_back(v1);
			verts.push_back(v3);
			verts.push_back(v2);

			// Add three more indices
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;
			indices.push_back(vertCounter); vertCounter += 1;

			// Was there a 4th face?
			if (facesRead == 12)
			{
				// Make the last vertex
				Vertex v4;
				v4.Position = positions[i[9] - 1];
				v4.UV = uvs[i[10] - 1];
				v4.Normal = normals[i[11] - 1];

				// Flip the UV, Z pos and normal
				v4.UV.y = 1.0f - v4.UV.y;
				v4.Position.z *= -1.0f;
				v4.Normal.z *= -1.0f;

				// Add a whole triangle (flipping the winding order)
				verts.push_back(v1);
				verts.push_back(v4);
				verts.push_back(v3);

				// Add three more indices
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
				indices.push_back(vertCounter); vertCounter += 1;
			}
		}
	}

	// Close the file and create the actual buffers
	obj.close();

	// - At this point, "verts" is a vector of Vertex structs, and can be used
	//    directly to create a vertex buffer:  &verts[0] is the address of the first vert
	//
	// - The vector "indices" is similar. It's a vector of unsigned ints and
	//    can be used directly for the index buffer: &indices[0] is the address of the first int
	//
	// - "vertCounter" is BOTH the number of vertices and the number of indices
	// - Yes, the indices are a bit redundant here (one per vertex)

	// set mesh params
	meshParams.vertices = &verts[0];
	meshParams.indices = &indices[0];
	meshParams.numVerts = vertCounter;
	meshParams.numIndices = vertCounter;

	// upload model
	UploadModel(meshParams, device);
}


// --------------------------------------------------------
// Uploads provided model data to the GPU.
//
// params	- Required mesh parameters for uploading data
// device	- device to upload our data to
// --------------------------------------------------------
bool Mesh::UploadModel(const MeshParameters& params, ID3D11Device* const device)
{
	// Basic foolproof checks
	// Ensure minimal params passed
	if (device == nullptr ||
		params.indices == nullptr ||
		params.vertices == nullptr ||
		params.numIndices <= 0 ||
		params.numVerts <= 0)
		return false;
	// if any of the struct 
	//assert(vertices != nullptr);
	//assert(indices != nullptr);
	//assert(device != nullptr);
	//assert(numVerts > 0 && numIndices > 0);

	// Init fields
	this->numIndices = params.numIndices;
	vertexBuffer = nullptr;
	indexBuffer = nullptr;

	// Create the VERTEX BUFFER description -----------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * params.numVerts; // multiply number of verts by size of vertex so GPU knows exact num of bytes
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// Create the proper struct to hold the initial vertex data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = params.vertices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	device->CreateBuffer(&vbd, &initialVertexData, &vertexBuffer);

	// Create the INDEX BUFFER description ------------------------------------
	// - The description is created on the stack because we only need
	//    it to create the buffer.  The description is then useless.
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(int) * numIndices; // multiply num of indices by sizeof int so GPU knows exact num of bytes for array
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;

	// Create the proper struct to hold the initial index data
	// - This is how we put the initial data into the buffer
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = params.indices;

	// Actually create the buffer with the initial data
	// - Once we do this, we'll NEVER CHANGE THE BUFFER AGAIN
	device->CreateBuffer(&ibd, &initialIndexData, &indexBuffer);

	return true;
}
