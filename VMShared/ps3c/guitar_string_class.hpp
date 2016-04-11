#ifndef GUITARSTRING_HPP_INCLUDED
#define GUITARSTRING_HPP_INCLUDED


/**
 * @file guitar_string_class.hpp
 * @copyright 2016
 * @author Zachary Krausman
 * @date 3/28/16
 * @version 1
 *
 * @brief Guitar String
 *
 */
#include <stdlib.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <algorithm>
#include <vector>
#include "ring_buffer_class.hpp"



/**
 * @brief namespace for the whole assignment
 */
namespace GuitarHero {

class GuitarString {
 public:
    /**
    * @brief constructor
    * @param takes a double
    */
    explicit GuitarString(double frequency);
    /**
    * @brief constructor
    * @param takes a vector of shorts
    */
    explicit GuitarString(const std::vector<sf::Int16> & init);
    /**
    * @brief randomizes values in the buffer
    */
    void pluck();
    /**
    * @brief Apply Karplus-Strong update
    */
    void tic();
    /**
    * @brief return the front of the buffer
    * @return the item at the front of the buffer
    */
    sf::Int16 sample();
    /**
    * @brief holds the number of times tic was called
    * @return the total number of times tic() was called on this instance
    */
    int time();

 private:
    ring_buffer<sf::Int16> rb;  ///< ring buffer
    int ticCount;  ///< amount of times tic has been called
};

}  // namespace GuitarHero


#endif  // GUITARSTRING_HPP_INCLUDED

