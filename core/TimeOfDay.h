#ifndef TIMEOFDAY_H
#define TIMEOFDAY_H

#include <glm/glm.hpp>

/**
 * Day/Night cycle manager
 * Controls time progression, sky colors, lighting
 */
class TimeOfDay
{
public:
    TimeOfDay(float startTime = 0.5f); // Start at noon

    void update(float deltaTime);
    
    // Visual properties
    glm::vec3 getSkyColor() const;
    glm::vec3 getSunDirection() const;
    float getAmbientStrength() const;
    bool isNightTime() const;
    
    // Controls
    void setTime(float t);
    void togglePause();
    void setSpeed(float s) { speed = s; }
    
    float getTime() const { return time; }

private:
    float time;       // 0.0-1.0 (0=midnight, 0.5=noon, 1.0=midnight)
    float speed;      // Time progression speed (default 0.02 = ~50s full cycle)
    bool paused;
    
    glm::vec3 interpolateColor(float t, const glm::vec3& c0, const glm::vec3& c1, const glm::vec3& c2, const glm::vec3& c3) const;
};

#endif
