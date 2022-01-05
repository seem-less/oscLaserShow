#include "lasercontroldecorator.h"

// setup
LaserControlDecorator::LaserControlDecorator()
{
    ofAddListener(this->notifyControllerLabelChange, this, &LaserControlDecorator::assigningToLaser);

    //Setup ofxLaser
    laserWidth = 800;
    laserHeight = 800;
    laserManager.setCanvasSize(laserWidth,laserHeight);
    numLaserEffects = 20;
    currentLaserEffect = 1;

    //select default laser
    laserManager.setSelectedLaser(0);

    // removing default zones
    while(laserManager.getNumZones() > 0){
        laserManager.deleteZone(&laserManager.getZone(0));
    }

    // create and add zone to laser. zone's are needed otherwise nothing will be sent to the laser
    laserManager.addZone(0,0,laserWidth,laserHeight);
    laserManager.addZoneToLaser(0,0);
    // set zone output box size
    laserManager.getLaser(0).getLaserZoneForZone(&laserManager.getZone(0))->zoneTransform.setDst(ofRectangle(0,0,laserWidth,laserHeight));

    // remove default masks
    for(int i = 0; laserManager.getLaser(0).maskManager.quads.size() > 0; i++){
        laserManager.getLaser(0).maskManager.deleteQuadMask(laserManager.getLaser(0).maskManager.quads.at(i));
    }
    // add new mask to laser
    laserManager.getLaser(0).maskManager.addQuadMask(getOscMaskLevel());

}

LaserControlDecorator::~LaserControlDecorator()
{
    laserManager.deleteLaser(0);
}

// update
void LaserControlDecorator::update(){

    // updating laser config from OSC values
    // safety relevant OSC parameters
    setArmStatus(getOscLaserOn());
    setGlobalBrightness(getOscGlobalBrightness());
    // all other OSC parameters
    maskBottomHeight = ofMap(getOscBottomMaskHeight(),1,0,0,laserHeight);
    maskTopHeight = ofMap(getOscTopMaskHeight(),1,0,0,laserHeight);
    for(ofxLaser::QuadMask* mask : laserManager.getLaser(0).maskManager.quads){
        mask->set(0,maskTopHeight,laserWidth,maskBottomHeight-maskTopHeight);
        mask->maskLevel = getOscMaskLevel() * 100;
    }
    currentLaserEffect = getOscPatternChoice();
    refreshControllerList(getOscRefreshingControllerList());

    // ofxLaser update
    float deltaTime = ofClamp(ofGetLastFrameTime(), 0, 0.2);
    elapsedTime += (deltaTime * getOscAnimationSpeed());
    laserManager.update();
}

// draw
void LaserControlDecorator::drawPatterns(){
    showLaserEffect(currentLaserEffect);

    // sends points to the DAC
    laserManager.send();

    // if would like to draw the UI...
//    laserManager.drawUI();
}

void LaserControlDecorator::setArmStatus(bool isOn){
    if(laserManager.areAllLasersArmed()){
        sendBool("/arm_lasers", true);
    }else{
        sendBool("/arm_lasers", false);
    }
    // arming lasers
    if(isOn){
        laserManager.armAllLasers();
    }else{
        laserManager.disarmAllLasers();
    }
}

void LaserControlDecorator::setGlobalBrightness(float oscGlobalBrightness){
    laserManager.globalBrightness = getOscGlobalBrightness();
    sendFloat("/global_brightness",laserManager.globalBrightness);
}

void LaserControlDecorator::refreshControllerList(bool refreshing){
    // refreshing controller list
    if(refreshing){
        laserManager.dacAssigner.updateDacList();
        ofxOscBundle bundle;
        for(const ofxLaser::DacData& dac : laserManager.dacAssigner.getDacList()) {
            ofxOscMessage(sendMsg);
            sendMsg.setAddress("/controller_list");
            cout<<dac.label<<endl;
            if(dac.available) sendMsg.addStringArg(dac.label);
            bundle.addMessage(sendMsg);
        }
        oscSender.sendBundle(bundle);
        setOscRefreshingControllerList(false);
    }
}

void LaserControlDecorator::assigningToLaser(string &dacControllerLabel){
    cout<<dacControllerLabel<<endl;
    laserManager.dacAssigner.assignToLaser(dacControllerLabel,laserManager.getLaser(0));
}

void LaserControlDecorator::showLaserEffect(int effectnum) {

    string renderProfile;
    switch (renderProfileIndex) {
        case 0 :
            renderProfile = OFXLASER_PROFILE_DEFAULT;
            break;
        case 1 :
            renderProfile = OFXLASER_PROFILE_DETAIL;
            break;
        case 2 :
            renderProfile = OFXLASER_PROFILE_FAST;
            break;
    }
    renderProfileLabel = "Render Profile : OFXLASER_PROFILE_" + renderProfile;


    float left = laserWidth*0.1;
    float top = laserHeight*0.1;
    float right = laserWidth*0.9;
    float bottom = laserHeight*0.9;
    float width = laserWidth*0.8;
    float height = laserHeight*0.8;

    switch (effectnum) {

        case 1: {

            // LASER LINES
            int numlines = 7;

            for(int i = 0; i<numlines; i++) {

                float progress =(float)i/(float)(numlines-1);
                float hue =(float)i/(float)(numlines);

                float xpos =left + (width*progress);

                laserManager.drawLine(ofPoint(xpos, top+height*0.1), ofPoint(xpos, top+height*0.4), ofColor(255), renderProfile);

                ofColor c;
                c.setHsb(hue*255, 255, 255);

                laserManager.drawLine(ofPoint(xpos, top+height*0.6), ofPoint(xpos, top+height*0.9), c, renderProfile);

            }

      break;

        }


        case 2: {

            // LASER LINES ANIMATING
            int numlines = 7;

            for(int i = 0; i<numlines; i++) {

                float progress =(float)i/(float)(numlines-1);
                float hue =(float)i/(float)(numlines);

                float xpos =left + (width*progress) + (sin(elapsedTime*4+i*0.5)*width*0.05);

                laserManager.drawLine(ofPoint(xpos, top+height*0.1), ofPoint(xpos, top+height*0.4), ofColor(255), renderProfile);
                ofColor c;
                c.setHsb(hue*255, 255, 255);
                laserManager.drawLine(ofPoint(xpos, top+height*0.6), ofPoint(xpos, top+height*0.9), c, renderProfile);

            }

            break;

        }


        case 3: {

            // LASER CIRCLES
            int numCircles = 4;

            for(int i = 0; i<numCircles; i++) {

                float progress =(float)i/(float)(numCircles-1);
                float hue =(float)i/(float)(numCircles);

                float xpos =left + (width*progress);

                laserManager.drawCircle(ofPoint(xpos, top+height*0.3),30, ofColor(255), renderProfile);
                ofColor c;
                c.setHsb(hue*255, 255, 255);

                laserManager.drawCircle(ofPoint(xpos, top+height*0.7), 30, c, renderProfile);

            }

            break;

        }

        case 4: {

            // LASER CIRCLES ANIMATING
            int numCircles = 4;

            for(int i = 0; i<numCircles; i++) {

                float progress =(float)i/(float)(numCircles-1);
                float hue =(float)i/(float)(numCircles);

                float xpos =left + (width*progress) + (sin(elapsedTime*4+i*0.5)*width*0.05);

                laserManager.drawCircle(ofPoint(xpos, top+height*0.3), 30, ofColor::white, renderProfile);
                ofColor c;
                c.setHsb(hue*255, 255, 255);

                laserManager.drawCircle(ofPoint(xpos, top+height*0.7), 30, c, renderProfile);

            }

            break;

        }

        case 5: {

            // LASER PARTICLES
            int numParticles = 12;

            for(int i = 0; i<numParticles; i++) {

                float progress =(float)i/(float)(numParticles-1);

                float xpos =left + (width*progress) ;

                laserManager.drawDot(ofPoint(xpos, top+height*0.3), ofColor(255),1, renderProfile);
                ofColor c;
                c.setHsb(progress*255, 255, 255);
                laserManager.drawDot(ofPoint(xpos, top+height*0.7), c, 1,  renderProfile);

            }

            break;

        }
        case 6: {

            // LASER PARTICLES ANIMATING

            float speed = 1;
            for(int i = 0; i<30; i++) {

                ofColor c;
                c.setHsb(i*6,255,255);
                ofPoint p;
                float spread = ofMap(cos(elapsedTime*0.4),1,-1,0.01,0.1);
                p.x = sin((elapsedTime-((float)i*spread)) *1.83f * speed) * 300;
                p.y = sin((elapsedTime-((float)i*spread)) *2.71f *speed) * 300;
                p.x+=laserWidth/2;
                p.y+=laserHeight/2;

                laserManager.drawDot(p, c, 1, renderProfile);

            }

            break;

        }
        case 7: {

            // 3D rotation
            // you don't need to wrap your draw calls in
            // laser.beginDraw() and laser.endDraw() unless
            // you're doing 3D (it fixes the viewport perspective)

            laserManager.beginDraw();

            float speed = 20;
            ofPushMatrix();
            ofTranslate(laserWidth/2,laserHeight/2);
            ofRotateYDeg(elapsedTime*speed);
            int hue = (int)(elapsedTime*32)%255; // 8 seconds to go around
            ofColor c;
            c.setHsb(hue, 255, 255);

            ofPolyline poly;

            for(int j = 0; j<4; j++) {
                poly.clear();
                ofPushMatrix();
                ofRotateXDeg(j*90);

                poly.addVertex(glm::vec3(100,-100,100));
                poly.addVertex(glm::vec3(100, 100,100));
                poly.addVertex(glm::vec3(-100, 100,100));
                poly.addVertex(glm::vec3(-100, -100,100));
                laserManager.drawPoly(poly, c, renderProfile);

                ofPopMatrix();
            }
            ofPopMatrix();

            laserManager.endDraw();


            break;

        }case 8: {

            // 3D rotation
            // you don't need to wrap your draw calls in
            // laser.beginDraw() and laser.endDraw() unless
            // you're doing 3D (it fixes the viewport perspective)
            laserManager.drawCircle(laserWidth/2,laserHeight/2,laserWidth/2,ofColor::blue);
            break;

        }case 9: {

            laserManager.drawLine(glm::vec3(0,laserHeight/2,0),glm::vec3(laserWidth,laserHeight/2,0),ofColor::blue);
            break;

        }
    }

    // LASER POLYLINES
    for(size_t i = 0; i<polyLines.size(); i++) {
        laserManager.drawPoly(polyLines[i], colour, renderProfile );
    }

}




