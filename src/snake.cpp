#include "easylogging++.h"
#include "json.hpp"
#include "snake.h"

using nlohmann::json;

// Postionen ges av int position => endast ett värde på mappen

// ---------------------- NEXT MOOVE  -------------------------------

std::string Snake::get_next_move(json map) {

  int response = 1;
  std::string responsArray [] = {"UP", "DOWN", "RIGHT", "LEFT"};
  
  // Reset responsValues
  responsValue[0] = 0.4; // UP
  responsValue[1] = 0.4; // DOWN
  responsValue[2] = 0.4; // RIGHT
  responsValue[3] = 0.4; // LEFT

  calculateRespons(0);
  calculateRespons(1);
  calculateRespons(2);
  calculateRespons(3);

  // Hitta vår snake första gången vi spelar 
  if(mySnakeSlot == -1){
  mySnakeSlot = 0;
      while(map["snakeInfos"][mySnakeSlot]["name"] != name){
        mySnakeSlot++;
      } 
  }

  int x, y;
  std::tie(x,y) = pos2xy(map["snakeInfos"][mySnakeSlot]["positions"][0], map["width"]);

  //LOG(INFO) << "Snake pos: " << x << ", " << y ; 

  LOG(INFO) << "Snake is making move " << responsArray[response] << " at worldtick: " << map["worldTick"];

  return responsArray[response];

};
// ---------------------- OUR FUNCTIONS -------------------------------

void Snake::calculateRespons(int direction){
double currentRespValue = 0; 


responsValue[direction] = currentRespValue;
}


void Snake::initializeCurves(){
  curveFood[0]= 0; 

  curveFood[1]= 0; 
  curveFood[2]= 0; 
  curveFood[3]= 0; 
  curveFood[4]= 0; 

  curveWall[0]= 0; 
  curveWall[1]= 0; 
  curveWall[2]= 0; 
  curveWall[3]= 0; 
  curveWall[4]= 0; 

  curvePlayers[0]= 0; 
  curvePlayers[1]= 0; 
  curvePlayers[2]= 0; 
  curvePlayers[3]= 0; 
  curvePlayers[4]= 0; 

  curveTail[0]= 0; 
  curveTail[1]= 0; 
  curveTail[2]= 0; 
  curveTail[3]= 0; 
  curveTail[4]= 0; 

  curveObstacle[0]= 0; 
  curveObstacle[1]= 0; 
  curveObstacle[2]= 0; 
  curveObstacle[3]= 0; 
  curveObstacle[4]= 0; 

}

// From int pos to x,y coords
std::tuple<int, int> Snake::pos2xy(const int position, const int map_width) {
  float pos = position;
  float width = map_width;

  int y = floor(pos / width);
  int x = fabs(pos - y * width);

  return std::make_tuple(x, y);
}



 // ---------------------- THERE FUNCTIONS -------------------------------
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
  mySnakeSlot = -1;
  LOG(INFO) << "Game is starting";
  // Leta upp din snake ? 
  initializeCurves();
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

