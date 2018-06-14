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
    float Normal[3];
    float TexCoord[2];
} V3_C4_T2_N3;

typedef struct {
    float Position[3];
    float Color[4];
}V3_C4;

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
    
    void initTouch();
    bool onToucheBegan(const Touch* touch, Event* event);
    void onToucheMoved(const Touch* touch, Event* event);
    void onToucheEnded(const Touch* touch, Event* event);
    
    void initMouse();
    void onMouseMove(EventMouse* event);
    
    void initKeyBoard();
    void onKeyPressed(EventKeyboard::KeyCode, Event*);
    void onKeyReleased(EventKeyboard::KeyCode, Event*);
    
    void initCube();
    void initAxis();
    
    void drawCube();
    void drawAxis();
    
    void initLight();
private:
    void updateEye();
    void updateMVMatrix();
    void updatePMatrix();
    void updateMVPMatrix();
private:
    CustomCommand _command;
    
    GLuint cubevao = 0;
    GLuint axisvao = 0;
    GLuint textureId;
    
    Vec3 target;
    Vec3 eye;
    Vec3 direction;
    Vec3 right;
    
    float distance;
    float radinV;
    float radinH;
    
    EventKeyboard::KeyCode pressingKey;
    float horizon;
    float vertical;
    
    Mat4 _mvMatrix;
    Mat4 _pMatrix;
};

#endif /* myTest_hpp */
