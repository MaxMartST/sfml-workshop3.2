#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace std;
using namespace sf;

constexpr int WINDOW_WIDTH{800}, WINDOW_HEIGHT{600};
constexpr int POSITION_LEFT_EYE_X{300}, POSITION_LEFT_EYE_Y{300}, POSITION_RIGHT_EYE_X{500}, POSITION_RIGHT_EYE_Y{300};

struct Eye
{
    ConvexShape whiteEye;
    ConvexShape pupil;
    Vector2f position;
    float rotation = -90;
    float angle = 0;
};

void updatePupilElements(Eye &eye);
void drawEllips(ConvexShape &shape, const Vector2f &radius);
void initEye(Eye &eye, int positionX, int positionY);
void onMouseMove(const Event::MouseMoveEvent &event, Vector2f &mousePosition);
void pollEvents(RenderWindow &window, Vector2f &mousePosition);
float toDegrees(float radians);
void updateArrowElements(Eye &eye);
void update(const Vector2f &mousePosition, Eye &eye);
void redrawFrame(RenderWindow &window, Eye &eye);

void drawEllips(ConvexShape &shape, const Vector2f &radius)
{
    constexpr int pointCount = 200;
    shape.setPointCount(pointCount);
    for (int pointNo = 0; pointNo < pointCount; ++pointNo)
    {
        float angle = float(2 * M_PI * pointNo) / float(pointCount);
        Vector2f point = {
            radius.x * std::sin(angle),
            radius.y * std::cos(angle)};
        shape.setPoint(pointNo, point);
    }
}
void initEye(Eye &eye, int positionX, int positionY)
{
    eye.position.x = positionX;
    eye.position.y = positionY;
    const Vector2f eyeRaius = {100.f, 50.f};
    const Vector2f pupilRadius = {20.f, 10.f};
    eye.whiteEye.setRotation(eye.rotation);
    eye.pupil.setRotation(eye.rotation);
    eye.whiteEye.setPosition(eye.position);
    eye.pupil.setPosition(eye.position);

    eye.whiteEye.setFillColor(sf::Color(0xFF, 0xFF, 0xFF));
    eye.pupil.setFillColor(sf::Color(0x00, 0x00, 0x00));
    drawEllips(eye.whiteEye, eyeRaius);
    drawEllips(eye.pupil, pupilRadius);

    updatePupilElements(eye);
}

void updatePupilElements(Eye &eye)
{
    Vector2f trackRadius = {20.f, 40.f};
    eye.pupil.setPosition({eye.position.x + (trackRadius.x * std::cos(eye.angle)), eye.position.y + (trackRadius.y * std::sin(eye.angle))});
}

Vector2f toEuclidean(float radius, float angle)
{
    return {
        radius * std::cos(angle),
        radius * std::sin(angle)};
}

float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}

void updateArrowElements(Eye &eye)
{
    const Vector2f eyeOffset = toEuclidean(40, eye.rotation);
    eye.pupil.setPosition(eye.position + eyeOffset);
}

void update(const Vector2f &mousePosition, Eye &eye)
{
    const Vector2f delta = mousePosition - eye.position;
    eye.rotation = atan2(delta.y, delta.x);
    updateArrowElements(eye);
}

void onMouseMove(const Event::MouseMoveEvent &event, Vector2f &mousePosition)
{
    //cout << "mouse x=" << event.x << ", y=" << event.y << std::endl;
    mousePosition = {float(event.x), float(event.y)};
}

void pollEvents(RenderWindow &window, Vector2f &mousePosition)
{
    Event event;
    while (window.pollEvent(event))
    {
        switch (event.type)
        {
        case Event::Closed:
            window.close();
            break;
        case Event::MouseMoved:
            onMouseMove(event.mouseMove, mousePosition);
            break;
        default:
            break;
        }
    }
}

void redrawFrame(RenderWindow &window, Eye &eye)
{
    window.setFramerateLimit(120);

    window.draw(eye.whiteEye);
    window.draw(eye.pupil);
}

int main()
{
    ContextSettings settings;
    settings.antialiasingLevel = 8;
    RenderWindow window(VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "sfml3.2", Style::Default, settings);

    Eye eyeLeft;
    Eye eyeRight;
    Vector2f mousePosition;

    initEye(eyeLeft, POSITION_LEFT_EYE_X, POSITION_LEFT_EYE_Y);
    initEye(eyeRight, POSITION_RIGHT_EYE_X, POSITION_RIGHT_EYE_Y);

    while (window.isOpen())
    {
        pollEvents(window, mousePosition);
        window.clear(Color::Black);
        update(mousePosition, eyeLeft);
        update(mousePosition, eyeRight);
        redrawFrame(window, eyeLeft);
        redrawFrame(window, eyeRight);
        window.display();
    }
}