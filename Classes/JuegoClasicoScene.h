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

#ifndef __JUEGO_CLASICO_SCENE_H__
#define __JUEGO_CLASICO_SCENE_H__

#include "cocos2d.h"
#include "GameOverScene.h"
#include <AudioEngine.h>
#include <time.h>
#include <chrono>

class JuegoClasico : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(JuegoClasico);
    
    int peces_pescados;
    int vidas_restantes;
    int RANGO_DE_APARICION;
    
    // VECTORES DE PECES
    std::vector<cocos2d::Sprite* > pecesID;
    std::vector<cocos2d::Sprite* > pecesDI;
    
    // VECTORES DE OBSTACULOS
    std::vector<cocos2d::Sprite* > botasID;
    std::vector<cocos2d::Sprite* > botasDI;
    std::vector<cocos2d::Sprite* > barrilesID;
    std::vector<cocos2d::Sprite* > barrilesDI;
    
    // VECTORES DE MEDUSAS
    std::vector<cocos2d::Sprite* > medusasID;
    std::vector<cocos2d::Sprite* > medusasDI;
    
    // VECTORES DE TIBUONES
    std::vector<cocos2d::Sprite* > tiburonesID;
    std::vector<cocos2d::Sprite* > tiburonesDI;
    
    // VECTORES DE CANGREJOS
    std::vector<cocos2d::Sprite* > cangrejosID;
    std::vector<cocos2d::Sprite* > cangrejosDI;
    
    cocos2d::Sprite* _pescador;
    cocos2d::Sprite* _pez;
    cocos2d::Sprite* _bota;
    cocos2d::Sprite* _barril;
    cocos2d::Sprite* _medusa;
    cocos2d::Sprite* _tiburon;
    cocos2d::Sprite* _cangrejo;
    cocos2d::Sprite* _anzuelo;
    cocos2d::DrawNode* _line;
    
    cocos2d::Label* puntuacionLabel;
    cocos2d::Label* vidasLabel;
    
    void jugar(Ref* pSender);
    
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
    virtual void onFishTouch(cocos2d::Event *event, bool derizq);
    virtual void onObstaculoTouch(cocos2d::Event *event, bool barril);
    virtual void onMedusaTouch(cocos2d::Event *event, bool derizq);
    virtual void onSharkTouch(cocos2d::Event *event, bool derizq);
    virtual void onCangrejoTouch(cocos2d::Event *event, bool derizq);
    
    virtual void onMouseMove(cocos2d::Event* event);
    
    void goBack(Ref* pSender);
    void gameOver(Ref* pSender);
    
};

#endif // __JUEGO_CLASICO_H__
