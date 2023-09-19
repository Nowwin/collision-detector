#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include <vector>
#include <random>
#include <ctime>

#include "libraries/math.hpp"

//A simple cricle struct
struct Circle
{
    float radius;
    float mass;
    Vector2f position;
    Vector2f velocity;
    Vector2f acceleration;
    
    // Default constructor
    Circle() : radius(1.0f), mass(1.0f), position(Vector2f()), velocity(Vector2f()), acceleration(Vector2f()) {}

    // Constructor with radius and mass
    Circle(float r, float m) : radius(r), mass(m), position(Vector2f()), velocity(Vector2f()), acceleration(Vector2f()) {}

    // Constructor with radius, mass and position
    Circle(float r, float m, Vector2f pos) : radius(r), mass(m), position(pos), velocity(Vector2f()), acceleration(Vector2f()) {}

    // Constructor with all attributes
    Circle(float r, float m, Vector2f pos, Vector2f vel, Vector2f acc) : radius(r), mass(m), position(pos), velocity(vel), acceleration(acc) {}

    bool checkBoundaryConditions(int x, int y, Vector2f &newPos) {
        return newPos.x < 0 || newPos.y < 0 || newPos.x > x || newPos.y > y;      
    }

    void move(int x, int y) {
        Vector2f newPos = position + velocity;
        if (checkBoundaryConditions(x, y, newPos))
        {
            /* if (newPos.x < 0 || newPos.x > x )
            {
                velocity.x = -x;
            } else
            {
                velocity.y = -y;
            } */
            
            velocity = -velocity;
            
        } else
        {
            position = newPos;
        }
        
        
    }

    void moveTime(float t) {
        position = position + velocity.scalar(t);
    }

    void applyImpulse(Vector2f j) {
        velocity = velocity + j.scalar(1/mass);
    }
};



bool isCircleColliding(Circle* c1, Circle *c2) {
    Vector2f distance = c1->position - c2->position;

    return (c1->radius + c2->radius)*(c1->radius + c2->radius) >= distance.distanceSquared();
}

void resolveCollision(Circle* c1, Circle* c2) {
    Vector2f normal = c2->position - c1->position;
    normal.Normalize();

    //Assuming elastic collision
    float e = 1;

    Vector2f relativeVelocity = c2->velocity - c1->velocity;
    float impulseMagnitude = -((1 + e) * Dot(relativeVelocity, normal))/(1/c1->mass + 1/c2->mass);
    Vector2f impulse = normal.scalar(impulseMagnitude);

    c1->applyImpulse(impulse.scalar(-1.0f));
    c2->applyImpulse(impulse);
}


// Function to generate a random float between min and max (inclusive)
float getRandomFloat(float min, float max) {
    static std::mt19937 gen(time(0)); // Seed the random number generator with the current time
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}

// Function to generate a vector of random circles
std::vector<Circle> generateRandomCircles(int numCircles, float xMin, float xMax, float yMin, float yMax, float velMin, float velMax) {
    std::vector<Circle> circles;
    for (int i = 0; i < numCircles; ++i) {
        Circle circle;

        // Random position within the specified range
        circle.position.x = getRandomFloat(xMin, xMax);
        circle.position.y = getRandomFloat(yMin, yMax);

        // Random velocity within the specified range
        circle.velocity.x = getRandomFloat(velMin, velMax);
        circle.velocity.y = getRandomFloat(velMin, velMax);

        // For simplicity, let's set the radius and mass to fixed values for all circles
        circle.radius = 4.0f;
        circle.mass = 1.0f;

        circles.push_back(circle);
    }
    return circles;
}

#endif
