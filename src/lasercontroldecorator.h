#ifndef LASERCONTROLDECORATOR_H
#define LASERCONTROLDECORATOR_H

#include "oscdecorator.h"
#include "ofxLaserManager.h"

class LaserControlDecorator : public OscDecorator
{
public:
    LaserControlDecorator();
    ~LaserControlDecorator();
    ofxLaser::Manager laserManager;

    void update();
    void setArmStatus(bool isOn);
    void setGlobalBrightness(float oscGlobalBrightness);
    void refreshControllerList(bool refreshing);
    void drawPatterns();

private:
    void showLaserEffect(int currentLaserEffect);
    void assigningToLaser(string &oscSelectedControllerLabel);

    float laserWidth;
    float laserHeight;
    int numLaserEffects;
    int currentLaserEffect = 1;
    float maskBottomHeight;
    float maskTopHeight;

    ofParameter<int> renderProfileIndex = 0;
    ofParameter<string> renderProfileLabel;

    std::vector<ofPolyline> polyLines;
    ofParameter<ofColor> colour;

    float elapsedTime;
    ofParameter<float>timeSpeed;
};

#endif // LASERCONTROLDECORATOR_H
