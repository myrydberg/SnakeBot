#include "easylogging++.h"
#include "json.hpp"
#include "snake.h"

using nlohmann::json;

// Postionen ges av int position => endast ett värde på mappen

int a = 0; 
int mySnakeSlot = -1; 


std::string Snake::get_next_move(json map) {
  


  // HERE WE DO STUFF
  std::string response = "DOWN";
  std::string responsArray [] = {"UP", "DOWN", "RIGHT", "LEFT"};

  // Hitta vår snake första gången vi spelar 
  if(mySnakeSlot == -1){
  mySnakeSlot = 0;
      while(map["snakeInfos"][mySnakeSlot]["name"] != name){
        mySnakeSlot++;
      } 
  LOG(INFO) << "--> My snake name: " << map["snakeInfos"][mySnakeSlot]["name"];
  LOG(INFO) << "--> My snake slot: " << mySnakeSlot << std::endl; 
  }


  // 





 /* int pos = map["snakeInfos"][myInt]["positions"][0];
  int width = map["width"];
  int y = floor(pos / width);
  int x = fabs(pos - y * width);*/

/*if(a>3)
    a=0;
  else
    a++;*/

  
  // Hitta food 

  LOG(INFO) << "--> Number of foods " << map["foodPositions"].size() ;
  //LOG(INFO) << "--> Number of foods " << map["foodPositions"] ;

  LOG(INFO) << "Snake is making move " << responsArray[a] << " at worldtick: " << map["worldTick"];
  return responsArray[a];
};

void Snake::InitializeCurves(){
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


 // -----------------------------------------------------
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
  LOG(INFO) << "Game is starting";
  // Leta upp din snake ? 
  InitializeCurves();
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

