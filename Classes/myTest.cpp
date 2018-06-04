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
    
    GLuint vertexVBO;
    glGenBuffers(1, &vertexVBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertexVBO);
    
    V3_C4_T2 verticies[] = {
        { {-1, -1, 0}, {1, 0, 0, 1}, {0, 1}},
        { {1, -1, 0}, {1, 0, 0, 1}, {1, 1}},
        { {0, 1, 0}, {1, 0, 0, 1}, {0.5, 0}},
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
    
    GLuint positionLocation = glGetAttribLocation(program->getProgram(), "a_position");
    glEnableVertexAttribArray(positionLocation);
    glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, sizeof(V3_C4_T2), (GLvoid*)offsetof(V3_C4_T2, Position));
    
    GLuint colorLocation = glGetAttribLocation(program->getProgram(), "a_color");
    glEnableVertexAttribArray(colorLocation);
    glVertexAttribPointer(colorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(V3_C4_T2), (GLvoid*)offsetof(V3_C4_T2, Color));
    
    Sprite *sprite = Sprite::create("HelloWorld.png");
    textureId = sprite->getTexture()->getName();
    
    GLuint tex = glGetAttribLocation(program->getProgram(), "a_texCoord");
    glEnableVertexAttribArray(tex);
    glVertexAttribPointer(tex, 2, GL_FLOAT, GL_FALSE, sizeof(V3_C4_T2), (GLvoid*)offsetof(V3_C4_T2, TexCoord));
    
    GLubyte indices[] = {
      0, 1, 2
    };
    
    GLuint indexVBO;
    glGenBuffers(1, &indexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    return true;
}

void myTest::onDraw()
{
    auto director = Director::getInstance();
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    director->loadIdentityMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    auto program = getGLProgram();
    program->use();
    program->setUniformsForBuiltins();
    
    glBindVertexArray(vao);
    
    GL::bindTexture2D(textureId);
    glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, (GLvoid*)0);
    
    glBindVertexArray(0);
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, 3);
    CHECK_GL_ERROR_DEBUG();
    
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void myTest::visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags)
{
    Layer::visit(renderer, parentTransform, parentFlags);
    _command.init(_globalZOrder);
    _command.func = CC_CALLBACK_0(myTest::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_command);
}