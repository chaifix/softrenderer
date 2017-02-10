#include "IndexedModel.h"
#include "../util/ObjModel.h"
#include <map>
#include <iostream>
using namespace std;

IndexedModel::IndexedModel()
{
    //m_positions = std::vector<Vector4>();
    //m_texCoords = std::vector<Vector4>();
    //m_normals = std::vector<Vector4>();
    //m_tangents = std::vector<Vector4>();
    //m_indices = std::vector<int>();
}

IndexedModel::IndexedModel(const ObjModel& obj)
{
    *this;
    IndexedModel normalModel;
    typedef map<ObjModel::ObjIndex, int> objindex_int_map;
    map<ObjModel::ObjIndex, int> resultIndexMap; 
    map<int, int> normalIndexMap;
    map<int, int> indexMap; 

    for (int i = 0; i < obj.indices.size(); i++)
    {
        const ObjModel::ObjIndex& curIndex = obj.indices[i];

        const Vector4& curPosition = obj.positions[curIndex.vertexIndex];
        Vector4 curTexCoord ; 
        Vector4 curNormal ;

        if (obj.hasTexCoords)
            curTexCoord = obj.texCoords[curIndex.texCoordIndex];
        if (obj.hasNormals)
            curNormal = obj.normals[curIndex.normalIndex];
        // 
        auto iModelVertexIndex = resultIndexMap.find(curIndex);
        int modelVertexIndex = 0;
        if (iModelVertexIndex == resultIndexMap.end())
        {
            modelVertexIndex = m_positions.size();
            resultIndexMap.insert(objindex_int_map::value_type(curIndex, modelVertexIndex));

            m_positions.push_back(curPosition);
            m_texCoords.push_back(curTexCoord);
            if (obj.hasNormals)
                m_normals.push_back(curNormal);
        }
        else
            modelVertexIndex = resultIndexMap[curIndex];
        // normal 
        auto iNormalModelIndex = normalIndexMap.find(curIndex.vertexIndex);
        int normalModelIndex = 0;
        if (iNormalModelIndex == normalIndexMap.end())
        {
            normalModelIndex = normalModel.m_positions.size();
            normalIndexMap.insert(map<int, int>::value_type(curIndex.vertexIndex, normalModelIndex));

            normalModel.m_positions.push_back(curPosition);
            normalModel.m_texCoords.push_back(curTexCoord);
            normalModel.m_normals.push_back(curNormal);
            normalModel.m_tangents.push_back(Vector4());
        }
        else
            normalModelIndex = iNormalModelIndex->second;

        m_indices.push_back(modelVertexIndex);
        normalModel.m_indices.push_back(normalModelIndex);
        indexMap.insert(map<int, int>::value_type(modelVertexIndex, normalModelIndex));
    }

    if (!obj.hasNormals)
    {
        normalModel.CalcNormals();

        for (int i = 0; i < m_positions.size(); i++)
            m_normals.push_back(normalModel.m_normals[indexMap[i]]);
    }

    normalModel.CalcTangents();

    for (int i = 0; i < m_positions.size(); i++)
        m_tangents.push_back(normalModel.m_tangents[indexMap[i]]);
}

void IndexedModel::CalcNormals()
{
    for (int i = 0; i < m_indices.size(); i += 3)
    {
        int i0 = m_indices[i];
        int i1 = m_indices[i + 1];
        int i2 = m_indices[i + 2];

        Vector4 v1 = m_positions[i1].Sub(m_positions[i0]);
        Vector4 v2 = m_positions[i2].Sub(m_positions[i0]);

        Vector4 normal = v1.Cross(v2).Normal();

        m_normals[i0] = m_normals[i0].Add(normal);
        m_normals[i1] = m_normals[i1].Add(normal);
        m_normals[i2] = m_normals[i2].Add(normal);
    }

    for (int i = 0; i < m_normals.size(); i++)
    {
        m_normals[i] = m_normals[i].Normal();
    }
}

void IndexedModel::CalcTangents()
{
    for (int i = 0; i < m_indices.size(); i += 3)
    {
        int i0 = m_indices[i];
        int i1 = m_indices[i + 1];
        int i2 = m_indices[i + 2];

        Vector4 edge1 = m_positions[i1].Sub(m_positions[i0]);
        Vector4 edge2 = m_positions[i2].Sub(m_positions[i0]);

        float deltaU1 = m_texCoords[i1].GetX() - m_texCoords[i0].GetX();
        float deltaV1 = m_texCoords[i1].GetY() - m_texCoords[i0].GetY();
        float deltaU2 = m_texCoords[i2].GetX() - m_texCoords[i0].GetX();
        float deltaV2 = m_texCoords[i2].GetY() - m_texCoords[i0].GetY();

        float dividend = (deltaU1*deltaV2 - deltaU2*deltaV1);
        float f = dividend == 0 ? 0.0f : 1.0f / dividend;

        Vector4 *tangent = new Vector4(f * (deltaV2 * edge1.GetX() - deltaV1 * edge2.GetX()), f * (deltaV2 * edge1.GetY() - deltaV1 * edge2.GetY()), f * (deltaV2 * edge1.GetZ() - deltaV1 * edge2.GetZ()), 0);

        m_tangents[i0] = m_tangents[i0].Add(*tangent);
        m_tangents[i1] = m_tangents[i1].Add(*tangent);
        m_tangents[i2] = m_tangents[i2].Add(*tangent);
        delete tangent;
    }

    for (int i = 0; i < m_tangents.size(); i++)
    {
        m_tangents[i] = m_tangents[i].Normal();
    }
}

std::vector<Vector4>& IndexedModel::GetPositions()
{
    return m_positions;
}

std::vector<Vector4>& IndexedModel::GetTexCoords()
{
    return m_texCoords;
}

std::vector<Vector4>& IndexedModel::GetNormals()
{
    return m_normals;
}

std::vector<Vector4>& IndexedModel::GetTangents()
{
    return m_tangents;
}

std::vector<int>& IndexedModel::GetIndices()
{
    return m_indices;
}
