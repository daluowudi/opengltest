//
//  myTest.cpp
//  opengltest
//
//  Created by playcrab on 18/6/4.
//
//

#include "myTest.hpp"

USING_NS_CC;

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
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    float w = 2;
    float wd2 = w/2;
    
    V3_C4_T2 verticies[] = {
        // 前
        { {-wd2, -wd2, 0}, {1, 0, 0, 1}, {0, 1}}, 	//0
        { {wd2, -wd2, 0}, {1, 0, 0, 1}, {1, 1}}, 	//1
        { {wd2, wd2, 0}, {1, 0, 0, 1}, {1, 0}}, 	//2
        { {-wd2, wd2, 0}, {1, 0, 0, 1}, {0, 0}}, 	//3
        // 后
        { {-wd2, -wd2, -w}, {1, 0, 1, 1}, {0, 1}},	// 4
        { {wd2, -wd2, -w}, {1, 0, 1, 1}, {1, 1}},	// 5
        { {wd2, wd2, -w}, {1, 0, 1, 1}, {1, 0}},	// 6
        { {-wd2, wd2, -w}, {1, 0, 1, 1}, {0, 0}},	// 7
        // 左
        { {-wd2, -wd2, 0}, {0, 1, 0, 1}, {0, 1}},	// 8
        { {-wd2, -wd2, -w}, {0, 1, 0, 1}, {1, 1}},	// 9
        { {-wd2, wd2, -w}, {0, 1, 0, 1}, {1, 0}},	// 10
        { {-wd2, wd2, 0}, {0, 1, 0, 1}, {0, 0}},	// 11
        // 右
        { {wd2, -wd2, 0}, {1, 1, 0, 1}, {0, 1}},	// 12
        { {wd2, -wd2, -w}, {1, 1, 0, 1}, {1, 1}},	// 13
        { {wd2, wd2, -w}, {1, 1, 0, 1}, {1, 0}},	// 14
        { {wd2, wd2, 0}, {1, 1, 0, 1}, {0, 0}},		// 15
        // 上
        { {-wd2, wd2, 0}, {0, 0, 1, 1}, {0, 1}},	// 16
        { {-wd2, wd2, -w}, {0, 0, 1, 1}, {1, 1}},	// 17
        { {wd2, wd2, -w}, {0, 0, 1, 1}, {1, 0}},	// 18
        { {wd2, wd2, 0}, {0, 0, 1, 1}, {0, 0}},		// 19
        // 下
        { {-wd2, -wd2, 0}, {0, 1, 1, 1}, {0, 1}},	// 20
        { {-wd2, -wd2, -w}, {0, 1, 1, 1}, {1, 1}},	// 21
        { {wd2, -wd2, -w}, {0, 1, 1, 1}, {1, 0}},	// 22
        { {wd2, -wd2, 0}, {0, 1, 1, 1}, {0, 0}},	//23
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
    
    GLuint indexVBO;
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
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
    
    return true;
}

void myTest::onDraw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(true);
    
    auto director = Director::getInstance();
    
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    director->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    Mat4 projectionMatrix;
    Mat4::createPerspective(45, GLfloat(480.0/320.0), 0.01, 100, &projectionMatrix);
    
    Mat4 viewMatrix;
    Mat4::createLookAt(Vec3(0, 0, 5), Vec3(0,0,0), Vec3(0,1,0), &viewMatrix);
    
    Mat4 modelMatrix;
    modelMatrix.setIdentity();
    modelMatrix.rotate(Vec3(0, 1, 0), CC_DEGREES_TO_RADIANS(30));
    modelMatrix.rotate(Vec3(1, 0, 0), CC_DEGREES_TO_RADIANS(20));
    
    director->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, projectionMatrix);
    director->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, viewMatrix);
    director->multiplyMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW, modelMatrix);
    
    
    auto program = getGLProgram();
    program->use();
    program->setUniformsForBuiltins();
    
    glBindVertexArray(vao);
    
    GL::bindTexture2D(textureId);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, (GLvoid*)0);
    
    glBindVertexArray(0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 36);
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    
    glDepthMask(false);
}

void myTest::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
    Layer::visit(renderer, parentTransform, parentFlags);
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(myTest::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}