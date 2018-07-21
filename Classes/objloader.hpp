//
//  objloader.hpp
//  opengltest
//
//  Created by playcrab on 18/6/26.
//
//

#ifndef objloader_hpp
#define objloader_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "cocos2d.h"

//using namespace std;
//using namespace cocos2d;

bool loadObj(const char * path,
             std::vector<float> &out_vertices,
             std::vector<float> &out_uvs,
             std::vector<float> &out_normals);

bool loadObj(std::istream &inStream,
             std::vector<float> &out_vertices,
             std::vector<float> &out_uvs,
             std::vector<float> &out_normals);

bool isSpace(const char c);

std::string parseString(const char *&token);
float parseFloat(const char *&token);
void parseFloat3(float &x, float &y, float &z, const char *&token);
void parseFloat2(float &u, float &v, const char *&token);

int parseTriple(const char *&token);
void parseTriple3(int &i1, int &i2, int &i3, const char *&token);

void indexVBO(std::vector<float> &in_vertices,
              std::vector<float> &in_uvs,
              std::vector<float> &in_normals,
              
              std::vector<unsigned int> &out_indices,
              std::vector<float> &out_vertices,
              std::vector<float> &out_uvs,
              std::vector<float> &out_normals);

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
                  std::vector<float> &out_bitangents);

void parseTBN(std::vector<float> &vertices,
              std::vector<float> &uvs,
              std::vector<float> &normals,
              std::vector<float> &tangent,
              std::vector<float> &bitangent);

#endif /* objloader_hpp */
