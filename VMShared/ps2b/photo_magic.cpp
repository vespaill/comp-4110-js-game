/* Copyright 2016 Zachary Krausman */
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include "lfsr_class.hpp"


int main(int argc, char* argv[]) {
    sf::Image image, image1;  ///< original of image, and copy to be edited
    if (!image.loadFromFile(argv[1]))
        return -1;
    image1 = image;

    sf::Color p;  ///< color that will be changed
    LFSR lfsr(argv[3], atoi(argv[4]));
    sf::Vector2u size = image.getSize();
    for (int x = 0; x < size.x; x++) {
        for (int y = 0; y < size.y; y++) {
            p = image1.getPixel(x, y);
            p.r = lfsr.generate(8) ^ p.r;
            p.g = lfsr.generate(8) ^ p.g;
            p.b = lfsr.generate(8) ^ p.b;
            image1.setPixel(x, y, p);
        }
    }
     ///< Window for original and encrypted image respectively
    sf::RenderWindow window1(sf::VideoMode(size.x, size.y), "Original");
    sf::RenderWindow window2(sf::VideoMode(size.x, size.y), "Encrypted");
    sf::Texture texture, texture1;  ///< Image that can be used for drawing
    texture.loadFromImage(image);  ///< Original image
    texture1.loadFromImage(image1);  ///< Encrypted image

    sf::Sprite sprite, sprite1;  ///< Sprites that have textures are assigned to
    sprite.setTexture(texture);  ///< Original image
    sprite1.setTexture(texture1);  ///< Encrypted image

    while (window1.isOpen() && window2.isOpen()) {  ///< Open/close windows
        sf::Event event;
        while (window1.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window1.close();
        }
        while (window2.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window2.close();
        }
        window1.clear();
        window1.draw(sprite);  ///< Drawing the original image
        window1.display();
        window2.clear();
        window2.draw(sprite1);  ///< Drawing the encrypted image
        window2.display();
    }
    if (!image1.saveToFile(argv[2]))
        return -1;

    return 0;
}