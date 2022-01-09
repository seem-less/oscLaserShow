#include "oscLaser.h"

// setup
OscLaser::OscLaser()
{
    ofAddListener(this->notifyControllerLabelChange, this, &OscLaser::assigningToLaser);
}

OscLaser::~OscLaser()
{
    laserManager.deleteLaser(0);
}

// setup
void OscLaser::setup(int udpListeningPort, int udpSendingPort, int tcpPort , int laserWidth, int laserHeight) {

    //Setup OSC over UDP
    oscReciever.setup(udpListeningPort);
    oscReciever.start();
    oscSender.setup("localhost" ,udpSendingPort);

    config.laserWidth = laserWidth;
    config.laserHeight = laserHeight;
    setTcpPort(tcpPort);

    //Setup ofxLaser
    laserManager.setCanvasSize(config.laserWidth,config.laserHeight);
    numLaserEffects = 20;
    currentLaserEffect = 1;

    //select default laser
    laserManager.setSelectedLaser(0);

    // removing default zones
    while(laserManager.getNumZones() > 0){
        laserManager.deleteZone(&laserManager.getZone(0));
    }

    // create and add zone to laser. zone's are needed otherwise nothing will be sent to the laser
    laserManager.addZone(0,0,config.laserWidth,config.laserHeight);
    laserManager.addZoneToLaser(0,0);
    // set zone output box size
    laserManager.getLaser(0).getLaserZoneForZone(&laserManager.getZone(0))->zoneTransform.setDst(ofRectangle(0,0,config.laserWidth,config.laserHeight));

    // remove default masks
    for(int i = 0; laserManager.getLaser(0).maskManager.quads.size() > 0; i++){
        laserManager.getLaser(0).maskManager.deleteQuadMask(laserManager.getLaser(0).maskManager.quads.at(i));
    }
    // add new mask to laser
    laserManager.getLaser(0).maskManager.addQuadMask(getOscMaskLevel());

    setSetupComplete(true);
}

// update
void OscLaser::update(){
    // check if setup method called in ofApp::setup()
    if(!getSetupComplete()){
        ofLog(OF_LOG_FATAL_ERROR) << "OSC send and recieve ports not set. Did you forget to run the laser setup() method in ofApp::setup()?" << endl;
        ofExit();
    }

    // recieve messages from OSC
    OscUpdate();

    // if there are 0 clients connected, cut off laser
    if(numberOfConnectedClients(TCP) == 0){
        setArmStatus(false);
    }else{
        // updating laser config from OSC values
        // safety relevant OSC parameters
        setArmStatus(getOscLaserOn());
        setGlobalBrightness(getOscGlobalBrightness());
        // all other OSC parameters
        maskBottomHeight = ofMap(getOscBottomMaskHeight(),1,0,0,config.laserHeight);
        maskTopHeight = ofMap(getOscTopMaskHeight(),1,0,0,config.laserHeight);
        for(ofxLaser::QuadMask* mask : laserManager.getLaser(0).maskManager.quads){
            mask->set(0,maskTopHeight,config.laserWidth,maskBottomHeight-maskTopHeight);
            mask->maskLevel = getOscMaskLevel() * 100;
        }
        currentLaserEffect = getOscPatternChoice();
        refreshControllerList(getOscRefreshingControllerList());

    }

    // ofxLaser update
    laserManager.update();

}

// draw
void OscLaser::draw(){
    // LASER POLYLINES
    for(size_t i = 0; i<polyLines.size(); i++) {
        laserManager.drawPoly(polyLines[i], colour, getRenderProfile());
    }

    // sends points to the DAC
    laserManager.send();

    // if would like to draw the UI...
//    laserManager.drawUI();
}

string OscLaser::getRenderProfile(){

    string renderProfile;
        switch (getRenderProfileIndex()) {
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
    return renderProfile;
}


void OscLaser::setArmStatus(bool isOn){
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

void OscLaser::setGlobalBrightness(float oscGlobalBrightness){
    laserManager.globalBrightness = getOscGlobalBrightness();
    sendFloat("/global_brightness",laserManager.globalBrightness);
}

void OscLaser::refreshControllerList(bool refreshing){
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

void OscLaser::assigningToLaser(string &dacControllerLabel){
    cout<<dacControllerLabel<<endl;
    laserManager.dacAssigner.assignToLaser(dacControllerLabel,laserManager.getLaser(0));
}

float OscLaser::getElapsedTime() const
{
    return elapsedTime;
}

void OscLaser::setElapsedTime(float newElapsedTime)
{
    elapsedTime = newElapsedTime;
}

const LaserConfig &OscLaser::getConfig() const
{
    return config;
}

const ofParameter<int> &OscLaser::getRenderProfileIndex() const
{
    return renderProfileIndex;
}

int OscLaser::getCurrentLaserEffect() const
{
    return currentLaserEffect;
}

bool OscLaser::getSetupComplete() const
{
    return setupComplete;
}

void OscLaser::setSetupComplete(bool newSetupComplete)
{
    setupComplete = newSetupComplete;
}
