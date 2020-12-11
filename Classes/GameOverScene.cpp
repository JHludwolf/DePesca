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

#include "GameOverScene.h"

USING_NS_CC;

Scene* GameOver::createScene()
{
    return GameOver::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameOver::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    // Inicializar director y auxiliares para el tamaño
    auto director = Director::getInstance();
    auto tamano = director->getWinSize();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // GAME OVER
    // Fondo de Pantalla
    auto spriteFondo = Sprite::create("Files/Escenas/GameOverWallpaper.png");
    spriteFondo->setScale(tamano.width/(1900), tamano.height/(1300));
    spriteFondo->setAnchorPoint(Vec2::ZERO);
    spriteFondo->setPosition(origin);
    addChild(spriteFondo,0);
    
    
    // RESUMEN DEL JUEGO
    auto pecesPescados = UserDefault::getInstance()->getIntegerForKey("pecesPescados");
    auto vidasRestantes = UserDefault::getInstance()->getIntegerForKey("vidasRestantes");
    int puntuacion = pecesPescados*10 + vidasRestantes*50;
    int monedas = (int) puntuacion / 10;
    
    // PECES PESCADOS
    pecesPescadosLabel = Label::createWithTTF(std::to_string(pecesPescados), "fonts/Marker Felt.ttf", 45);
    pecesPescadosLabel->setPosition(Point(430, 475));
    pecesPescadosLabel->setColor(Color3B::WHITE);
    this->addChild(pecesPescadosLabel, 1);
    
    // PUNTUACION Peces*10 + vidas*50
    puntosLabel = Label::createWithTTF(std::to_string(puntuacion), "fonts/Marker Felt.ttf", 45);
    puntosLabel->setPosition(Point(430, 415));
    puntosLabel->setColor(Color3B::WHITE);
    this->addChild(puntosLabel, 1);
    
    // MONEDAS PUNTUACION / 10
    pecesPescadosLabel = Label::createWithTTF(std::to_string(monedas), "fonts/Marker Felt.ttf", 45);
    pecesPescadosLabel->setPosition(Point(430, 360));
    pecesPescadosLabel->setColor(Color3B::WHITE);
    this->addChild(pecesPescadosLabel, 1);
    
    // BACK BUTTON
    auto backButton = MenuItemFont::create("                            ",CC_CALLBACK_1(GameOver::goBack,this));
    backButton->setScale(1.25);
    backButton->setPosition(750, 260);
    
    auto playButton = MenuItemFont::create("                            ",CC_CALLBACK_1(GameOver::newGame,this));
    playButton->setScale(1.25);
    playButton->setPosition(750, 170);
    
    auto *menu = Menu::create(backButton, playButton, nullptr);
    menu->setPosition(Point(0, 0));
    this->addChild(menu);
    
    return true;
}

void GameOver::goBack(GameOver::Ref* psender){
    Director::getInstance()->resume();
    auto scene = HelloWorld::createScene();
    Director::getInstance()->replaceScene(TransitionFadeBL::create(1, scene));
}

void GameOver::newGame(GameOver::Ref* psender){
    Director::getInstance()->resume();
    if(UserDefault::getInstance()->getBoolForKey("ContraTiempo")){
        auto scene = ContraTiempo::createScene();
        Director::getInstance()->replaceScene(TransitionFadeBL::create(1, scene));
    } else {
        auto scene = JuegoClasico::createScene();
        Director::getInstance()->replaceScene(TransitionFadeBL::create(1, scene));
    }
}



void GameOver::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
