#include "oscdecorator.h"

OscDecorator::OscDecorator()
{
}

OscDecorator::~OscDecorator(){

}

void OscDecorator::OscUpdate(){

    // restart TCP server if it is down
    if(!TCP.isConnected()){
        static int time = ofGetElapsedTimeMillis();
        if(ofGetElapsedTimeMillis()-time>7000){
            TCP.close();
            if(TCP.setup(tcpPort)) ofLog() << "Server successfully up and running on port: "<< tcpPort;
            time = ofGetElapsedTimeMillis();
        }
    }
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
            delete mes;
    }
}

void OscDecorator::setTcpPort(int newTcpPort)
{
    tcpPort = newTcpPort;
}

int OscDecorator::numberOfConnectedClients(ofxTCPServer& TCP){
    // check how many clients are currently connected
    int currentlyConnectedClients = 0;
    for(int i = 0; i < TCP.getLastID(); i++)
    {
        if( TCP.isClientConnected(i) ) currentlyConnectedClients++;
    }
    return currentlyConnectedClients;
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
        ofNotifyEvent(notifyControllerLabelChange, *getOscSelectedControllerLabel(), this);
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



