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