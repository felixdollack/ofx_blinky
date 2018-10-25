//
//  ofx_blinky.h
//  EOGcalibration
//
//  Created by Felix Dollack on 10.10.18.
//

#ifndef ofx_blinky_h
#define ofx_blinky_h

#include "ofMain.h"

class Blinky {
public:
    Blinky(float radius);
    Blinky(float radius, ofColor mainColor);
    void update(void);
    void draw(void);
    void draw(int x, int y);
    void setPosition(ofVec2f position);
    ofVec2f getPosition(void);
    void setSize(float radius);
    void setColor(ofColor color);
    void setColors(ofColor foreground_color, ofColor background_color);
    void setBlinking(bool state);
    bool isBlinking();
    void setBlinkyOn(bool state);
    bool isBlinkyOn(void);

private:
    bool _blinking_state, _blinky_on_state;
    float _radius, _blinking_interval_seconds;
    ofVec2f _position;
    ofColor _blinky_highlight_color, _blinky_base_color;
    float _last_blinky_update, _blinking_start_time;

    static ofColor getDarkerColor(ofColor color);
};

#endif /* ofx_blinky_h */
