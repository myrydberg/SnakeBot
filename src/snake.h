#pragma once

#include "json.hpp"
#include <array>
#include <iostream>


class Snake
{
public:
  std::string name = "MyLitteBot";
  std::string get_next_move(nlohmann::json map);
  void on_game_ended();
  void on_tournament_ended();
  void on_snake_dead(std::string death_reason);
  void on_game_starting();
  void on_player_registered();
  void on_invalid_playername();
  void on_game_result(nlohmann::json playerRanks);
  void InitializeCurves();


  // Our curves 
	std::array<double,5> curveFood;
	std::array<double,5> curveWall;
	std::array<double,5> curvePlayers;
	std::array<double,5> curveTail;
	std::array<double,5> curveObstacle;
};





