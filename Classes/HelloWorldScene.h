/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#define TEX_COORD_MAX   1

#include "cocos2d.h"

typedef struct {
    float Position[3];
    float Color[4];
    float TexCoord[2];
} Vertex;

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags) override;
    void onDraw();
    
    cocos2d::Mat4 getProjectionMatrix();
    cocos2d::Mat4 getModelViewMatrix();
    void calPos(cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event *event);
private:
    cocos2d::CustomCommand _command;
    
    GLuint vao = 0;
    GLuint textureId;
    
    cocos2d::Vec3 _eyePos = cocos2d::Vec3(0, 0, 10);
    cocos2d::Vec3 _direction = cocos2d::Vec3(0, 0, -1);
    cocos2d::Vec3 _up = cocos2d::Vec3(0, 1, 0);
    
    float horizontalAngle = 90.0;
    float verticalAngle = 0.0;
    float initialFov = 45.0f;
    
    float speed = 1.0f;
};

#endif // __HELLOWORLD_SCENE_H__
