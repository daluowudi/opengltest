//
//  myTest.hpp
//  opengltest
//
//  Created by playcrab on 18/6/4.
//
//

#ifndef myTest_hpp
#define myTest_hpp

#include "cocos2d.h"

USING_NS_CC;

#define TEX_COORD_MAX   1

typedef struct {
    float Position[3];
    float Color[4];
    float TexCoord[2];
} V3_C4_T2;

class myTest : public cocos2d::Layer
{
public:
    myTest();
    ~myTest(){};
    
    static cocos2d::Scene* createScene();
    
    virtual bool init() override;
    
    // implement the "static create()" method manually
    CREATE_FUNC(myTest);
    
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
    void onDraw();
    
    bool onToucheBegan(const Touch* touch, Event* event);
    void onToucheMoved(const Touch* touch, Event* event);
    void onToucheEnded(const Touch* touch, Event* event);
    
private:
    void updateMVMatrix();
    void updatePMatrix();
private:
    CustomCommand _command;
    
    GLuint vao = 0;
    GLuint textureId;
    
    Vec3 eye;
    Vec3 target;
    
    float distance;
    float radianY;
    float radianX;
    
    Mat4 _mvMatrix;
    Mat4 _pMatrix;
    
    bool _update;
};

#endif /* myTest_hpp */
