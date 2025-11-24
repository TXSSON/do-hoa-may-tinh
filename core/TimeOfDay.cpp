#include "TimeOfDay.h"
#include <algorithm>
#include <cmath>

TimeOfDay::TimeOfDay(float startTime)
    : time(startTime), speed(0.02f), paused(false)
{
}

void TimeOfDay::update(float deltaTime)
{
    if (!paused)
    {
        time += speed * deltaTime;
        // Wrap around
        if (time > 1.0f)
            time -= 1.0f;
        if (time < 0.0f)
            time += 1.0f;
    }
}

glm::vec3 TimeOfDay::getSkyColor() const
{
    // Time keyframes:
    // 0.0 = midnight (dark blue)
    // 0.25 = dawn (orange/pink)
    // 0.5 = noon (bright blue)
    // 0.75 = dusk (orange)
    // 1.0 = midnight again
    
    glm::vec3 nightColor(0.05f, 0.05f, 0.15f);      // Dark blue
    glm::vec3 dawnColor(0.8f, 0.4f, 0.3f);          // Orange/pink
    glm::vec3 dayColor(0.53f, 0.81f, 0.92f);        // Bright blue
    glm::vec3 duskColor(0.9f, 0.5f, 0.2f);          // Orange
    
    return interpolateColor(time, nightColor, dawnColor, dayColor, duskColor);
}

glm::vec3 TimeOfDay::getSunDirection() const
{
    // Sun rotates from east to west
    // At noon (0.5), sun is overhead (0, 1, 0)
    // At midnight (0.0, 1.0), sun is below horizon
    
    float angle = (time - 0.5f) * 3.14159f * 2.0f; // Radians
    float x = std::sin(angle);
    float y = std::cos(angle);
    
    return glm::normalize(glm::vec3(x, y, 0.2f));
}

float TimeOfDay::getAmbientStrength() const
{
    // Stronger ambient during day, weaker at night
    if (time >= 0.25f && time <= 0.75f)
    {
        // Day time - bright
        return 0.6f;
    }
    else
    {
        // Night time - dim
        return 0.2f;
    }
}

bool TimeOfDay::isNightTime() const
{
    // Night between 0.0-0.2 and 0.8-1.0
    return (time < 0.2f || time > 0.8f);
}

void TimeOfDay::setTime(float t)
{
    time = std::max(0.0f, std::min(1.0f, t));
}

void TimeOfDay::togglePause()
{
    paused = !paused;
}

glm::vec3 TimeOfDay::interpolateColor(float t, const glm::vec3& c0, const glm::vec3& c1, 
                                       const glm::vec3& c2, const glm::vec3& c3) const
{
    // Interpolate between 4 colors based on time
    // 0.0-0.25: c0 -> c1 (night -> dawn)
    // 0.25-0.5: c1 -> c2 (dawn -> day)
    // 0.5-0.75: c2 -> c3 (day -> dusk)
    // 0.75-1.0: c3 -> c0 (dusk -> night)
    
    if (t < 0.25f)
    {
        float blend = t / 0.25f;
        return glm::mix(c0, c1, blend);
    }
    else if (t < 0.5f)
    {
        float blend = (t - 0.25f) / 0.25f;
        return glm::mix(c1, c2, blend);
    }
    else if (t < 0.75f)
    {
        float blend = (t - 0.5f) / 0.25f;
        return glm::mix(c2, c3, blend);
    }
    else
    {
        float blend = (t - 0.75f) / 0.25f;
        return glm::mix(c3, c0, blend);
    }
}
