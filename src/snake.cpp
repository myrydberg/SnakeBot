#include "easylogging++.h"
#include "json.hpp"
#include "snake.h"

using nlohmann::json;

std::string Snake::get_next_move(json map) {
  std::string response = "DOWN";

  if (!slotFound)
  {
    while(map["snakeInfos"][myInt]["name"] != name){
      myInt++;
    }
    slotFound = true;
  }

  int pos = map["snakeInfos"][myInt]["positions"][0];
  int width = map["width"];
  int y = floor(pos / width);
  int x = fabs(pos - y * width);

  //LOG(INFO) << "Snake pos: " << x << ", " << y ; 
  if (y > 30)
  {
    response = "RIGHT";
  }
  else if (y < 2)
  {
    response = "LEFT";
  }
  LOG(INFO) << "Snake is making move " << response << " at worldtick: " << map["worldTick"];
  return response;
};

void Snake::on_game_ended() {
  LOG(INFO) << "Game has ended";
};

void Snake::on_tournament_ended() {
  LOG(INFO) << "Tournament has ended";
};

void Snake::on_snake_dead(std::string death_reason) {
  LOG(INFO) << "Our snake has died, reason was: " << death_reason;
};

void Snake::on_game_starting() {
  myInt = 0;
  slotFound = false;
  LOG(INFO) << "Game is starting";
};

void Snake::on_player_registered() {
  LOG(INFO) << "Player was successfully registered";
};

void Snake::on_invalid_playername() {
  LOG(INFO) << "The player name is invalid, try another?";
};

void Snake::on_game_result(nlohmann::json playerRanks) {
  LOG(INFO) << "Game result:";
  nlohmann::json playerRank;
  el::Logger* defaultLogger = el::Loggers::getLogger("default");
  for (json::iterator it = playerRanks.begin(); it != playerRanks.end(); ++it) {
    playerRank = (nlohmann::json) *it;
    defaultLogger->info("%v.\t%v pts\t%v (%v)", playerRank["rank"], playerRank["points"],
            playerRank["playerName"], playerRank["alive"] ? "alive" : "dead");
  }
};
;
