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

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
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
    
    // CANCION PRINCIPAL EN LOOP
    AudioEngine::stopAll();
    auto audio = AudioEngine::play2d("Files/Theme.mp3", true);
    
    // MENU PRINCIPAL
    // Fondo de Pantalla
    auto spriteFondo = Sprite::create("Files/Escenas/MenuPrincipal.png");
    spriteFondo->setScale(tamano.width/(1900), tamano.height/(1300));
    spriteFondo->setAnchorPoint(Vec2::ZERO);
    spriteFondo->setPosition(origin);
    addChild(spriteFondo,0);
    
    // JUEGO CLASICO BUTTON
    auto juegoClasicoButton = MenuItemFont::create("         ",CC_CALLBACK_1(HelloWorld::jugarJuegoClasico,this));
    juegoClasicoButton->setScale(1.75);
    juegoClasicoButton->setPosition(Point(visibleSize.width / 2, 315));
    
    
    // CONTRA TIEMPO BUTTON
    auto contraTiempoButton = MenuItemFont::create("         ",CC_CALLBACK_1(HelloWorld::jugarContraTiempo,this));
    contraTiempoButton->setScale(1.75);
    contraTiempoButton->setPosition(Point(visibleSize.width / 2, 215));
    
    // INSTRUCCIONES BUTTON
    auto instruccionesButton = MenuItemFont::create("         ",CC_CALLBACK_1(HelloWorld::verInstrucciones,this));
    instruccionesButton->setScale(1.75);
    instruccionesButton->setPosition(Point(visibleSize.width / 2, 115));
    
    auto *menu = Menu::create(juegoClasicoButton, contraTiempoButton, instruccionesButton, nullptr);
    menu->setPosition(Point(0, 0));
    this->addChild(menu);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::jugarJuegoClasico(HelloWorld::Ref* pSender){
    Director::getInstance()->resume();
    auto scene = JuegoClasico::createScene();
    Director::getInstance()->replaceScene(TransitionFadeDown::create(1, scene));

}

void HelloWorld::jugarContraTiempo(HelloWorld::Ref* pSender){
    Director::getInstance()->resume();
    auto scene = ContraTiempo::createScene();
    Director::getInstance()->replaceScene(TransitionFadeDown::create(1, scene));
}

void HelloWorld::verInstrucciones(HelloWorld::Ref* pSender){
    Director::getInstance()->resume();
    auto scene = Instrucciones::createScene();
    Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
}

bool HelloWorld::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    log("onTouchBegan x=%f, y=%f", touch->getLocation().x, touch->getLocation().y);
    return true;
}


void HelloWorld::onMouseMove(cocos2d::Event* event){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    EventMouse* e = (EventMouse*)event;
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(e);
    
    const float x = mouseEvent->getLocation().x;
    const float y = mouseEvent->getLocation().y;
    //log("x=%f, y=%f", x, y);
    _line->setPositionY(500-y);
}
