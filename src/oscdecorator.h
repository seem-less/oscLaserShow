#ifndef OSCDECORATOR_H
#define OSCDECORATOR_H

#include "basepattern.h"
#include "ofxOsc.h"
#define OSC_RECIEVE_PORT 9000
#define OSC_SEND_PORT 9001

class OscDecorator : public BasePattern
{
public:
    OscDecorator();
    ~OscDecorator();
    void OscUpdate();

    ofEvent<string> notifyControllerLabelChange;

    void sendBool(string address, bool boolStatus);
    void sendFloat(string address, float number);

    ofxOscSender oscSender;
    ofxOscReceiver oscReciever;

    int getOscPatternChoice() const;
    float getOscAnimationSpeed() const;
    bool getOscLaserOn() const;
    bool getOscRefreshingControllerList() const;
    string* getOscSelectedControllerLabel();
    float getOscBottomMaskHeight() const;
    float getOscTopMaskHeight() const;
    float getOscMaskLevel() const;

    void setOscRefreshingControllerList(bool newOscRefreshingControllerList);

    float getOscGlobalBrightness() const;

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
};

#endif // OSCDECORATOR_H
