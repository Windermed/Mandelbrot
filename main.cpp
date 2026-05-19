#include <iostream>     // std::cout
#include <complex>      // std::complex, std::abs
#include <SFML/Graphics.hpp>
#include "ComplexPlane.h"

// helpers i use across projects.
#define Message(input) std::cout << input << std::endl;
#define MessageNE(input) std::cout << input;
#define InputResponse(input) std::cin >> input;

int main()
{
    Font font;
    Text text;

    // we'll get half of our current desktop resolution so we aren't on full screen.
    int width = VideoMode::getDesktopMode().width / 2;
    int height = VideoMode::getDesktopMode().height / 2;

    // create the render window.
    RenderWindow window(VideoMode(width, height), "Mandelbrot");

    // then create the complexPlane object to handle mandelbrot rendering.
    ComplexPlane complexPlane(width, height);

    // load font. throw an error message in logs if not found.
    if (!font.loadFromFile("arial.ttf"))
    {
        Message("ERROR: Font is not valid")
        throw runtime_error("ERROR: FONT IS NOT VALID.");
        return -1;
    }

    // setup text.
    text.setFont(font);
    text.setCharacterSize(16);
    text.setFillColor(Color::White);
    text.setPosition(10.0f, 10.0f);

    // while our window is open
    while (window.isOpen())
    {
        // handle input.
        Event event;
        while (window.pollEvent(event))
        {

            if (event.type == Event::MouseButtonPressed)
            {

                // when left clicked, you can zoom in and recenter on the click location.
                if (event.mouseButton.button == Mouse::Left)
                {
                    //Message("LEFT CLICK WORKS!!!")
                    complexPlane.ZoomIn();
                    complexPlane.SetCenter({ event.mouseButton.x, event.mouseButton.y });
                }

                // when right click, you can zoom out and re center on click location
                if (event.mouseButton.button == Mouse::Right)
                {
                    //Message("RIGHT CLICKKK!!!")
                    complexPlane.ZoomOut();
                    complexPlane.SetCenter({ event.mouseButton.x, event.mouseButton.y });
                }
            }

            // when moving the mouse, it tracks the position so that it can then later update via HUD.
            if (event.type == Event::MouseMoved)
            {
                complexPlane.SetMouseLocation({ event.mouseMove.x, event.mouseMove.y });
                //Message("MOUSE MOVING")
            }
        }

        // when pressing escape, it will automatically shut down the program.
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        // recalculate pixel colors when State is calculating.
        complexPlane.UpdateRender();
        //Message("Render is complete!")

        // then update the HUD with the current center and mouse coords.
        complexPlane.LoadText(text);

        window.clear();

        // draw the mandelbrot fractal
        window.draw(complexPlane);

        // draws the HUD.
        window.draw(text);

        window.display();
    }
    return 0;
}