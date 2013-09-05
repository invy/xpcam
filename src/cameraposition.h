#ifndef CAMERAPOSITION_H
#define CAMERAPOSITION_H


class CameraPosition {
public:
    CameraPosition()
        : x(0.0f), y(0.0f), z(0.0f), pitch(0.0f), heading(0.0f), roll(0.0f)
    {
    }

public:
    float x;
    float y;
    float z;
    float pitch;
    float heading;
    float roll;
};



#endif // CAMERAPOSITION_H
