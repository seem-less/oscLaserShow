#ifndef OSCLASER_H
#define OSCLASER_H

#include "oscdecorator.h"
#include "ofxLaserManager.h"

struct LaserConfig {
    int laserWidth;
    int laserHeight;
    int tcpPort;
    int portListen;            ///< port to listen on
    int portSend;            ///< port to listen on
};


class OscLaser : public OscDecorator
{
public:
    OscLaser();
    ~OscLaser();
    ofxLaser::Manager laserManager;

    void setup(int udpListeningPort, int udpSendingPort, int tcpPort, int laserWidth = 800, int laserHeight = 800);
    void update();
    void setArmStatus(bool isOn);
    void setGlobalBrightness(float oscGlobalBrightness);
    void refreshControllerList(bool refreshing);
    void draw();

    void setSetupComplete(bool newSetupComplete);
    bool getSetupComplete() const;
    int getCurrentLaserEffect() const;
    const ofParameter<int> &getRenderProfileIndex() const;
    const LaserConfig &getConfig() const;
    string getRenderProfile();

    float getElapsedTime() const;
    void setElapsedTime(float newElapsedTime);

private:
    void assigningToLaser(string &oscSelectedControllerLabel);

    LaserConfig config;

    int numLaserEffects;
    int currentLaserEffect = 1;
    float maskBottomHeight;
    float maskTopHeight;
    bool setupComplete = false;

    ofParameter<int> renderProfileIndex = 0;

    std::vector<ofPolyline> polyLines;
    ofParameter<ofColor> colour;

    float elapsedTime;
    ofParameter<float>timeSpeed;
};

#endif // OSCLASER_H
