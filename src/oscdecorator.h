#ifndef OSCDECORATOR_H
#define OSCDECORATOR_H

#include "ofMain.h"
#include "ofxOsc.h"
#include "ofxNetwork.h"

class OscDecorator
{
public:
    OscDecorator();
    ~OscDecorator();
    void OscUpdate();

    ofxTCPServer TCP;
    ofxOscSender oscSender;
    ofxOscReceiver oscReciever;

    ofEvent<string> notifyControllerLabelChange;

    void sendBool(string address, bool boolStatus);
    void sendFloat(string address, float number);

    int getOscPatternChoice() const;
    float getOscAnimationSpeed() const;
    bool getOscLaserOn() const;
    bool getOscRefreshingControllerList() const;
    string* getOscSelectedControllerLabel();
    float getOscBottomMaskHeight() const;
    float getOscTopMaskHeight() const;
    float getOscMaskLevel() const;
    float getOscGlobalBrightness() const;

    void setOscRefreshingControllerList(bool newOscRefreshingControllerList);
    void setTcpPort(int newTcpPort);
    int numberOfConnectedClients(ofxTCPServer& TCP);

private:
    void recieveOscPatternChoice(ofxOscMessage* mes);
    void recieveOscAnimationSpeed(ofxOscMessage* mes);
    void recieveOscLaserOn(ofxOscMessage* mes);
    void recieveOscRefreshingControllerList(ofxOscMessage* mes);
    void recieveOscSelectedControllerLabel(ofxOscMessage* mes);
    void recieveOscMaskHeight(ofxOscMessage* mes);
    void recieveOscMaskLevel(ofxOscMessage* mes);
    void recieveOscGlobalBrightness(ofxOscMessage* mes);

    int oscPatternChoice = 0;
    float oscAnimationSpeed = 1;
    bool oscLaserOn = false;
    bool oscRefreshingControllerList = false;
    string oscSelectedControllerLabel;
    float oscBottomMaskHeight = 0;
    float oscTopMaskHeight = 0.5;
    float oscMaskLevel = 0.5;
    float oscGlobalBrightness = 0.3;
    int tcpPort;
};

#endif // OSCDECORATOR_H
