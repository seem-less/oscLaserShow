#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    laser.setup(9000,9001,9002);
}

//--------------------------------------------------------------
void ofApp::update(){

    // update laser config
    laser.update();

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofBackground(0);

    showLaserEffect(laser.getCurrentLaserEffect());
    // drawing patterns
    laser.draw();

}

void ofApp::showLaserEffect(int effectnum) {

    // timing for animations
    float deltaTime = ofClamp(ofGetLastFrameTime(), 0, 0.2);
    laser.setElapsedTime(laser.getElapsedTime() + (deltaTime * laser.getOscAnimationSpeed()));

    float left = laser.getConfig().laserWidth*0.1;
    float top = laser.getConfig().laserHeight*0.1;
    float right = laser.getConfig().laserWidth*0.9;
    float bottom = laser.getConfig().laserHeight*0.9;
    float width = laser.getConfig().laserWidth*0.8;
    float height = laser.getConfig().laserHeight*0.8;

    switch (effectnum) {

        case 1: {

            // LASER LINES
            int numlines = 7;

            for(int i = 0; i<numlines; i++) {

                float progress =(float)i/(float)(numlines-1);
                float hue =(float)i/(float)(numlines);

                float xpos =left + (width*progress);

                laser.laserManager.drawLine(ofPoint(xpos, top+height*0.1), ofPoint(xpos, top+height*0.4), ofColor(255), laser.getRenderProfile());

                ofColor c;
                c.setHsb(hue*255, 255, 255);

                laser.laserManager.drawLine(ofPoint(xpos, top+height*0.6), ofPoint(xpos, top+height*0.9), c, laser.getRenderProfile());

            }

      break;

        }


        case 2: {

            // LASER LINES ANIMATING
            int numlines = 7;

            for(int i = 0; i<numlines; i++) {

                float progress =(float)i/(float)(numlines-1);
                float hue =(float)i/(float)(numlines);

                float xpos =left + (width*progress) + (sin(laser.getElapsedTime()*4+i*0.5)*width*0.05);

                laser.laserManager.drawLine(ofPoint(xpos, top+height*0.1), ofPoint(xpos, top+height*0.4), ofColor(255), laser.getRenderProfile());
                ofColor c;
                c.setHsb(hue*255, 255, 255);
                laser.laserManager.drawLine(ofPoint(xpos, top+height*0.6), ofPoint(xpos, top+height*0.9), c, laser.getRenderProfile());

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

                laser.laserManager.drawCircle(ofPoint(xpos, top+height*0.3),30, ofColor(255), laser.getRenderProfile());
                ofColor c;
                c.setHsb(hue*255, 255, 255);

                laser.laserManager.drawCircle(ofPoint(xpos, top+height*0.7), 30, c, laser.getRenderProfile());

            }

            break;

        }

        case 4: {

            // LASER CIRCLES ANIMATING
            int numCircles = 4;

            for(int i = 0; i<numCircles; i++) {

                float progress =(float)i/(float)(numCircles-1);
                float hue =(float)i/(float)(numCircles);

                float xpos =left + (width*progress) + (sin(laser.getElapsedTime()*4+i*0.5)*width*0.05);

                laser.laserManager.drawCircle(ofPoint(xpos, top+height*0.3), 30, ofColor::white, laser.getRenderProfile());
                ofColor c;
                c.setHsb(hue*255, 255, 255);

                laser.laserManager.drawCircle(ofPoint(xpos, top+height*0.7), 30, c, laser.getRenderProfile());

            }

            break;

        }

        case 5: {

            // LASER PARTICLES
            int numParticles = 12;

            for(int i = 0; i<numParticles; i++) {

                float progress =(float)i/(float)(numParticles-1);

                float xpos =left + (width*progress) ;

                laser.laserManager.drawDot(ofPoint(xpos, top+height*0.3), ofColor(255),1, laser.getRenderProfile());
                ofColor c;
                c.setHsb(progress*255, 255, 255);
                laser.laserManager.drawDot(ofPoint(xpos, top+height*0.7), c, 1,  laser.getRenderProfile());

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
                float spread = ofMap(cos(laser.getElapsedTime()*0.4),1,-1,0.01,0.1);
                p.x = sin((laser.getElapsedTime()-((float)i*spread)) *1.83f * speed) * 300;
                p.y = sin((laser.getElapsedTime()-((float)i*spread)) *2.71f *speed) * 300;
                p.x+=laser.getConfig().laserWidth/2;
                p.y+=laser.getConfig().laserHeight/2;

                laser.laserManager.drawDot(p, c, 1, laser.getRenderProfile());

            }

            break;

        }
        case 7: {

            // 3D rotation
            // you don't need to wrap your draw calls in
            // laser.beginDraw() and laser.endDraw() unless
            // you're doing 3D (it fixes the viewport perspective)

            laser.laserManager.beginDraw();

            float speed = 20;
            ofPushMatrix();
            ofTranslate(laser.getConfig().laserWidth/2,laser.getConfig().laserHeight/2);
            ofRotateYDeg(laser.getElapsedTime()*speed);
            int hue = (int)(laser.getElapsedTime()*32)%255; // 8 seconds to go around
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
                laser.laserManager.drawPoly(poly, c, laser.getRenderProfile());

                ofPopMatrix();
            }
            ofPopMatrix();

            laser.laserManager.endDraw();


            break;

        }case 8: {

            // 3D rotation
            // you don't need to wrap your draw calls in
            // laser.beginDraw() and laser.endDraw() unless
            // you're doing 3D (it fixes the viewport perspective)
            laser.laserManager.drawCircle(laser.getConfig().laserWidth/2,laser.getConfig().laserHeight/2,laser.getConfig().laserWidth/2,ofColor::blue);
            break;

        }case 9: {

            laser.laserManager.drawLine(glm::vec3(0,laser.getConfig().laserHeight/2,0),glm::vec3(laser.getConfig().laserWidth,laser.getConfig().laserHeight/2,0),ofColor::blue);
            break;

        }
    }
}





//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){

}
