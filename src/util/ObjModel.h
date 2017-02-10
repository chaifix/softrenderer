#ifndef __OBJMODEL_H
#define __OBJMODEL_H

#include <string>
#include <vector>
#include "../math/Vector4.h"

class ObjModel
{
public:
    ObjModel(std::string path);

    class ObjIndex
    {
    public:
        int vertexIndex; 
        int texCoordIndex; 
        int normalIndex; 

        ObjIndex()
            : vertexIndex(0),
            texCoordIndex(0),
            normalIndex(0)
        {
        }

        ObjIndex(const ObjIndex& obj)
            : vertexIndex(obj.vertexIndex), 
            texCoordIndex(obj.texCoordIndex), 
            normalIndex(obj.normalIndex)
        {
        }
        /*
        !(a < b) && !(b < a)
            = >
            a == b
        */
        bool operator < (const ObjIndex & r) const
        {
            //return vertexIndex < r.vertexIndex
            //    && texCoordIndex < r.texCoordIndex
            //    && normalIndex < r.normalIndex;
            return vertexIndex * 10000 + texCoordIndex * 1000 + normalIndex
                < r.vertexIndex * 10000 + r.texCoordIndex * 1000 + r.normalIndex;
        }
    };

    ObjIndex ParseOBJIndex(const std::string& token);
    std::vector<Vector4> positions; // 顶点
    std::vector<Vector4> texCoords; // UV
    std::vector<Vector4> normals; // 法线
    std::vector<ObjIndex> indices; // 索引 
    bool hasTexCoords;  // 
    bool hasNormals; // 
};

#endif