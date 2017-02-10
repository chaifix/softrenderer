#ifndef __MESH_H
#define __MESH_H
#include <vector>
#include <string>
#include "IndexedModel.h"
#include "../math/Vertex2.h"

class Mesh
{
public:
    Mesh() {}
    Mesh(std::string path)
    {
        IndexedModel model = ObjModel(path);
        for (int i = 0; i < model.GetPositions().size(); i++)
        {
            vertices.push_back(new Vertex(model.GetPositions()[i], 
                                model.GetTexCoords()[i]));
        }
        indices = model.GetIndices();
    }

public:
    std::vector<Vertex*> vertices;
    std::vector<int> indices; 
};

#endif