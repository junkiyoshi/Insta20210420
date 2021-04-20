#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openframeworks");

	ofBackground(239);
	ofSetLineWidth(2);
	ofEnableDepthTest();

	this->line.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	this->vertices.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->face.clear();
	this->line.clear();
	this->vertices.clear();

	int len = 300;
	int width = 30;
	for (int y = -300; y <= 300; y += width){

		int start_param = ofRandom(1000) + ofGetFrameNum() * ofRandom(0.5, 1);
		int end_param = start_param + 35;

		for (int param = start_param; param <= end_param; param++) {

			vector<glm::vec3> vertices;
			vertices.push_back(glm::vec3(this->make_point(len, param), width *  0.5 + y));
			vertices.push_back(glm::vec3(this->make_point(len, param), width * -0.5 + y));

			this->face.addVertices(vertices);
			this->line.addVertices(vertices);

			if (param > start_param) {

				this->face.addIndex(this->face.getNumVertices() - 1); this->face.addIndex(this->face.getNumVertices() - 2); this->face.addIndex(this->face.getNumVertices() - 4);
				this->face.addIndex(this->face.getNumVertices() - 1); this->face.addIndex(this->face.getNumVertices() - 3); this->face.addIndex(this->face.getNumVertices() - 4);

				this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 3);
				this->line.addIndex(this->line.getNumVertices() - 2); this->line.addIndex(this->line.getNumVertices() - 4);
			}

			if (param % 25 == 0) {

				this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);
			}
		}

		this->line.addIndex(this->line.getNumVertices() - 1); this->line.addIndex(this->line.getNumVertices() - 2);
	}

	vector<ofColor> base_color_list;
	ofColor color;
	vector<int> hex_list = { 0xef476f, 0xffd166, 0x06d6a0, 0x118ab2, 0x073b4c };
	for (auto hex : hex_list) {

		color.setHex(hex);
		base_color_list.push_back(color);
	}

	int color_index = 0;
	for (int i = 0; i < 300; i++) {

		this->vertices.addVertex(glm::vec3(
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005), 0, 1, -150, 150),
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005), 0, 1, -150, 150),
			ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005), 0, 1, -300, 300)
		));

		this->vertices.addColor(base_color_list[color_index]);
		color_index = (color_index + 1) % base_color_list.size();
	}

	for (int i = 0; i < this->vertices.getNumVertices(); i++) {

		for (int k = i + 1; k < this->vertices.getNumVertices(); k++) {

			if (glm::distance(this->vertices.getVertex(i), this->vertices.getVertex(k)) < 50) {

				this->vertices.addIndex(i);
				this->vertices.addIndex(k);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateX(90);

	ofSetColor(39);
	this->face.draw();

	ofSetColor(239);
	this->line.drawWireframe();

	for (int i = 0; i < this->vertices.getNumVertices(); i++) {

		ofSetColor(this->vertices.getColor(i));
		ofDrawSphere(this->vertices.getVertex(i), 5);
	}

	this->vertices.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
glm::vec2 ofApp::make_point(int len, int param) {

	param = param % 100;
	if (param < 25) {

		return glm::vec2(ofMap(param, 0, 25, -len * 0.5, len * 0.5), -len * 0.5);
	}
	else if (param < 50) {

		return glm::vec2(len * 0.5, ofMap(param, 25, 50, -len * 0.5, len * 0.5));
	}
	else if (param < 75) {

		return glm::vec2(ofMap(param, 50, 75, len * 0.5, -len * 0.5), len * 0.5);
	}
	else {

		return glm::vec2(-len * 0.5, ofMap(param, 75, 100, len * 0.5, -len * 0.5));
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}