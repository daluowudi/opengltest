//
//  objloader.cpp
//  opengltest
//
//  Created by playcrab on 18/6/26.
//
//

#include "objloader.hpp"

bool isSpace(const char c){ return (c == ' ') || (c == '\t'); }

std::string parseString(const char *&token) {
    std::string s;
    token += strspn(token, " \t");
    size_t e = strcspn(token, " \t\r");
    s = std::string(token, &token[e]);
    token += e;
    return s;
}

int parseTriple(const char *&token) {
    auto l1 = strspn(token, " \t/");
    token += l1;
    
    int i = (int)atoi(token);
    auto l2 = strcspn(token, " /");
    token += l2;
    
    return i;
}

void parseTriple3(int &i1, int &i2, int &i3, const char *&token) {
    i1 = parseTriple(token);
    i2 = parseTriple(token);
    i3 = parseTriple(token);
}

float parseFloat(const char *&token) {
    token += strspn(token, " \t");

    float f = (float)atof(token);
    token += strcspn(token, " \t\r");

    return f;
}

void parseFloat3(float &x, float &y, float &z,
                 const char *&token) {
    x = parseFloat(token);
    y = parseFloat(token);
    z = parseFloat(token);
}

void parseFloat2(float &u, float &v, const char *&token) {
    u = parseFloat(token);
    v = parseFloat(token);
}

bool loadObj(const char * path,
             std::vector<float> & out_vertices,
             std::vector<float> & out_uvs,
             std::vector<float> & out_normals){
    std::istringstream ifs(cocos2d::FileUtils::getInstance()->getStringFromFile(path));
    
    if (!ifs) {
        CCLOG("Cannot open file [%s]",path);
        return false;
    }
    return loadObj(ifs, out_vertices, out_uvs, out_normals);
}

bool loadObj(std::istream &inStream,
             std::vector<float> & out_vertices,
             std::vector<float> & out_uvs,
             std::vector<float> & out_normals){
    
    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<float> normals;
    std::vector<int> v_indices,uv_indices,n_indices;
    
    int maxchars = 8192;
    std::vector<char> buf(maxchars);
    
    while (inStream.peek() != -1) {
        inStream.getline(&buf[0], maxchars);
        
        std::string linebuf(&buf[0]);
        
        const char *token = linebuf.c_str();
        token += strspn(token, " \t");
        
        if (token[0] == '\0')
            continue; // empty line
        
        if (token[0] == '#')
            continue; // comment line
        
        //vertex
        if (token[0] == 'v' && isSpace(token[1])) {
            token += 2;
            float x,y,z;
            parseFloat3(x, y, z, token);
            
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            continue;
        }
        
        // uvs
        if (token[0] == 'v' && token[1] == 't' && isSpace(token[2])) {
            token += 3;
            float u,v;
            parseFloat2(u, v, token);
            // **dds纹理v是反的
            v = 1-v;
            
            uvs.push_back(u);
            uvs.push_back(v);
            continue;
        }
        
        // normals
        if (token[0] == 'v' && token[1] == 'n' && isSpace(token[2])) {
            token += 3;
            float x, y, z;
            parseFloat3(x, y, z, token);
            
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
            continue;
        }
        
        if (token[0] == 'f' && isSpace(token[1])) {
            token += 2;
            int vidx[3],uvidx[3],nidx[3];
            
            parseTriple3(vidx[0], uvidx[0], nidx[0], token);
            parseTriple3(vidx[1], uvidx[1], nidx[1], token);
            parseTriple3(vidx[2], uvidx[2], nidx[2], token);
            
            v_indices.push_back(vidx[0]);
            v_indices.push_back(vidx[1]);
            v_indices.push_back(vidx[2]);
            
            uv_indices.push_back(uvidx[0]);
            uv_indices.push_back(uvidx[1]);
            uv_indices.push_back(uvidx[2]);
            
            n_indices.push_back(nidx[0]);
            n_indices.push_back(nidx[1]);
            n_indices.push_back(nidx[2]);
            
            continue;
        }
    }
    
    for (size_t idx = 0; idx < v_indices.size(); ++idx) {
        size_t vidx = (v_indices[idx] - 1) * 3;
        for (auto it= vertices.begin() + vidx; it!= vertices.begin() + vidx + 3; ++it)
        {
            out_vertices.push_back(*it);
        }
        
        size_t uvidx = (uv_indices[idx] - 1) * 2;
        for (auto it= uvs.begin() + uvidx; it!= uvs.begin() + uvidx + 2; ++it)
        {
            out_uvs.push_back(*it);            
        }

        size_t nidx = (n_indices[idx] - 1) * 3;
        for (auto it= normals.begin() + nidx; it!= normals.begin() + nidx + 3; ++it)
        {
            out_normals.push_back(*it);    
        }
    }
    
    return true;
}

struct vertexPack {
    float vertex[3];
    float normal[3];
    float uv[2];
    bool operator<(const vertexPack &other) const{
        return memcmp((void*)this, (void*)&other, sizeof(vertexPack)) > 0;
    };
};

bool getSimilarIndex(vertexPack &pack, std::map<vertexPack, int> &outIndexSaver, unsigned int &result){
    auto it = outIndexSaver.find(pack);
    
    if (it == outIndexSaver.end()) {
        return false;
    }else{
        result = it->second;
        return true;
    }
}

void indexVBO(std::vector<float> &in_vertices,
              std::vector<float> &in_uvs,
              std::vector<float> &in_normals,
              
              std::vector<unsigned int> &out_indices,
              std::vector<float> &out_vertices,
              std::vector<float> &out_uvs,
              std::vector<float> &out_normals)
{
    std::map<vertexPack, int> outIndexSaver;
    for (int i = 0; i < in_vertices.size(); i+=3) {
        vertexPack vp;
        memset(&vp, 0, sizeof(vertexPack)); // 先清零再复制做比较才是正确的
        memcpy(&(vp.vertex), &in_vertices[i], sizeof(float) * 3);
        memcpy(&(vp.normal), &in_normals[i], sizeof(float) * 3);
        int uvidx = i/3*2;
        memcpy(&(vp.uv), &in_uvs[uvidx], sizeof(float) * 2);
        
        unsigned int index;
        if (getSimilarIndex(vp, outIndexSaver, index)) {
            out_indices.push_back(index);
        }else{
            out_vertices.insert(out_vertices.end(), in_vertices.begin() + i, in_vertices.begin() + i + 3);
            out_uvs.insert(out_uvs.end(), in_uvs.begin() + uvidx, in_uvs.begin() + uvidx + 2);
            out_normals.insert(out_normals.end(), in_normals.begin() + i, in_normals.begin() + i + 3);
            index = out_vertices.size() / 3 - 1;
            out_indices.push_back(index);
            outIndexSaver[vp] = index;
        }
    }
}

void parseTBN(std::vector<float> &vertices,
              std::vector<float> &uvs,
              std::vector<float> &normals,
              std::vector<float> &out_tangent,
              std::vector<float> &out_bitangent)
{
    for (int i = 0; i < vertices.size(); i+=9)
    {
        cocos2d::Vec3 v0(vertices[i],vertices[i+1],vertices[i+2]);
        cocos2d::Vec3 v1(vertices[i+3],vertices[i+4],vertices[i+5]);
        cocos2d::Vec3 v2(vertices[i+6],vertices[i+7],vertices[i+8]);

        int uidx = i/3*2;
        cocos2d::Vec2 u0(uvs[uidx],uvs[uidx+1]);
        cocos2d::Vec2 u1(uvs[uidx+2],uvs[uidx+3]);
        cocos2d::Vec2 u2(uvs[uidx+4],uvs[uidx+5]);

        cocos2d::Vec3 deltaPos1 = v1 - v0;
        cocos2d::Vec3 deltaPos2 = v2 - v0;

        cocos2d::Vec2 deltaUV1 = u1 - u0;
        cocos2d::Vec2 deltaUV2 = u2 - u0;

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y*deltaUV2.x);
        cocos2d::Vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
        cocos2d::Vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

        // 所有顶点存相同的向量
        for (int i = 0; i < 3; ++i)
        {
            out_tangent.push_back(tangent.x);
            out_tangent.push_back(tangent.y);
            out_tangent.push_back(tangent.z);

            out_bitangent.push_back(bitangent.x);
            out_bitangent.push_back(bitangent.y);
            out_bitangent.push_back(bitangent.z);
        }
    }

    // 正交化处理，使法线和切线垂直
    for (int i = 0; i < vertices.size(); i+=3)
    {
        cocos2d::Vec3 t(out_tangent[i],out_tangent[i+1],out_tangent[i+2]);
        cocos2d::Vec3 b(out_bitangent[i],out_bitangent[i+1],out_bitangent[i+2]);
        cocos2d::Vec3 n(normals[i],normals[i+1],normals[i+2]);

        t = t - n * cocos2d::Vec3::dot(n, t);
        t.normalize();
        
        cocos2d::Vec3 temp;
        cocos2d::Vec3::cross(n, t, &temp);
        if (cocos2d::Vec3::dot(temp, b) < 0.0f)
        {
            t = t * -1.0f;
        }

        out_tangent[i] = t.x;
        out_tangent[i+1] = t.y;
        out_tangent[i+2] = t.z;
    }
}

void indexVBO_TBN(std::vector<float> &in_vertices,
                  std::vector<float> &in_uvs,
                  std::vector<float> &in_normals,
                  std::vector<float> &in_tangents,
                  std::vector<float> &in_bitangents,
                  
                  std::vector<unsigned int> &out_indices,
                  std::vector<float> &out_vertices,
                  std::vector<float> &out_uvs,
                  std::vector<float> &out_normals,
                  std::vector<float> &out_tangents,
                  std::vector<float> &out_bitangents)
{
    std::map<vertexPack, int> outIndexSaver;
    cocos2d::Vec3 v1;
    cocos2d::Vec3 v2;
    for (int i = 0; i < in_vertices.size(); i+=3) {
        vertexPack vp;
        memset(&vp, 0, sizeof(vertexPack)); // 先清零再复制做比较才是正确的
        memcpy(&(vp.vertex), &in_vertices[i], sizeof(float) * 3);
        memcpy(&(vp.normal), &in_normals[i], sizeof(float) * 3);
        int uvidx = i/3*2;
        memcpy(&(vp.uv), &in_uvs[uvidx], sizeof(float) * 2);
        
        unsigned int index;
        if (getSimilarIndex(vp, outIndexSaver, index)) {
            out_indices.push_back(index);
            v1.set(out_tangents[index * 3], out_tangents[index * 3 + 1], out_tangents[index * 3 + 2]);
            v2.set(in_tangents[i], in_tangents[i + 1], in_tangents[i + 2]);
            v1+=v2;
            out_tangents[index] = v1.x;
            out_tangents[index + 1] = v1.y;
            out_tangents[index + 2] = v1.z;
            
            v1.set(out_bitangents[index * 3], out_bitangents[index * 3 + 1], out_bitangents[index * 3 + 2]);
            v2.set(in_bitangents[i], in_bitangents[i + 1], in_bitangents[i + 2]);
            v1+=v2;
            out_bitangents[index] = v1.x;
            out_bitangents[index + 1] = v1.y;
            out_bitangents[index + 2] = v1.z;
        }else{
            out_vertices.insert(out_vertices.end(), in_vertices.begin() + i, in_vertices.begin() + i + 3);
            out_uvs.insert(out_uvs.end(), in_uvs.begin() + uvidx, in_uvs.begin() + uvidx + 2);
            out_normals.insert(out_normals.end(), in_normals.begin() + i, in_normals.begin() + i + 3);
            out_tangents.insert(out_tangents.end(), in_tangents.begin() + i, in_tangents.begin() + i + 3);
            out_bitangents.insert(out_bitangents.end(), in_bitangents.begin() + i, in_bitangents.begin() + i + 3);
            index = out_vertices.size() / 3 - 1;
            out_indices.push_back(index);
            outIndexSaver[vp] = index;
        }
    }
}