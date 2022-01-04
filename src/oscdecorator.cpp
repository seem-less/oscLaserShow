#include "oscdecorator.h"

OscDecorator::OscDecorator()
{
    //Setup OSC
    oscReciever.setup(OSC_RECIEVE_PORT);
    oscReciever.start();
    oscSender.setup("localhost" ,OSC_SEND_PORT);
}

OscDecorator::~OscDecorator(){

}

void OscDecorator::OscUpdate(){
    while(oscReciever.hasWaitingMessages()){
        ofxOscMessage* mes = new ofxOscMessage;
        oscReciever.getNextMessage(*mes);

        recieveOscPatternChoice(mes);
        recieveOscLaserOn(mes);
        recieveOscRefreshingControllerList(mes);
        recieveOscSelectedControllerLabel(mes);
        recieveOscAnimationSpeed(mes);
        recieveOscMaskHeight(mes);
        recieveOscMaskLevel(mes);
        recieveOscGlobalBrightness(mes);

    }
}

void OscDecorator::recieveOscPatternChoice(ofxOscMessage *mes){
    if(mes->getAddress() == "/pattern_choice"){
        oscPatternChoice = mes->getArgAsInt(0);
    }
}

void OscDecorator::recieveOscLaserOn(ofxOscMessage *mes){
    if(mes->getAddress() == "/arm_lasers"){
        oscLaserOn = mes->getArgAsBool(0);
    }
}

void OscDecorator::recieveOscRefreshingControllerList(ofxOscMessage *mes){
    if(mes->getAddress() == "/refresh_controller_list"){
        oscRefreshingControllerList = mes->getArgAsBool(0);
    }
}

void OscDecorator::recieveOscSelectedControllerLabel(ofxOscMessage *mes){
    if(mes->getAddress() == "/selected_controller"){
        oscSelectedControllerLabel = mes->getArgAsString(0);
        controllerLabelChanged();
    }
}

void OscDecorator::recieveOscAnimationSpeed(ofxOscMessage *mes)
{
   if(mes->getAddress()=="/animation_speed"){
       oscAnimationSpeed = mes->getArgAsFloat(0);
   }
}
void OscDecorator::recieveOscMaskHeight(ofxOscMessage *mes){
    if(mes->getAddress()=="/mask_range"){
        oscBottomMaskHeight = mes->getArgAsFloat(0);
        oscTopMaskHeight = mes->getArgAsFloat(1);
    }
}

void OscDecorator::recieveOscMaskLevel(ofxOscMessage *mes)
{
   if(mes->getAddress()=="/mask_level"){
        oscMaskLevel = mes->getArgAsFloat(0);
   }
}

void OscDecorator::recieveOscGlobalBrightness(ofxOscMessage *mes)
{
   if(mes->getAddress()=="/global_brightness"){
       oscGlobalBrightness = mes->getArgAsFloat(0);
   }
}

void OscDecorator::sendBool(string address, bool boolStatus){
    ofxOscMessage(sendMsg);
    sendMsg.setAddress(address);
    sendMsg.addBoolArg(boolStatus);
    oscSender.sendMessage(sendMsg);
}

void OscDecorator::sendFloat(string address, float number){
    ofxOscMessage(sendMsg);
    sendMsg.setAddress(address);
    sendMsg.addFloatArg(number);
    oscSender.sendMessage(sendMsg);
}

void OscDecorator::controllerLabelChanged(){
    ofNotifyEvent(notifyControllerLabelChange, *getOscSelectedControllerLabel(), this);
}

float OscDecorator::getOscGlobalBrightness() const
{
    return oscGlobalBrightness;
}

void OscDecorator::setOscRefreshingControllerList(bool newOscRefreshingControllerList)
{
    oscRefreshingControllerList = newOscRefreshingControllerList;
}

float OscDecorator::getOscMaskLevel() const
{
    return oscMaskLevel;
}

float OscDecorator::getOscTopMaskHeight() const
{
    return oscTopMaskHeight;
}

float OscDecorator::getOscBottomMaskHeight() const
{
    return oscBottomMaskHeight;
}

bool OscDecorator::getOscRefreshingControllerList() const
{
    return oscRefreshingControllerList;
}

bool OscDecorator::getOscLaserOn() const
{
    return oscLaserOn;
}

float OscDecorator::getOscAnimationSpeed() const
{
    return oscAnimationSpeed;
}

int OscDecorator::getOscPatternChoice() const
{
    return oscPatternChoice;
}


string* OscDecorator::getOscSelectedControllerLabel(){
    return &oscSelectedControllerLabel;
}



