// Copyright 2016 Zachary Krausman and Daniel MacMillan
#include <boost/lexical_cast.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include "space.hpp"
namespace space {

Universe::Universe(double total_time, double change_time,
                   std::string file_name,
                   double win_x, double win_y) : total_time_(total_time),
    change_time_(change_time), file_name_(file_name), win_x_(win_x),
    win_y_(win_y) {
    if (!(std::cin >> number_planets_)) {
        throw std::runtime_error("Failed to load number_planets_ from file");
        exit(1);
    }
    if (!(std::cin >> solar_system_size_)) {
        throw std::runtime_error("Failed to load solar_system_size_ from file");
        exit(1);
    }
    for (int i = 0; i < number_planets_; i++) {
        std::string temp_fname;
        double un_x_pos, un_y_pos, un_x_vel, un_y_vel, mass_temp;
        std::cin >> un_x_pos >> un_y_pos >> un_x_vel >> un_y_vel >> mass_temp >>
                 temp_fname;
        Body temp(0, 0, un_x_vel, un_y_vel, mass_temp, 0, 0, un_x_pos, un_y_pos,
                  temp_fname);
        planets_.push_back(temp);
        // std::cout << temp_fname << std::endl;
    }
    unsigned int seed = time(NULL);
    for (int i = 0; i < rand_r(&seed) % ((static_cast<int>(win_x + win_y)) / 20)
     + 50; i++) {
        Star temp(rand_r(&seed) % 800, rand_r(&seed) % 800, 0, 
	rand_r(&seed) % 4);
        stars_.push_back(temp);
    }
    end_sim_ = 0;
    elapsed_time_ = 0;
    if (!font_.loadFromFile("arial.ttf")) {
        std::cout << "no font found" << std::endl;
    }

    text_.setFont(font_);
    text_.setString("Time: " + boost::lexical_cast<std::string>(elapsed_time_));
    text_.setCharacterSize(24);
    text_.setStyle(sf::Text::Bold);
    text_.setColor(sf::Color::White);
    text_.move(50, 50);
}

void Universe::calcForces() {
  std::vector<Body>::iterator bodies_itr_1;
  std::vector<Body>::iterator bodies_itr_2;

  for (bodies_itr_1 = planets_.begin(); bodies_itr_1 != planets_.end();
    ++bodies_itr_1) {
    for (bodies_itr_2 = bodies_itr_1; ++bodies_itr_2 != planets_.end();) {
      // Calculate distance between the two bodies, along with x/y components
      double delta_x = bodies_itr_1->getUx() - bodies_itr_2->getUx();
      double delta_y = bodies_itr_1->getUy() - bodies_itr_2->getUy();
      double distance_apart_ = std::sqrt(std::pow(delta_x, 2) +
      std::pow(delta_y, 2));
      // The force acting upon BOTH bodies
      double force = ((6.67e-11) * (bodies_itr_1->getMass()) *
                  (bodies_itr_2->getMass())) / (std::pow(distance_apart_, 2));
      double force_x = (force) * (delta_x / distance_apart_);
      double force_y = (force) * (delta_y / distance_apart_);
      // multiply by mass to get acceleration for ONE body, add to velocity
      double old_ux = bodies_itr_1->getUx_Velocity();
      bodies_itr_1->setUx_Velocity(old_ux + (force_x /
        bodies_itr_1->getMass()));
      double old_uy = bodies_itr_1->getUy_Velocity();
      bodies_itr_1->setUy_Velocity(old_uy + (force_y /
        bodies_itr_1->getMass()));
      // repeat for other body
      old_ux = bodies_itr_2->getUx_Velocity();
      bodies_itr_2->setUx_Velocity(old_ux + (force_x /
        bodies_itr_2->getMass()));
      old_uy = bodies_itr_2->getUy_Velocity();
      bodies_itr_2->setUy_Velocity(old_uy + (force_y /
        bodies_itr_2->getMass()));
    }
  }
}

void Universe::translateCoordinates() {
  for (int i = 0; i < number_planets_; i++) {
    planets_[i].setX(((planets_[i].getUx() / solar_system_size_)
    * win_x_ / 2) + win_x_ / 2);
    planets_[i].setY(((planets_[i].getUy() / solar_system_size_)
    * win_y_ / 2) + win_y_ / 2);
    planets_[i].setSpritePos(planets_[i].getX(), planets_[i].getY());
  }
}

void Universe::monitorTime() {
  text_.setString("Time: " + boost::lexical_cast<std::string>(elapsed_time_));
  if (elapsed_time_ >= total_time_) {
    // Rewrite to the txt file all the info now that it is updated
    std::ofstream file;
    file.open(file_name_.c_str(), std::ofstream::out | std::ofstream::trunc);
    file << number_planets_ << std::endl;
    file << solar_system_size_ << std::endl;
    std::vector<Body>::iterator bodies_itr = planets_.begin();
    for (bodies_itr; bodies_itr != planets_.end(); ++bodies_itr) {
      file << bodies_itr->getUx() << " "
      << bodies_itr->getUy() << " "
      << bodies_itr->getUx_Velocity() << " "
      << bodies_itr->getUy_Velocity() << " "
      << bodies_itr->getMass() << " "
      << bodies_itr->getGifName().c_str() << std::endl;
    }
    file.close();
    exit(0);
  }
}

void Universe::updateUniverse() {
  // calculate forces
  calcForces();

  // convert ux/uy_velocity to sfml size
  std::vector<Body>::iterator bodies_itr = planets_.begin();
  for (bodies_itr; bodies_itr != planets_.end(); ++bodies_itr) {
    // std::cout << "ux: " << bodies_itr->getUx_Velocity() << std::endl;
    // std::cout << "uy: " << bodies_itr->getUy_Velocity() << std::endl;
    // std::cout << "uxvel: " << bodies_itr->getUx_Velocity() << std::endl;
    // std::cout << "uyvel: " << bodies_itr->getUy_Velocity() << std::endl;
    bodies_itr->setX_Velocity(bodies_itr->getUx_Velocity() / 1000);
    bodies_itr->setY_Velocity(bodies_itr->getUy_Velocity() / 1000);
    bodies_itr->setUx(bodies_itr->getUx() + bodies_itr->getUx_Velocity());
    bodies_itr->setUy(bodies_itr->getUy() + bodies_itr->getUy_Velocity());
    bodies_itr->step(change_time_);
  }

  // Handle the time change - check if we need to end the program.
  elapsed_time_ = elapsed_time_ + change_time_;
  monitorTime();
}

double Universe::getTimeElapsed() const {
  return elapsed_time_;
}

std::vector<Body> Universe::getPlanets() const {
  return planets_;
}

std::istream& operator >> (std::istream& is, Universe& uni) {
  is >> uni.temp_;
  return is;
}

void Universe::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(text_);
  for (int i = 0; i < number_planets_; i++) {
    target.draw(planets_[i]);
  }
  for (int i = 0; i < stars_.size(); i++) {
    target.draw(stars_[i]);
  }
}

}  // namespace space