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

#include "ContraTiempoScene.h"

USING_NS_CC;

Scene* ContraTiempo::createScene()
{
    return ContraTiempo::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool ContraTiempo::init()
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
    
    // Wallpaper
    auto spriteFondo = Sprite::create("Files/Escenas/HardGameWallpaper.png");
    
    spriteFondo->setScale(tamano.width/(1900), tamano.height/(1300));
    
    spriteFondo->setAnchorPoint(Vec2::ZERO);
    spriteFondo->setPosition(origin);
   
    addChild(spriteFondo,0);
    
    // ACTION LISTENERS
    
    
    
    auto canaListener = EventListenerMouse::create();
    canaListener->onMouseMove = CC_CALLBACK_1(ContraTiempo::onMouseMove, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(canaListener, this);
    
    // VARIABLES
    peces_pescados = 0;
    vidas_restantes = 1;
    RANGO_DE_APARICION = 15000; 
    timeRemaining = 62.f;
    this->scheduleUpdate();
    
    // TEXTO PUNTUACION
    puntuacionLabel = Label::createWithTTF(std::to_string(peces_pescados), "fonts/Marker Felt.ttf", 35);
    puntuacionLabel->setPosition(Point(345, 482));
    puntuacionLabel->setColor(Color3B::WHITE);
    this->addChild(puntuacionLabel, 1);
    
    // TEXTO VIDAS
    vidasLabel = Label::createWithTTF(std::to_string(vidas_restantes), "fonts/Marker Felt.ttf", 55);
    vidasLabel->setPosition(Point(110, 625));
    vidasLabel->setColor(Color3B::BLACK);
    this->addChild(vidasLabel, 1);
    
    // TEXTO TIEMPO
    tiempoLabel = Label::createWithTTF(std::to_string(vidas_restantes), "fonts/Marker Felt.ttf", 55);
    tiempoLabel->setPosition(Point(870, 625));
    tiempoLabel->setColor(Color3B::BLACK);
    this->addChild(tiempoLabel, 1);
    
    // Pescador
    _pescador = Sprite::create("Files/Sprites/Pescador/pescando2gold.png");
    _pescador->setScale(0.33, 0.33);
    _pescador->setFlippedX(true);
    _pescador->setAnchorPoint(Vec2::ZERO);
    _pescador->setPosition(430,385);
    
    addChild(_pescador, 1);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(ContraTiempo::onTouchBegan, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _pescador);
    
    
    // Peces izquierda - derecha
    for(int i=0;i<30;i++){
        // Creacion del pez
        auto pezAux = Sprite::create("Files/Sprites/Pez/Pez1.png");
        pezAux->setScale(.5, .5);
        pezAux->setFlippedX(true);
        Point p1 = Point(RandomHelper::random_int(-RANGO_DE_APARICION, -100), RandomHelper::random_int(20, 350));
        
        Size pezSize = pezAux->getContentSize();
        
        pezAux->setPosition(p1);
        addChild(pezAux,1); // Agregar pez a la escena
        
        // Animacion del pez
        Vector<SpriteFrame*> framesPez;
        
        framesPez.pushBack(SpriteFrame::create("Files/Sprites/Pez/Pez1.png", Rect(0, 0, pezSize.width, pezSize.height)));
        framesPez.pushBack(SpriteFrame::create("Files/Sprites/Pez/Pez2.png", Rect(0, 0, pezSize.width, pezSize.height)));
        auto animationPez = Animation::createWithSpriteFrames(framesPez,0.2f);
        auto animatePez = Animate::create(animationPez);
        pezAux->runAction(RepeatForever::create(animatePez));
        
        auto pezListenerAux = EventListenerMouse::create();
        pezListenerAux->onMouseMove = CC_CALLBACK_1(ContraTiempo::onFishTouch, this, false);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(pezListenerAux, pezAux);
        
        auto actionAux = MoveTo::create(((visibleSize.width + abs(pezAux->getPositionX())) * 8) / visibleSize.width,
                                        Point(visibleSize.width+pezSize.width, pezAux->getPositionY()));
        pezAux->runAction(actionAux);
        
        pecesID.push_back(pezAux);
    }
    // Peces derecha - izquierda
    for(int i=0;i<30;i++){
        auto pezAux = Sprite::create("Files/Sprites/Pez/Pez1.png");
        pezAux->setScale(.5, .5);
        Point p1 = Point(RandomHelper::random_int(1000, RANGO_DE_APARICION+960), RandomHelper::random_int(20, 350));
        
        Size pezSize = pezAux->getContentSize();
        
        pezAux->setPosition(p1);
        addChild(pezAux,1);
        
        Vector<SpriteFrame*> framesPez;
        
        framesPez.pushBack(SpriteFrame::create("Files/Sprites/Pez/Pez1.png", Rect(0, 0, pezSize.width, pezSize.height)));
        framesPez.pushBack(SpriteFrame::create("Files/Sprites/Pez/Pez2.png", Rect(0, 0, pezSize.width, pezSize.height)));
        auto animationPez = Animation::createWithSpriteFrames(framesPez,0.2f);
        auto animatePez = Animate::create(animationPez);
        pezAux->runAction(RepeatForever::create(animatePez));
        
        auto pezListenerAux = EventListenerMouse::create();
        pezListenerAux->onMouseMove = CC_CALLBACK_1(ContraTiempo::onFishTouch, this, true);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(pezListenerAux, pezAux);
        
        auto actionAux = MoveTo::create(((abs(pezAux->getPositionX())) * 8) / visibleSize.width,
                                        Point(0-pezSize.width, pezAux->getPositionY()));
        pezAux->runAction(actionAux);
        
        pecesDI.push_back(pezAux);
        
    }
    
    // BOTAS DERECHA - IZQUIERDA
    for(int i=0;i<7;++i){
        auto botaAux = Sprite::create("Files/Sprites/Obstaculos/bota2.png");
        botaAux->setFlippedX(true);
        botaAux->setScale(.10, .10);
        Point p1 = Point(RandomHelper::random_int(1500, RANGO_DE_APARICION + 960), RandomHelper::random_int(20, 350));
        
        Size botaSize = botaAux->getContentSize();
        
        botaAux->setPosition(p1);
        addChild(botaAux,1);
        
        Vector<SpriteFrame*> framesBota;
        
        framesBota.pushBack(SpriteFrame::create("Files/Sprites/Obstaculos/bota1.png", Rect(0, 0, botaSize.width, botaSize.height)));
        framesBota.pushBack(SpriteFrame::create("Files/Sprites/Obstaculos/bota2.png", Rect(0, 0, botaSize.width, botaSize.height)));
        framesBota.pushBack(SpriteFrame::create("Files/Sprites/Obstaculos/bota3.png", Rect(0, 0, botaSize.width, botaSize.height)));
        auto animationBota = Animation::createWithSpriteFrames(framesBota,0.2f);
        auto animateBota = Animate::create(animationBota);
        botaAux->runAction(RepeatForever::create(animateBota));
        
        auto botaListenerAux = EventListenerMouse::create();
        botaListenerAux->onMouseMove = CC_CALLBACK_1(ContraTiempo::onObstaculoTouch, this, false);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(botaListenerAux, botaAux);
        
        auto actionAux = MoveTo::create(((abs(botaAux->getPositionX())) * 8) / visibleSize.width,
                                        Point(0-botaSize.width, botaAux->getPositionY()));
        botaAux->runAction(actionAux);
        
    }
    
    // BOTAS IZQUIERDA - DERECHA
    for(int i=0;i<7;++i){
        auto botaAux = Sprite::create("Files/Sprites/Obstaculos/bota2.png");
        botaAux->setScale(.10, .10);
        Point p1 = Point(RandomHelper::random_int(-RANGO_DE_APARICION, -500), RandomHelper::random_int(20, 350));
        
        Size botaSize = botaAux->getContentSize();
        
        botaAux->setPosition(p1);
        addChild(botaAux,1);
        
        Vector<SpriteFrame*> framesBota;
        
        framesBota.pushBack(SpriteFrame::create("Files/Sprites/Obstaculos/bota1.png", Rect(0, 0, botaSize.width, botaSize.height)));
        framesBota.pushBack(SpriteFrame::create("Files/Sprites/Obstaculos/bota2.png", Rect(0, 0, botaSize.width, botaSize.height)));
        framesBota.pushBack(SpriteFrame::create("Files/Sprites/Obstaculos/bota3.png", Rect(0, 0, botaSize.width, botaSize.height)));
        auto animationBota = Animation::createWithSpriteFrames(framesBota,0.2f);
        auto animateBota = Animate::create(animationBota);
        botaAux->runAction(RepeatForever::create(animateBota));
        
        auto botaListenerAux = EventListenerMouse::create();
        botaListenerAux->onMouseMove = CC_CALLBACK_1(ContraTiempo::onObstaculoTouch, this, false);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(botaListenerAux, botaAux);
        
        auto actionAux = MoveTo::create(((visibleSize.width + abs(botaAux->getPositionX())) * 8) / visibleSize.width,
                                        Point(visibleSize.width+botaSize.width, botaAux->getPositionY()));
        botaAux->runAction(actionAux);
        
    }
    
    // BARRILES DERECHA - IZQUIERDA
    for(int i=0;i<7;++i){
        auto barrilAux = Sprite::create("Files/Sprites/Obstaculos/barril1.png");
        barrilAux->setFlippedX(true);
        barrilAux->setScale(.13, .13);
        Point p1 = Point(RandomHelper::random_int(1500, RANGO_DE_APARICION + 960), RandomHelper::random_int(20, 335));
        
        Size barrilSize = barrilAux->getContentSize();
        
        barrilAux->setPosition(p1);
        addChild(barrilAux,1);
        
        Vector<SpriteFrame*> framesBarril;
        
        framesBarril.pushBack(SpriteFrame::create("Files/Sprites/Obstaculos/barril2.png", Rect(0, 0, barrilSize.width, barrilSize.height)));
        framesBarril.pushBack(SpriteFrame::create("Files/Sprites/Obstaculos/barril1.png", Rect(0, 0, barrilSize.width, barrilSize.height)));
        framesBarril.pushBack(SpriteFrame::create("Files/Sprites/Obstaculos/barril3.png", Rect(0, 0, barrilSize.width, barrilSize.height)));
        auto animationBota = Animation::createWithSpriteFrames(framesBarril,0.2f);
        auto animateBota = Animate::create(animationBota);
        barrilAux->runAction(RepeatForever::create(animateBota));
        
        auto botaListenerAux = EventListenerMouse::create();
        botaListenerAux->onMouseMove = CC_CALLBACK_1(ContraTiempo::onObstaculoTouch, this, true);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(botaListenerAux, barrilAux);
        
        auto actionAux = MoveTo::create(((abs(barrilAux->getPositionX())) * 8) / visibleSize.width,
                                        Point(0-barrilSize.width, barrilAux->getPositionY()));
        barrilAux->runAction(actionAux);
        
    }
    
    // BARRILES IZQUIERDA - DERECHA
    for(int i=0;i<7;++i){
        auto barrilAux = Sprite::create("Files/Sprites/Obstaculos/barril1.png");
        barrilAux->setScale(.13, .13);
        Point p1 = Point(RandomHelper::random_int(-RANGO_DE_APARICION, -500), RandomHelper::random_int(20, 335));
        
        Size barrilSize = barrilAux->getContentSize();
        
        barrilAux->setPosition(p1);
        addChild(barrilAux,1);
        
        Vector<SpriteFrame*> framesBarril;
        
        framesBarril.pushBack(SpriteFrame::create("Files/Sprites/Obstaculos/barril2.png", Rect(0, 0, barrilSize.width, barrilSize.height)));
        framesBarril.pushBack(SpriteFrame::create("Files/Sprites/Obstaculos/barril1.png", Rect(0, 0, barrilSize.width, barrilSize.height)));
        framesBarril.pushBack(SpriteFrame::create("Files/Sprites/Obstaculos/barril3.png", Rect(0, 0, barrilSize.width, barrilSize.height)));
        auto animationBota = Animation::createWithSpriteFrames(framesBarril,0.2f);
        auto animateBota = Animate::create(animationBota);
        barrilAux->runAction(RepeatForever::create(animateBota));
        
        auto botaListenerAux = EventListenerMouse::create();
        botaListenerAux->onMouseMove = CC_CALLBACK_1(ContraTiempo::onObstaculoTouch, this, true);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(botaListenerAux, barrilAux);
        
        auto actionAux = MoveTo::create(((visibleSize.width + abs(barrilAux->getPositionX())) * 8) / visibleSize.width,
                                        Point(visibleSize.width+barrilSize.width, barrilAux->getPositionY()));
        barrilAux->runAction(actionAux);
        
    }
    
    // MEDUSA DERECHA - IZQUIERDA
    for(int i=0;i<7;++i){
        auto medusaAux = Sprite::create("Files/Sprites/Jellyfish/Medusa1.png");
        medusaAux->setFlippedX(true);
        medusaAux->setScale(.13, .13);
        Point p1 = Point(RandomHelper::random_int(1500, RANGO_DE_APARICION + 960), RandomHelper::random_int(20, 345));
        
        Size medusaSize = medusaAux->getContentSize();
        
        medusaAux->setPosition(p1);
        addChild(medusaAux,1);
        
        Vector<SpriteFrame*> framesMedusa;
        
        framesMedusa.pushBack(SpriteFrame::create("Files/Sprites/Jellyfish/Medusa1.png", Rect(0, 0, medusaSize.width, medusaSize.height)));
        framesMedusa.pushBack(SpriteFrame::create("Files/Sprites/Jellyfish/Medusa2.png", Rect(0, 0, medusaSize.width, medusaSize.height)));
        auto animationMedusa = Animation::createWithSpriteFrames(framesMedusa,0.5f);
        auto animateMedusa = Animate::create(animationMedusa);
        medusaAux->runAction(RepeatForever::create(animateMedusa));
        
        auto botaListenerAux = EventListenerMouse::create();
        botaListenerAux->onMouseMove = CC_CALLBACK_1(ContraTiempo::onMedusaTouch, this, true);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(botaListenerAux, medusaAux);
        
        auto actionAux = MoveTo::create(((abs(medusaAux->getPositionX())) * 8) / visibleSize.width,
                                        Point(0-medusaSize.width, medusaAux->getPositionY()));
        medusaAux->runAction(actionAux);
        
    }
    
    // MEDUSA IZQUIERDA - DERECHA
    for(int i=0;i<7;++i){
        auto medusaAux = Sprite::create("Files/Sprites/Jellyfish/Medusa1.png");
        medusaAux->setScale(.13, .13);
        Point p1 = Point(RandomHelper::random_int(-RANGO_DE_APARICION, -500), RandomHelper::random_int(20, 345));
        
        Size medusaSize = medusaAux->getContentSize();
        
        medusaAux->setPosition(p1);
        addChild(medusaAux,1);
        
        Vector<SpriteFrame*> framesMedusa;
        
        framesMedusa.pushBack(SpriteFrame::create("Files/Sprites/Jellyfish/Medusa1.png", Rect(0, 0, medusaSize.width, medusaSize.height)));
        framesMedusa.pushBack(SpriteFrame::create("Files/Sprites/Jellyfish/Medusa2.png", Rect(0, 0, medusaSize.width, medusaSize.height)));
        auto animationMedusa = Animation::createWithSpriteFrames(framesMedusa,0.5f);
        auto animateMedusa = Animate::create(animationMedusa);
        medusaAux->runAction(RepeatForever::create(animateMedusa));
        
        auto medusaListenerAux = EventListenerMouse::create();
        medusaListenerAux->onMouseMove = CC_CALLBACK_1(ContraTiempo::onMedusaTouch, this, true);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(medusaListenerAux, medusaAux);
        
        auto actionAux = MoveTo::create(((visibleSize.width + abs(medusaAux->getPositionX())) * 8) / visibleSize.width,
                                        Point(visibleSize.width+medusaSize.width, medusaAux->getPositionY()));
        medusaAux->runAction(actionAux);
        
    }
    
    // TIBURON DERECHA - IZQUIERDA
    for(int i=0;i<3;++i){
        auto sharkAux = Sprite::create("Files/Sprites/Shark/tiburon1.png");
        sharkAux->setScale(.5, .5);
        Point p1 = Point(RandomHelper::random_int(1500, RANGO_DE_APARICION + 960), RandomHelper::random_int(20, 320));
        
        Size sharkSize = sharkAux->getContentSize();
        
        sharkAux->setPosition(p1);
        addChild(sharkAux,1);
        
        Vector<SpriteFrame*> framesShark;
        
        framesShark.pushBack(SpriteFrame::create("Files/Sprites/Shark/tiburon1.png", Rect(0, 0, sharkSize.width, sharkSize.height)));
        framesShark.pushBack(SpriteFrame::create("Files/Sprites/Shark/tiburon2.png", Rect(0, 0, sharkSize.width, sharkSize.height)));
        auto animationShark = Animation::createWithSpriteFrames(framesShark,0.5f);
        auto animateShark = Animate::create(animationShark);
        sharkAux->runAction(RepeatForever::create(animateShark));
        
        auto sharkListenerAux = EventListenerMouse::create();
        sharkListenerAux->onMouseMove = CC_CALLBACK_1(ContraTiempo::onSharkTouch, this, true);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(sharkListenerAux, sharkAux);
        
        auto actionAux = MoveTo::create(((abs(sharkAux->getPositionX())) * 9) / visibleSize.width,
                                        Point(0-sharkSize.width, sharkAux->getPositionY()));
        sharkAux->runAction(actionAux);
        
    }
    
    // TIBURON IZQUIERDA - DERECHA
    for(int i=0;i<3;++i){
        auto sharkAux = Sprite::create("Files/Sprites/Shark/tiburon1.png");
        sharkAux->setFlippedX(true);
        sharkAux->setScale(.5, .5);
        Point p1 = Point(RandomHelper::random_int(-RANGO_DE_APARICION, -500), RandomHelper::random_int(50, 320));
        
        Size sharkSize = sharkAux->getContentSize();
        
        sharkAux->setPosition(p1);
        addChild(sharkAux,1);
        
        Vector<SpriteFrame*> framesShark;
        
        framesShark.pushBack(SpriteFrame::create("Files/Sprites/Shark/tiburon1.png", Rect(0, 0, sharkSize.width, sharkSize.height)));
        framesShark.pushBack(SpriteFrame::create("Files/Sprites/Shark/tiburon2.png", Rect(0, 0, sharkSize.width, sharkSize.height)));
        auto animationShark = Animation::createWithSpriteFrames(framesShark,0.5f);
        auto animateShark = Animate::create(animationShark);
        sharkAux->runAction(RepeatForever::create(animateShark));
        
        auto sharkListenerAux = EventListenerMouse::create();
        sharkListenerAux->onMouseMove = CC_CALLBACK_1(ContraTiempo::onSharkTouch, this, false);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(sharkListenerAux, sharkAux);
        
        auto actionAux = MoveTo::create(((visibleSize.width + abs(sharkAux->getPositionX())) * 8) / visibleSize.width,
                                        Point(visibleSize.width+sharkSize.width, sharkAux->getPositionY()));
        sharkAux->runAction(actionAux);
        
    }
    
    // Caña de pescar
    _line = DrawNode::create();
    _line->setLineWidth(20); // NO FUNCIONA
    _line->drawLine(Point(visibleSize.width/2 + 18, 627), Point(visibleSize.width/2 + 18, 200), Color4F::BLACK);
    addChild(_line);
    
    _anzuelo = Sprite::create("Files/Sprites/AnzueloYGusano/anzuelogusano.png");
    _anzuelo->setPosition(Point(visibleSize.width/2 + 18, 200));
    _anzuelo->setScale(0.1);
    addChild(_anzuelo);
    
    return true;
}


void ContraTiempo::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}

void ContraTiempo::jugar(JuegoClasico::Ref* pSender){
    
}

bool ContraTiempo::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event) {
    auto pescador = event->getCurrentTarget()->getPosition();
    if (touch->getLocation().fuzzyEquals(pescador, 300)){
        log("Pescador touch x=%f, y=%f", touch->getLocation().x, touch->getLocation().y);
    }
    return true;
}

void ContraTiempo::onFishTouch(cocos2d::Event* event, bool derizq){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    EventMouse* e = (EventMouse*)event;
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(e);
    
    const float x = mouseEvent->getLocation().x;
    //const float y = mouseEvent->getLocation().y;
    const float y = _anzuelo->getPosition().y;
    
    auto fish_point = event->getCurrentTarget()->getPosition();
    int touchrange = derizq ? 120 : -125;
    //if (fish_point.fuzzyEquals(Point(visibleSize.width/2 + touchrange , 690 - y), 15)){
    if (fish_point.fuzzyEquals(Point(visibleSize.width/2 + touchrange , y), 15)){
        auto audio = AudioEngine::play2d("Files/Efectos/pescado.mp3");
        
        auto pezAux = event->getCurrentTarget();
        
        
        if(derizq){
            for(int i=0;i<pecesDI.size();i++){
                if (pecesDI[i] == pezAux) pecesDI.erase(pecesDI.begin() + i);
            }
        } else {
            for(int i=0;i<pecesID.size();i++){
                if (pecesID[i] == pezAux) pecesID.erase(pecesID.begin() + i);
            }
        }
        
        removeChild(event->getCurrentTarget());
        //event->getCurrentTarget()->setVisible(false);
        peces_pescados = peces_pescados + 1;
        puntuacionLabel->setString(std::to_string(peces_pescados));
        //log("Pescados: %d",peces_pescados);
        
       }
    //return true;
}

void ContraTiempo::onObstaculoTouch(cocos2d::Event* event, bool barril){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    EventMouse* e = (EventMouse*)event;
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(e);
    
    const float x = mouseEvent->getLocation().x;
    //const float y = mouseEvent->getLocation().y;
    const float y = _anzuelo->getPosition().y;
    
    auto obs_point = event->getCurrentTarget()->getPosition();
    int touchrange = barril ? 45 : 35;
    if (obs_point.fuzzyEquals(Point(visibleSize.width/2 , y), touchrange)){
        auto audio = AudioEngine::play2d("Files/Efectos/obstaculo.mp3");
        
        auto obsAux = event->getCurrentTarget();
        
        _eventDispatcher->removeEventListenersForTarget(obsAux);
        vidas_restantes = vidas_restantes - 1;
        vidasLabel->setString(std::to_string(vidas_restantes));
        
       }
}

void ContraTiempo::onMedusaTouch(cocos2d::Event *event, bool derizq){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    EventMouse* e = (EventMouse*)event;
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(e);
    
    const float x = mouseEvent->getLocation().x;
    //const float y = mouseEvent->getLocation().y;
    const float y = _anzuelo->getPosition().y;
    
    auto medusa_point = event->getCurrentTarget()->getPosition();
    int touchrange = 35;
    if (medusa_point.fuzzyEquals(Point(visibleSize.width/2 , y), touchrange)){
        auto audio = AudioEngine::play2d("Files/Efectos/medusa.mp3");
        
        auto medusaAux = event->getCurrentTarget();
        //medusaAux->setDisplayFrame
        
        _eventDispatcher->removeEventListenersForTarget(medusaAux);
        vidas_restantes = vidas_restantes - 1;
        vidasLabel->setString(std::to_string(vidas_restantes));
        
       }
}

void ContraTiempo::onSharkTouch(cocos2d::Event *event, bool derizq){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    EventMouse* e = (EventMouse*)event;
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(e);
    
    const float x = mouseEvent->getLocation().x;
    const float y = _anzuelo->getPosition().y;
    
    auto shark_point = event->getCurrentTarget()->getPosition();
    int touchrange = derizq ? 250 : -200;
    if (shark_point.fuzzyEquals(Point(visibleSize.width/2 + touchrange , y), 30)){
        auto audio = AudioEngine::play2d("Files/Efectos/bite.mp3");
        
        auto sharkAux = event->getCurrentTarget();
        //medusaAux->setDisplayFrame
        
        _eventDispatcher->removeEventListenersForTarget(sharkAux);
        vidas_restantes = vidas_restantes - 1;
        vidasLabel->setString(std::to_string(vidas_restantes));
        
       }
}

void ContraTiempo::onCangrejoTouch(cocos2d::Event *event, bool derizq){
    
}


void ContraTiempo::onMouseMove(cocos2d::Event* event){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    EventMouse* e = (EventMouse*)event;
    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(e);
    
    const float x = mouseEvent->getLocation().x;
    const float y = mouseEvent->getLocation().y;
    removeChild(_line);
    if(y < 90){
        _line = DrawNode::create();
        _line->setLineWidth(20); // NO FUNCIONA
        _line->drawLine(Point(visibleSize.width/2 + 18, 627), Point(visibleSize.width/2 + 18, 600), Color4F::BLACK);
        addChild(_line);
        _anzuelo->setPosition(Point(visibleSize.width/2 + 18, 600));
    } else {
        _line = DrawNode::create();
        _line->setLineWidth(20); // NO FUNCIONA
        _line->drawLine(Point(visibleSize.width/2 + 18, 627), Point(visibleSize.width/2 + 18, 690-y), Color4F::BLACK);
        addChild(_line);
        _anzuelo->setPosition(Point(visibleSize.width/2 + 18, 690-y));
    }
    
    bool pecesEnEscenaID = false;
    for(int i=0;i<pecesID.size();i++){
        
        if(pecesID[i]->getPosition().x < visibleSize.width+pecesID[i]->getContentSize().width){
            pecesEnEscenaID = true;
            break;
        }
    }
    
    bool pecesEnEscenaDI = false;
    for(int i=0;i<pecesDI.size();i++){
        
        if(pecesDI[i]->getPosition().x > 0-pecesDI[i]->getContentSize().width){
            pecesEnEscenaDI = true;
        }
    }
    if(!pecesEnEscenaID && !pecesEnEscenaDI){
        // SONIDO DE VICTORIA
        UserDefault::getInstance()->setIntegerForKey("pecesPescados", ContraTiempo::peces_pescados);
        UserDefault::getInstance()->setIntegerForKey("vidasRestantes", ContraTiempo::vidas_restantes);
        UserDefault::getInstance()->setBoolForKey("ContraTiempo", true);
        Director::getInstance()->replaceScene(TransitionFade::create(1, GameOver::createScene() , Color3B(255, 255, 255)));
    }
    
    if(vidas_restantes == 0){
        // SONIDO DE LOSER
        UserDefault::getInstance()->setIntegerForKey("pecesPescados", ContraTiempo::peces_pescados);
        UserDefault::getInstance()->setIntegerForKey("vidasRestantes", ContraTiempo::vidas_restantes);
        UserDefault::getInstance()->setBoolForKey("ContraTiempo", true);
        Director::getInstance()->replaceScene(TransitionFade::create(1, GameOver::createScene() , Color3B(255, 255, 255)));
    }
}

void ContraTiempo::goBack(Ref* psender){
    Director::getInstance()->popScene();
}

void ContraTiempo::gameOver(JuegoClasico::Ref* pSender){
    Director::getInstance()->resume();
    auto scene = GameOver::createScene();
    Director::getInstance()->replaceScene(TransitionFadeTR::create(1, scene));
}

void ContraTiempo::update(float dt) {
  if (timeRemaining > 0.f) {
    timeRemaining -= dt;
      
    if(timeRemaining >= 60) tiempoLabel->setString("1:00");
    else if(timeRemaining < 10) tiempoLabel->setString("0:0" + std::to_string((int)timeRemaining));
    else tiempoLabel->setString("0:" + std::to_string((int)timeRemaining));
    
    if (timeRemaining <= 0.f) {
        UserDefault::getInstance()->setIntegerForKey("pecesPescados", ContraTiempo::peces_pescados);
        UserDefault::getInstance()->setBoolForKey("ContraTiempo", true);
        UserDefault::getInstance()->setIntegerForKey("vidasRestantes", ContraTiempo::vidas_restantes);
        Director::getInstance()->replaceScene(TransitionFade::create(1, GameOver::createScene() , Color3B(255, 255, 255)));
    }
  }
}
