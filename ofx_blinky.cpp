//
//  ofx_blinky.cpp
//  EOGcalibration
//
//  Created by Felix Dollack on 10.10.18.
//

#include "ofx_blinky.h"

Blinky::Blinky(float radius) : Blinky(radius, ofColor::red) {}
Blinky::Blinky(float radius, ofColor mainColor) : Blinky(radius, mainColor, BeepMode::BEEP_OFF, false, 0.5f){}
Blinky::Blinky(float radius, ofColor mainColor, ofColor secondaryColor) : Blinky(radius, mainColor, secondaryColor, BeepMode::BEEP_OFF, false, 0.5f){}
Blinky::Blinky(float radius, ofColor mainColor, ofColor secondaryColor, BeepMode beepMode, bool useHighBeep, float blinkingSpeedHz) : Blinky(radius, mainColor, beepMode, useHighBeep, blinkingSpeedHz) {
    setColors(mainColor, secondaryColor);
};
Blinky::Blinky(float radius, ofColor mainColor, BeepMode beepMode, bool useHighBeep, float blinkingSpeedHz) {
    this->_radius = radius;
    setColor(mainColor);
    setPosition(ofVec2f(-1, -1));
    this->_beep = NULL;
    setBeepHigh(useHighBeep);
    setBeepMode(beepMode);
    setBlinking(false);
    this->_last_blinky_update  = ofGetElapsedTimef();
    this->_blinking_start_time = 0;
    this->_blinking_interval_seconds = blinkingSpeedHz/2.0f; // Hz / (1 on + 1 off state)
}

void Blinky::update() {
    float time = ofGetElapsedTimef();

    // at the beginning of a new blinking cycle save the current time
    if ((this->_blinking_state == true) && (this->_blinking_start_time == 0)) {
        this->_blinking_start_time = time;
    }

    if (this->_blinking_state == true) {
        // while blinking see if we have to change between base and highlighted blinky version
        if ((time - this->_last_blinky_update) > this->_blinking_interval_seconds) {
            setBlinkyOn(!isBlinkyOn());
            this->_last_blinky_update = time;
        }
    } else {
        this->_blinking_start_time = 0;
    }
}

void Blinky::draw() {
    draw(this->_position.x, this->_position.y);
}

void Blinky::draw(int x, int y) {
    // draw base version
    ofSetColor(this->_blinky_base_color);
    ofDrawCircle(x, y, this->_radius);

    // draw highlighted version
    if (isBlinkyOn() == true) {
        ofSetColor(this->_blinky_highlight_color);
        ofDrawCircle(x, y, this->_radius);
    }
}

ofVec2f Blinky::getPosition() {
    return this->_position;
}

void Blinky::setPosition(ofVec2f position) {
    this->_position = position;
}

void Blinky::setSize(float radius) {
    this->_radius = radius;
}

void Blinky::setBlinking(bool state) {
    this->_blinking_state = state;
    if (this->_blinking_state == false) {
        setBlinkyOn(this->_blinking_state);
    }
}
bool Blinky::isBlinking() {
    return this->_blinking_state;
}

void Blinky::setColor(ofColor color) {
    this->_blinky_highlight_color = color;
    this->_blinky_base_color = getDarkerColor(this->_blinky_highlight_color);
}

void Blinky::setColors(ofColor foreground_color, ofColor background_color) {
    this->_blinky_highlight_color = foreground_color;
    this->_blinky_base_color = background_color;
}

void Blinky::setBlinkyOn(bool state) {
    if ((this->_blinky_on_state == false) && (this->_blinky_on_state != state)) {
        if (getBeepMode() > BeepMode::BEEP_OFF) {
            this->_beep->play();
        }
    }
    if ((this->_blinky_on_state == true) && (this->_blinky_on_state != state)) {
        if (getBeepMode() > BeepMode::BEEP_ON_START) {
            this->_beep->play();
        }
    }
    this->_blinky_on_state = state;
}
bool Blinky::isBlinkyOn() {
    return this->_blinky_on_state;
}

void Blinky::setBeepMode(BeepMode state) {
    this->_beep_mode = state;
    if (this->_beep_mode > BeepMode::BEEP_OFF) {
        if (this->_beep == NULL) {
            this->_beep = new ofSoundPlayer();
        }
        reloadBeep();
    } else {
        if (this->_beep != NULL) {
            delete this->_beep;
        }
        this->_beep = NULL;
    }
}
BeepMode Blinky::getBeepMode() {
    return this->_beep_mode;
}

void Blinky::setBeepHigh(bool state) {
    this->_use_high_beep = state;
    if (this->_use_high_beep == true) {
        this->_beep_filename = "beep_high.wav";
        reloadBeep();
    } else {
        this->_beep_filename = "beep_low.wav";
        reloadBeep();
    }
}

void Blinky::reloadBeep() {
    if (this->_beep != NULL) {
        if (this->_beep->isLoaded() == true) {
            this->_beep->unload();
        }
        if (this->_beep->isLoaded() == false) {
            this->_beep->load(this->_beep_filename);
        }
    }
}

ofColor Blinky::getDarkerColor(ofColor color) {
    ofColor desaturated = color;
    float hue, saturation, brightness;
    desaturated.getHsb(hue, saturation, brightness);
    desaturated.setHsb(hue, saturation, brightness/2);
    return desaturated;
}
