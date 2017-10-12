#include "easylogging++.h"
#include "json.hpp"
#include "snake.h"

using nlohmann::json;

// Postionen ges av int position => endast ett värde på mappen

// ---------------------- NEXT MOOVE  -------------------------------

std::string Snake::get_next_move(json map) {

  int response = 1;
  std::string responsArray [] = {"UP", "DOWN", "RIGHT", "LEFT"};
  
  // Reset responsValue
  responsValue[0] = 0; // UP
  responsValue[1] = 0; // DOWN
  responsValue[2] = 0; // RIGHT
  responsValue[3] = 0; // LEFT


  int mySnakeX, mySnakeY;
  LOG(INFO) << "innan pos " << map["snakeInfos"].size();
  LOG(INFO) << "innan pos2 " << map["snakeInfos"][mySnakeSlot]["positions"][0];
  //std::tie(mySnakeX,mySnakeY) = pos2xy(map["snakeInfos"][mySnakeSlot]["positions"][0], map["width"]);
  tempPos2xy(map["snakeInfos"][mySnakeSlot]["positions"][0], map["width"], mySnakeX, mySnakeY);
  LOG(INFO) << "efter pos ";

  calculateRespons(map, 0);
  calculateRespons(map, 1);
  calculateRespons(map, 2);
  calculateRespons(map, 3);

  // Hitta vår snake första gången vi spelar 
  if(mySnakeSlot == -1){
  mySnakeSlot = 0;
      while(map["snakeInfos"][mySnakeSlot]["name"] != name){
        mySnakeSlot++;
      } 
  }

  // Calculate what direction is the best
  int respMaxSlot = 0;
  for(int i = 1; i < responsValue.size(); i++)
  {
    if(responsValue[i-1] < responsValue[i]){
        respMaxSlot = i;
    }
  }
    
  LOG(INFO) << "Snake is making move " << responsArray[respMaxSlot] << " at worldtick: " << map["worldTick"];

  return responsArray[respMaxSlot];

};
// ---------------------- OUR FUNCTIONS -------------------------------
// From int pos to x,y coords
/*std::tuple<int, int> Snake::pos2xy(const int position, const int map_width) {
  float pos = position;
  float width = map_width;
  
  LOG(INFO) << "här  ";
  int myY = floor(pos / width);
  LOG(INFO) << "y " << myY; 
  int myX = fabs(pos - myY * width);
  LOG(INFO) << "x " << myX; 

  return std::make_tuple(myX, myY);
}*/

void Snake::tempPos2xy(const int position, const int map_width, int &resX, int &resY){
    LOG(INFO) << "här här  ";

  float pos = position;
  float width = map_width;
  
  LOG(INFO) << "här  ";
  int myY = floor(pos / width);
  LOG(INFO) << "y " << myY; 
  int myX = fabs(pos - myY * width);
  LOG(INFO) << "x " << myX; 
}

void Snake::calculateRespons(json map, int direction){ // Här ska allt som påverkar förföyttningen ske 
  double currentRespValue = 0; 
  

  //LOG(INFO) << "Snake pos: " << x << ", " << y ; 
  // Vi vill att vår snake ska hålla sig inom 2 < y/x > 30 

  /*if(direction = 3){ // we are on upp 
    if()
  }*/




  if(direction = 3) // just for testing, allways go left
  {
    currentRespValue = 1; 
  }
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
  LOG(INFO) << "initializeCurves finnished";

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

