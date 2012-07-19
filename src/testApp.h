#pragma once

#include "ofMain.h"
#include "ofxGameCamera.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
    ofVec3f surfaceNormal(ofVec3f v1, ofVec3f v2, ofVec3f v3);
    
    vector<float> phases;
    
    ofxGameCamera cam;
    ofMesh mesh;
    
    ofLight light;
};
