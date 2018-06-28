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

//using namespace std;
//using namespace cocos2d;

bool loadObj(const char * path,
             std::vector<std::vector<float>> & out_vertices,
             std::vector<std::vector<float>> & out_uvs,
             std::vector<std::vector<float>> & out_normals);

bool loadObj(std::istream &inStream,
             std::vector<std::vector<float>> & out_vertices,
             std::vector<std::vector<float>> & out_uvs,
             std::vector<std::vector<float>> & out_normals);

bool isSpace(const char c);

std::string parseString(const char *&token);
float parseFloat(const char *&token);
void parseFloat3(float &x, float &y, float &z, const char *&token);
void parseFloat2(float &u, float &v, const char *&token);

int parseTriple(const char *&token);
void parseTriple3(int &i1, int &i2, int &i3, const char *&token);
#endif /* objloader_hpp */
