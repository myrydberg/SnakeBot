#pragma once

#include "json.hpp"

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
};
