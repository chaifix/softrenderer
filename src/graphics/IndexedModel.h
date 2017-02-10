#pragma once

#include <vector>
#include "../math/Vector4.h"
#include "../util/ObjModel.h"

class IndexedModel
{
public:
    IndexedModel();
    IndexedModel(const ObjModel& obj);

    void CalcNormals();

    void CalcTangents();

    std::vector<Vector4>& GetPositions();
    std::vector<Vector4>& GetTexCoords();
    std::vector<Vector4>& GetNormals();
    std::vector<Vector4>& GetTangents();
    std::vector<int>& GetIndices();

private:
    std::vector<Vector4> m_positions;
    std::vector<Vector4> m_texCoords;
    std::vector<Vector4> m_normals;
    std::vector<Vector4> m_tangents;
    std::vector<int> m_indices;

};
