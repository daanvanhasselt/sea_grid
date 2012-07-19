#include "testApp.h"

#define NUMROWS 20
#define NUMCOLS 20
#define SPACING 30

//--------------------------------------------------------------
void testApp::setup(){
    ofSetVerticalSync(true);
    
    ofEnableLighting();
    light.enable();
    light.setPointLight();
    light.setDiffuseColor(ofFloatColor(.6));
    light.setAmbientColor(ofFloatColor(.4));
    light.setPosition(SPACING * (NUMCOLS-1) / 2., 20, 0);
    ofSetSmoothLighting(false);
    
    // setup camera
    cam.setup();
    cam.targetNode.move(SPACING * (NUMCOLS-1) / 2., 40, SPACING * NUMROWS);
    cam.rotate(PI/2.0, 1.0, 0, 0);
    
    // setup mesh
    mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	for (int y = 0; y < NUMROWS; y++){
		for (int x = 0; x < NUMCOLS; x++){
			mesh.addVertex(ofPoint(x * SPACING, y * SPACING, 0));	// mesh index = x + y*width
            mesh.addNormal(ofVec3f(0,0,0));
            phases.push_back(ofRandom(TWO_PI));
		}
	}
	
    // setup mesh indices
	for (int y = 0; y < NUMROWS - 1; y++){
		for (int x = 0; x < NUMCOLS - 1; x++){
			mesh.addIndex(x + y * NUMCOLS);                 // 0
			mesh.addIndex((x + 1) + y * NUMCOLS);			// 1
			mesh.addIndex(x + (y + 1) * NUMCOLS);			// 10
			
			mesh.addIndex((x + 1) + y * NUMCOLS);			// 1
			mesh.addIndex((x + 1) + (y + 1) * NUMCOLS);		// 11
			mesh.addIndex(x + (y + 1) * NUMCOLS);			// 10
		}
	}
    
    ofBackground(255);
}

//--------------------------------------------------------------
void testApp::update(){
    for (int y = 0; y < NUMROWS - 1; y++){
		for (int x = 0; x < NUMCOLS - 1; x++){
            int index = y  * NUMCOLS  + x;
            ofVec3f v = mesh.getVertex(index);  // update vertex height
            v.z = sin(phases[index]) * 7.0;
            phases[index] += 0.02;
            mesh.setVertex(index, v);
            
            int index1 = y  * NUMCOLS  + x;
            int index2 = y * NUMCOLS + (x + 1);
            int index3 = (y - 1) * NUMCOLS + x;            
            mesh.setNormal(index, surfaceNormal(mesh.getVertex(index1), mesh.getVertex(index2), mesh.getVertex(index3)));      // update normal
		}
	}
}

//--------------------------------------------------------------
void testApp::draw(){
    glEnable(GL_DEPTH_TEST);
    cam.begin();            
        ofPushMatrix();
        ofRotateX(90);
        ofSetColor(255);
        mesh.drawFaces();       // draw faces
    
        // draw our normals, and show that they are perpendicular to the vector from the center to the vertex
//        vector<ofVec3f> n = mesh.getNormals();
//        vector<ofVec3f> v = mesh.getVertices();
//        float normalLength = 10.;
//        
//        ofSetColor(0);         
//        for(int i=0; i < n.size() ;i++){
//            ofLine(v[i].x,v[i].y,v[i].z,
//                   v[i].x+n[i].x*normalLength,v[i].y+n[i].y*normalLength,v[i].z+n[i].z*normalLength);
//        }
    
        ofSetColor(180);
        ofTranslate(0, 0, -1);
        mesh.drawWireframe();   // draw lines
        
        ofSetColor(40);
//        glPointSize(2);
//        glEnable(GL_POINT_SMOOTH);
//        mesh.drawVertices();     // draw points
    for (int i = 0; i < mesh.getNumVertices(); i++) {
        ofPushMatrix();
        ofTranslate(mesh.getVertex(i));
        ofSphere(.5);
        ofPopMatrix();
    }
            
        ofPopMatrix();
    
//    light.customDraw();
    cam.end();
    
//    ofSaveScreen("frames/"+ofToString(ofGetFrameNum(), 0)+".bmp");
}

//--------------------------------------------------------------
ofVec3f testApp::surfaceNormal(ofVec3f v1, ofVec3f v2, ofVec3f v3){		// return surface normal of triangle
	ofVec3f sideOne = v1 - v2;	// get side vectors
	ofVec3f sideTwo = v2 - v3;
	
	ofVec3f normal = sideOne.getCrossed(sideTwo);	// cross product
	normal = normal.normalized();					// normalize
	return normal;
}