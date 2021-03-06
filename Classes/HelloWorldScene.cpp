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

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = cocos2d::Scene::create();
    
    auto layer = HelloWorld::create();
    
    scene->addChild(layer);
    
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto program = new GLProgram;
    program->autorelease();
    
    program->initWithFilenames("shaders/myVertexShader.vsh", "shaders/myFragmentShader.fsh");
    program->link();
    //set uniform locations
    program->updateUniforms();
    setGLProgram(program);
    
    //创建和绑定vao
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    //创建和绑定vbo
    GLuint vertexVBO;
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);

    Vertex data[] =
    {
        // Front
        { {1, -1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
        { {1, 1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
        { {-1, 1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
        { {-1, -1, 0}, {0, 0, 0, 1}, {0, 0}},
        // Back
        { {1, 1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
        { {-1, -1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
        { {1, -1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
        { {-1, 1, -2}, {0, 0, 0, 1}, {0, 0}},
        // Left
        { {-1, -1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
        { {-1, 1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
        { {-1, 1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
        { {-1, -1, -2}, {0, 0, 0, 1}, {0, 0}},
        // Right
        { {1, -1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
        { {1, 1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
        { {1, 1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
        { {1, -1, 0}, {0, 0, 0, 1}, {0, 0}},
        // Top
        { {1, 1, 0}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
        { {1, 1, -2}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
        { {-1, 1, -2}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
        { {-1, 1, 0}, {0, 0, 0, 1}, {0, 0}},
        // Bottom
        { {1, -1, -2}, {1, 0, 0, 1}, {TEX_COORD_MAX, 0}},
        { {1, -1, 0}, {0, 1, 0, 1}, {TEX_COORD_MAX, TEX_COORD_MAX}},
        { {-1, -1, 0}, {0, 0, 1, 1}, {0, TEX_COORD_MAX}},
        { {-1, -1, -2}, {0, 0, 0, 1}, {0, 0}}
    };

    
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Position));
    
    Sprite *sprite = Sprite::create("HelloWorld.png");
    textureId = sprite->getTexture()->getName();
//    glGenTextures(1, &textureId);
////    glBindTexture(GL_TEXTURE_2D, textureId);
//    GL::bindTexture2D(textureId);
//    
//    Image *image = new Image;
//    image->initWithImageFile("HelloWorld.png");
//    unsigned char *imageData = image->getData();
//    int width = image->getWidth();
//    int height = image->getHeight();
//    
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
//    CC_SAFE_FREE(image);
//    
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
////    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    
    GLuint tex = glGetAttribLocation(program->getProgram(), "a_texCoord");
    glEnableVertexAttribArray(tex);
    glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoord));
    
    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Color));
    
    GLubyte indices[] = {
        // Front
        0, 1, 2,
        2, 3, 0,
        // Back
        4, 5, 6,
        4, 5, 7,
        // Left
        8, 9, 10,
        10, 11, 8,
        // Right
        12, 13, 14,
        14, 15, 12,
        // Top
        16, 17, 18,
        18, 19, 16,
        // Bottom
        20, 21, 22,
        22, 23, 20
    };
    GLuint indexVBO;
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    //for safty
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
//    glEnable(GL_DEPTH_TEST);
//    glDepthFunc(GL_LESS);
    
//    auto listenerkeyPad = EventListenerKeyboard::create();
//    listenerkeyPad->onKeyPressed = CC_CALLBACK_2(HelloWorld::calPos, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    
    return true;
}

void HelloWorld::onDraw()
{
//    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
    
    auto director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    director->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    Mat4 projectionMatrix ;//= getProjectionMatrix();
    Mat4::createPerspective(60, 480/320, 0.1, 100, &projectionMatrix);
    director->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, projectionMatrix);
    
    Mat4 modelViewMatrix ; //= getModelViewMatrix();
    Mat4::createLookAt(Vec3(0,0,2), Vec3(0,0,0), Vec3(0,1,0), &modelViewMatrix);

    
    director->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, modelViewMatrix);
    
    auto program = getGLProgram();
    program->use();
    program->setUniformsForBuiltins();
    
    //use vao，因为vao记录了每一个顶点属性和缓冲区的状态，所以只需要绑定就可以使用了
    glBindVertexArray(vao);
    
    GL::bindTexture2D(textureId);
//    glBindTexture(GL_TEXTURE_2D, textureId);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (GLvoid*)0);
    
    glBindVertexArray(0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 6);
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    
    //    program->setUniformsForBuiltins();
}

void HelloWorld::calPos(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
    _direction = Vec3(
        cosf(CC_DEGREES_TO_RADIANS(verticalAngle)) * sinf(CC_DEGREES_TO_RADIANS(horizontalAngle)),
        sinf(CC_DEGREES_TO_RADIANS(verticalAngle)),
        cosf(CC_DEGREES_TO_RADIANS(verticalAngle)) * cosf(CC_DEGREES_TO_RADIANS(horizontalAngle))
                   
    );
    Vec3 right(
        sinf(CC_DEGREES_TO_RADIANS(horizontalAngle) - M_PI/2),
        0,
        cosf(CC_DEGREES_TO_RADIANS(horizontalAngle) - M_PI/2)
    );
    
    Vec3::cross(_direction, right, &_up);
    
    if (keycode == EventKeyboard::KeyCode::KEY_W)
    {
        _eyePos += _direction * speed;
    }
    
    if (keycode == EventKeyboard::KeyCode::KEY_S)
    {
        _eyePos -= _direction * speed;
    }
    
    if (keycode == EventKeyboard::KeyCode::KEY_A)
    {
        _eyePos -= right * speed;
    }
    
    if (keycode == EventKeyboard::KeyCode::KEY_D)
    {
        _eyePos += right * speed;
    }
}

Mat4 HelloWorld::getProjectionMatrix()
{
    Mat4 promat;
    Mat4::createPerspective(initialFov, 960/640, 0.01, 100, &promat);
    
    return promat;
}

Mat4 HelloWorld::getModelViewMatrix()
{
    Mat4 mvmat;
    Mat4::createLookAt(_eyePos, _eyePos + _direction, _up, &mvmat);
    
    return mvmat;
}

// on "init" you need to initialize your instance
//bool HelloWorld::init()
//{
//    //////////////////////////////
//    // 1. super init first
//    if ( !Layer::init() )
//    {
//        return false;
//    }
//    
////    this->setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_COLOR));
//    
//    auto program = new GLProgram;
//    program->autorelease();
//    
//    program->initWithFilenames("shaders/myVertexShader.vsh", "shaders/myFragmentShader.fsh");
//    program->link();
//    //set uniform locations
//    program->updateUniforms();
//    setGLProgram(program);
//    
//    //创建和绑定vao
//    glGenVertexArrays(1, &vao);
//    glBindVertexArray(vao);
//    
//    //创建和绑定vbo
//    GLuint vertexVBO;
//    glGenBuffers(1, &vertexVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
//    
////    auto size = Director::getInstance()->getVisibleSize();
////    float vertercies[] = { 0,0,   //第一个点的坐标
////        size.width, 0,   //第二个点的坐标
////        size.width / 2, size.height};  //第三个点的坐标
//    
////    float vertercies[] = { -1,-1,   //第一个点的坐标
////        1, -1,   //第二个点的坐标
////        0, 1};  //第三个点的坐标
// 
////    float vertercies[] = {
////        -1,-1,
////        1,-1,
////        -1,1,
////        -1,1,
////        1,1,
////        1,-1
////    };
//    float vertercies[] = {
//        -0.5,-0.5,
//        0.5,-0.5,
//        -0.5,0.5,
//        0.5,0.5,
//    };
////    float color[] = {
////        0, 1, 0, 1,
////        1, 0, 0, 1,
////        0, 0, 1, 1,
////        0, 1, 0, 1,
////        1, 0, 0, 1,
////        0, 0, 1, 1
////    };
//    
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertercies), vertercies, GL_STATIC_DRAW);
//    //获取vertex attribute "a_position"的入口点
//    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
//    //打开入a_position入口点
//    glEnableVertexAttribArray(positionLocation);
//    //传递顶点数据给a_position，注意最后一个参数是数组的偏移了。
//    glVertexAttribPointer(positionLocation, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
//    
//    GLubyte indices[] = {
//        0, 1, 2,
//        2, 1, 3,
//    };
//    GLuint indexVBO;
//    glGenBuffers(1, &indexVBO);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//    
//    //set for color
//    float color[] = {
//        0, 1, 0, 1,
//        0, 1, 0, 1,
//        0, 1, 0, 1,
//        0, 1, 0, 1,
//        0, 1, 0, 1,
//        0, 1, 0, 1
//    };
//    GLuint colorVBO;
//    glGenBuffers(1, &colorVBO);
//    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(color), color, GL_STATIC_DRAW);
//    
//    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
//    glEnableVertexAttribArray(colorLocation);
//    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
//    
////    glDrawArrays(GL_TRIANGLES, 0, 3);
//    
//    //for safty
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    
//    
////    GLuint uColorLocation = glGetUniformLocation(program->getProgram(), "u_color");
////    float uColor[] = {1.0, 1.0, 1.0, 1.};
////    glUniform4fv(uColorLocation, 1, uColor);
//    
//    return true;
//}

void HelloWorld::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
    Layer::visit(renderer,parentTransform,parentFlags);
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(HelloWorld::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}
/*****
void HelloWorld::onDraw()
{
//    //获得当前HelloWorld的shader
//    auto glProgram = getGLProgram();
//    //使用此shader
//    glProgram->use();
//    //设置该shader的一些内置uniform,主要是MVP，即model-view-project矩阵
//    glProgram->setUniformsForBuiltins();
    
    auto program = new GLProgram;
    program->initWithFilenames("shaders/myVertexShader.vsh", "shaders/myFragmentShader.fsh");
    program->link();
    //set uniform locations
    program->updateUniforms();
    
    program->setUniformsForBuiltins();
    
    auto size = Director::getInstance()->getWinSize();
    float vertercies[] = { 0,0,   //第一个点的坐标
        size.width, 0,   //第二个点的坐标
        size.width / 2, size.height};  //第三个点的坐标
    
    float color[] = { 0, 1,0, 1,    //第一个点的颜色，绿色
        1,0,0, 1,  //第二个点的颜色, 红色
        0, 0, 1, 1};  //第三个点的颜色， 蓝色
    
    //激活名字为position和color的vertex attribute
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION | GL::VERTEX_ATTRIB_FLAG_COLOR);
    //分别给position和color指定数据源
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, 0, vertercies);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_FLOAT, GL_FALSE, 0, color);
    //绘制三角形，所谓的draw call就是指这个函数调用
    glDrawArrays(GL_TRIANGLES, 0, 3);
    //通知cocos2d-x 的renderer，让它在合适的时候调用这些OpenGL命令
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
    //如果出错了，可以使用这个函数来获取出错信息
    CHECK_GL_ERROR_DEBUG();
}
*/

void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
