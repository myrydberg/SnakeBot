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


  



  /*calculateRespons(map, 0);
  calculateRespons(map, 1);
  calculateRespons(map, 2);
  calculateRespons(map, 3);*/


  // Hitta vår snake första gången vi spelar 
  if(mySnakeSlot == -1){
  mySnakeSlot = 0;
      while(map["snakeInfos"][mySnakeSlot]["name"] != name){
        mySnakeSlot++;
      } 
  }

  int snake_x, snake_y;
  std::tie(snake_x,snake_y) = pos2xy(map["snakeInfos"][mySnakeSlot]["positions"][0], map["width"]);
  // LOG(INFO) << "Snake pos: " << x << ", " << y ; 
  // 

  for (int i = 0; i < map["obstaclePositions"].size(); ++i)
  {
    int i_x, i_y;
    std::tie(i_x,i_y) = pos2xy(map["obstaclePositions"][i], map["width"]);
    int dist_x = std::max(std::min(i_x - snake_x, 5), -5);
    int dist_y = std::max(std::min(i_y - snake_y, 5), -5);

    if (dist_x < 0)
    {
      responsValue[1] += curveObstacle[abs(dist_x)-1]; // DOWN
    }
    else
    {
      responsValue[0] += curveObstacle[dist_x-1]; // UP
    }

    if (dist_y < 0)
    {
      responsValue[3] += curveObstacle[abs(dist_y)-1]; // LEFT
    }
    else
    {
      responsValue[2] += curveObstacle[dist_y-1]; // RIGHT
    }

  }

  for (int i = 0; i < map["foodPositions"].size(); ++i)
  {
    int i_x, i_y;
    std::tie(i_x,i_y) = pos2xy(map["foodPositions"][i], map["width"]);
    int dist_x = std::max(std::min(i_x - snake_x, 5), -5);
    int dist_y = std::max(std::min(i_y - snake_y, 5), -5);

    if (dist_x < 0)
    {
      responsValue[1] += curveFood[abs(dist_x)-1]; // DOWN
    }
    else
    {
      responsValue[0] += curveFood[dist_x-1]; // UP
    }

    if (dist_y < 0)
    {
      responsValue[3] += curveFood[abs(dist_y)-1]; // LEFT
    }
    else
    {
      responsValue[2] += curveFood[dist_y-1]; // RIGHT
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
std::tuple<int, int> Snake::pos2xy(const int position, const int map_width) {
  float pos = position;
  float width = map_width;
  
  LOG(INFO) << "här  ";
  int myY = floor(pos / width);
  LOG(INFO) << "y " << myY; 
  int myX = fabs(pos - myY * width);
  LOG(INFO) << "x " << myX; 


  return std::make_tuple(myX, myY);
}


void Snake::initializeCurves(){
  curveFood[0]= 10; 
  curveFood[1]= 5; 
  curveFood[2]= 2.5; 
  curveFood[3]= 1.25; 
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

  curveObstacle[0]= -10; 
  curveObstacle[1]= -5; 
  curveObstacle[2]= -2.5; 
  curveObstacle[3]= -1.25; 
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

int Snake::xy2pos(const int x, const int y, const int map_width) {
  int res =  x + y * map_width;
  return res;
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

