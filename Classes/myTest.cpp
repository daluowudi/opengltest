//
//  myTest.cpp
//  opengltest
//
//  Created by playcrab on 18/6/4.
//
//

#include "myTest.hpp"

USING_NS_CC;

myTest::myTest()
: cubevao(0)
, textureId(0)
, distance(5)
, radinV(0)
, radinH(0)
, horizon(M_PI)
, vertical(0)
, pressingKey(EventKeyboard::KeyCode::KEY_NONE)
{
    eye.set(0, 0, 5);
    target.set(0, 0, 0);
}

Scene* myTest::createScene()
{
    auto scene = cocos2d::Scene::create();
    
    auto layer = myTest::create();
    
    scene->addChild(layer);
    
    return scene;
}



bool myTest::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto program = new GLProgram;
    program->initWithFilenames("nvs.vsh", "nfs.fsh");
    program->link();
    program->updateUniforms();
    setGLProgram(program);
    
    program->autorelease();
    
    initCube();
    initAxis();
//    initTouch();
    initMouse();
    initKeyBoard();
    
    return true;
}

void myTest::onDraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(true);
    
//    updateMVMatrix();
//    updatePMatrix();
    updateEye();
    updateMVPMatrix();
    
    auto director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    director->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    director->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, _pMatrix);
    director->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, _mvMatrix);
    
    auto program = getGLProgram();
    program->use();
    program->setUniformsForBuiltins();
    
    drawCube();
    drawAxis();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);
}

void myTest::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
    Layer::visit(renderer, parentTransform, parentFlags);
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(myTest::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}

void myTest::updateMVMatrix()
{
    Vec3 axisV{
        0,
        cosf(radinV),
        sinf(radinV),
    };
    Vec3 axisH{
        cosf(radinH),
        0,
        sinf(radinH),
    };
    Vec3 direction;
    
    Vec3 up;
    Vec3::cross(axisH, axisV, &direction);
    direction.normalize();
    Mat4::createLookAt(direction * distance, target, axisV, &_mvMatrix);
}

void myTest::updatePMatrix()
{
    Mat4::createPerspective(45, GLfloat(480.0/320.0), 0.01, 100, &_pMatrix);
}

void myTest::updateMVPMatrix()
{
    
    Mat4::createPerspective(45, GLfloat(480.0/320.0), 0.01, 100, &_pMatrix);
    
    direction.set(
        cosf(vertical) * sinf(horizon),
        sinf(vertical),
        cosf(vertical) * cosf(horizon)
    );
    
    right.set(
        sinf(horizon - M_PI_2),
        0,
        cosf(horizon - M_PI_2)
    );
    
    Vec3 up;
    Vec3::cross(right, direction, &up);
    
    Mat4::createLookAt(eye, direction + eye, up, &_mvMatrix);
}

void myTest::updateEye()
{
    float speed = 0.05;
    if (pressingKey == EventKeyboard::KeyCode::KEY_W)
    {
        eye += direction * speed;
    }

    if (pressingKey == EventKeyboard::KeyCode::KEY_S)
    {
        eye -= direction * speed;
    }

    if (pressingKey == EventKeyboard::KeyCode::KEY_A)
    {
        eye -= right * speed;
    }

    if (pressingKey == EventKeyboard::KeyCode::KEY_D)
    {
        eye += right * speed;
    }
}

void myTest::initTouch()
{
    // touches
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(myTest::onToucheBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(myTest::onToucheMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(myTest::onToucheEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool myTest::onToucheBegan(const Touch* touch, cocos2d::Event *event)
{
    return true;
}

void myTest::onToucheMoved(const Touch* touch, cocos2d::Event *event)
{
    Size size = Director::getInstance()->getVisibleSize();
    
    // 0-1的坐标系
    float npx= touch->getPreviousLocation().x/size.width;
    float npy = touch->getPreviousLocation().y/size.height;
    
    float nx = touch->getLocation().x/size.width;
    float ny = touch->getLocation().y/size.height;
    
    float speed = 5;
    horizon += -2 * asinf((nx - npx) * speed / 2 / distance);
    vertical += 2 * asinf((ny - npy) * speed / 2 / distance);
}

void myTest::onToucheEnded(const Touch* touch, cocos2d::Event *event)
{

}

void myTest::initMouse()
{
    auto listener = EventListenerMouse::create();
    listener->onMouseMove = CC_CALLBACK_1(myTest::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void myTest::onMouseMove(EventMouse *event)
{
    Size size = Director::getInstance()->getWinSizeInPixels();
    float w_2 = size.width / 2;
    float h_2 = size.height / 2;
    
    Vec2 pos = event->getLocationInView();
//    CCLOG("%f %f",pos.x,pos.y);
    pos.x = clampf(pos.x, 0, size.width);
    pos.y = clampf(pos.y, 0, size.height);
    horizon = M_PI - (pos.x - w_2) / w_2 * M_PI_2;
    vertical = (pos.y - h_2) / h_2 * M_PI_2;
}

void myTest::initKeyBoard()
{
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(myTest::onKeyPressed, this);
    listener->onKeyReleased = CC_CALLBACK_2(myTest::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void myTest::onKeyPressed(EventKeyboard::KeyCode keycode, Event* event)
{
    pressingKey = keycode;
}

void myTest::onKeyReleased(EventKeyboard::KeyCode keycode, Event* event)
{
    pressingKey = EventKeyboard::KeyCode::KEY_NONE;
}

void myTest::initCube()
{
    auto program = getGLProgram();
    
    glGenVertexArrays(1, &cubevao);
    glBindVertexArray(cubevao);
    
    float w = 2;
    float wd2 = w/2;
    
    V3_C4_T2 verticies[] = {
        // 前
        { {-wd2, -wd2, wd2}, {1, 0, 0, 1}, {0, 1}}, 	//0
        { {wd2, -wd2, wd2}, {1, 0, 0, 1}, {1, 1}}, 	//1
        { {wd2, wd2, wd2}, {1, 0, 0, 1}, {1, 0}}, 	//2
        { {-wd2, wd2, wd2}, {1, 0, 0, 1}, {0, 0}}, 	//3
        // 后
        { {-wd2, -wd2, -wd2}, {1, 0, 1, 1}, {0, 1}},	// 4
        { {wd2, -wd2, -wd2}, {1, 0, 1, 1}, {1, 1}},	// 5
        { {wd2, wd2, -wd2}, {1, 0, 1, 1}, {1, 0}},	// 6
        { {-wd2, wd2, -wd2}, {1, 0, 1, 1}, {0, 0}},	// 7
        // 左
        { {-wd2, -wd2, wd2}, {0, 1, 0, 1}, {0, 1}},	// 8
        { {-wd2, -wd2, -wd2}, {0, 1, 0, 1}, {1, 1}},	// 9
        { {-wd2, wd2, -wd2}, {0, 1, 0, 1}, {1, 0}},	// 10
        { {-wd2, wd2, wd2}, {0, 1, 0, 1}, {0, 0}},	// 11
        // 右
        { {wd2, -wd2, wd2}, {1, 1, 0, 1}, {0, 1}},	// 12
        { {wd2, -wd2, -wd2}, {1, 1, 0, 1}, {1, 1}},	// 13
        { {wd2, wd2, -wd2}, {1, 1, 0, 1}, {1, 0}},	// 14
        { {wd2, wd2, wd2}, {1, 1, 0, 1}, {0, 0}},		// 15
        // 上
        { {-wd2, wd2, wd2}, {0, 0, 1, 1}, {0, 1}},	// 16
        { {-wd2, wd2, -wd2}, {0, 0, 1, 1}, {1, 1}},	// 17
        { {wd2, wd2, -wd2}, {0, 0, 1, 1}, {1, 0}},	// 18
        { {wd2, wd2, wd2}, {0, 0, 1, 1}, {0, 0}},		// 19
        // 下
        { {-wd2, -wd2, wd2}, {0, 1, 1, 1}, {0, 1}},	// 20
        { {-wd2, -wd2, -wd2}, {0, 1, 1, 1}, {1, 1}},	// 21
        { {wd2, -wd2, -wd2}, {0, 1, 1, 1}, {1, 0}},	// 22
        { {wd2, -wd2, wd2}, {0, 1, 1, 1}, {0, 0}},	//23
    };
    
    GLuint vertexVBO;
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    
    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(V3_C4_T2), (GLvoid*)offsetof(V3_C4_T2, Position));
    
    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(V3_C4_T2), (GLvoid*)offsetof(V3_C4_T2, Color));
    
    GLuint tex = glGetAttribLocation(program->getProgram(), "a_texCoord");
    glEnableVertexAttribArray(tex);
    glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, sizeof(V3_C4_T2), (GLvoid*)offsetof(V3_C4_T2, TexCoord));
    
    GLubyte indices[] = {
        // 前
        0, 1, 2,
        2, 3, 0,
        // 后
        4, 5, 6,
        6, 7, 4,
        // 左
        8, 9, 10,
        10, 11, 8,
        // 右
        12, 13, 14,
        14, 15, 12,
        // 上
        16, 17, 18,
        18, 19, 16,
        // 下
        20, 21, 22,
        22, 23, 20
    };

    float vertexNl[24 * 3];
    Vec3 temp;
    // 计算每个顶点的法向量,不共用顶点所以非常好算
    for (int i = 0; i < 24; i+=4) {
        Vec3::cross((Vec3(verticies[i + 1].Position) - Vec3(verticies[i].Position)), (Vec3(verticies[i + 2].Position) - Vec3(verticies[i].Position)), &temp);
        temp.normalize();
        for (int j = i; j < i + 4; ++j) {
            vertexNl[j * 3 + 0] = temp.x;
            vertexNl[j * 3 + 1] = temp.y;
            vertexNl[j * 3 + 2] = temp.z;
        }
    }
    
    GLuint normalVBO;
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexNl), vertexNl, GL_STATIC_DRAW);
    
    GLuint vertexNormalLocation = glGetAttribLocation(program->getProgram(), "a_vertexNormal");
    glEnableVertexAttribArray(vertexNormalLocation);
    glVertexAttribPointer(vertexNormalLocation, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
    
    GLuint indexVBO;
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    //    Sprite *sprite = Sprite::create("HelloWorld.png");
    //    textureId = sprite->getTexture()->getName();
    
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    
    Image *image = new Image;
    image->initWithImageFile("HelloWorld.png");
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,  GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    unsigned char *imageData = image->getData();
    int width = image->getWidth();
    int height = image->getHeight();
    
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 width,
                 height,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_BYTE,//must be GL_UNSIGNED_BYTE
                 imageData);
    CC_SAFE_DELETE(image);
}

void myTest::drawCube()
{
    glBindVertexArray(cubevao);
    
    GL::bindTexture2D(textureId);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (GLvoid*)0);
    
    glBindVertexArray(0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 36);
    CHECK_GL_ERROR_DEBUG();
}

void myTest::initAxis()
{
    auto program = getGLProgram();
    
    glGenVertexArrays(1, &axisvao);
    glBindVertexArray(axisvao);
    
    float length = 3;
    float cx = 0,cy = 0,cz = 0;
    V3_C4 verticies[] = {
        //原点
        {{cx, cy, cz}, {1, 1, 1, 1}},
        //x轴
        {{cx + length, cy, cz}, {0,1,0,1}},
        //y轴
        {{cx, cx + length, cz}, {1,0,0,1}},
        //z轴
        {{cx, cy, cx + length}, {0,0,1,1}}
    };
    
    GLuint vertexVBO;
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), &verticies, GL_STATIC_DRAW);
    
    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(V3_C4), (GLvoid*)offsetof(V3_C4, Position));
    
    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(V3_C4), (GLvoid*)offsetof(V3_C4, Color));
    
    GLubyte indices[] = {
        0,1,
        0,2,
        0,3
    };
    GLuint indexVBO;
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), &indices, GL_STATIC_DRAW);
    
    glBindVertexArray(0);
}

void myTest::drawAxis()
{
    glBindVertexArray(axisvao);
    
    glDrawElements(GL_LINES, 6, GL_UNSIGNED_BYTE, (GLvoid*)0);
    
    glBindVertexArray(0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 4);
    CHECK_GL_ERROR_DEBUG();
}