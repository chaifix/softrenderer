#include "ObjModel.h"
#include "../math/Vector4.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdlib>

using namespace std;

static void split(const string& str, char c, vector<string>& dst)
{
    // Çå¿Õ
    dst.swap(vector<string>());
    for (int i = 0; i < str.size();)
    {
        int j = i;
        for (; j < str.size() && str[j] != c; j++);
        dst.push_back(str.substr(i, j - i));
        i = j;
        while (i < str.size() && str[++i] == c);
    }
}

float toFloat(const string& str)
{
    //stringstream ss(str);
    //float res; 
    //ss >> res; 
    //return res; 
    return atof(str.c_str());
}

ObjModel::ObjModel(std::string  path)
{
    ifstream f(path);
    string line;
    vector<string> tokens;
    while (!f.eof())
    {
        getline(f, line);
        split(line, ' ', tokens);
        if (tokens.size() == 0 || tokens[0] == "#") continue; // skip comment line 
        // we only care about v, vt, vn, f
        if (tokens[0] == "v")
        {// vertex
            positions.push_back(Vector4(toFloat(tokens[1]), toFloat(tokens[2]), toFloat(tokens[3]), 1));
        }
        else if (tokens[0] == "vt")
        {// UV
            texCoords.push_back(Vector4(toFloat(tokens[1]), toFloat(tokens[2]), 0, 0));
        }
        else if (tokens[0] == "vn")
        {// normal 
            normals.push_back(Vector4(toFloat(tokens[1]), toFloat(tokens[2]), toFloat(tokens[3]), 0));
        }
        else if (tokens[0] == "f")
        {// faces
            for (int i = 0; i < tokens.size() - 3; i++)
            {
                indices.push_back(ParseOBJIndex(tokens[1]));
                indices.push_back(ParseOBJIndex(tokens[2 + i]));
                indices.push_back(ParseOBJIndex(tokens[3 + i]));
            }
        }
    }
}

ObjModel::ObjIndex ObjModel::ParseOBJIndex(const std::string& token)
{
    ObjIndex res; 
    vector<string> values;
    split(token, '/', values);
    // vertex coord
    res.vertexIndex = toFloat(values[0]) - 1;
    // texture coord 
    if (values.size() > 1)
    {
        hasTexCoords = true;
        res.texCoordIndex = toFloat(values[1]) - 1;
        // normal coord
        if (values.size() > 2)
        {
            hasNormals = true;
            res.normalIndex = toFloat(values[2]) - 1;
        }
    }
   
    return res;
}
