#include "easylogging++.h"
#include "json.hpp"
#include "snake.h"

using nlohmann::json;

// Postionen ges av int position => endast ett värde på mappen

// ---------------------- NEXT MOOVE  -------------------------------

std::string Snake::get_next_move(json map) {
  int mapHight = map["height"] ;
  int mapWidth = map["width"] ;

  //int response = 1;
  std::string responsArray [] = {"UP", "DOWN", "RIGHT", "LEFT"};

   std::array<double,4> responsValue;

  // Reset responsValue
  responsValue[0] = 0; // UP
  responsValue[1] = 0; // DOWN
  responsValue[2] = 0; // RIGHT
  responsValue[3] = 0; // LEFT


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

  for (int i = 0; i < map["obstaclePositions"].size(); ++i) // OBSTACLE
  {
    int i_x, i_y;
    std::tie(i_x,i_y) = pos2xy(map["obstaclePositions"][i], map["width"]);
    int dist_x = std::max(std::min(i_x - snake_x, 5), -5);
    int dist_y = std::max(std::min(i_y - snake_y, 5), -5);

    if (dist_x < 0) {// DOWN
    responsValue[1] += curveObstacle[abs(dist_x)-1]; 
    responsValue[2] += curveObstacle[abs(dist_x)-1] * 0.5; 
    responsValue[3] += curveObstacle[abs(dist_x)-1] * 0.5; 

    } 
    else {// UP
      responsValue[0] += curveObstacle[dist_x-1];
      responsValue[2] += curveObstacle[dist_x-1] * 0.5;
      responsValue[3] += curveObstacle[dist_x-1] * 0.5; 
      } 

    if (dist_y < 0) { // LEFT
    responsValue[3] += curveObstacle[abs(dist_y)-1];
    responsValue[0] += curveObstacle[abs(dist_y)-1] * 0.5;
    responsValue[1] += curveObstacle[abs(dist_y)-1] * 0.5;
     } 
    else { // RIGHT
    responsValue[2] += curveObstacle[dist_y-1];
    responsValue[0] += curveObstacle[dist_y-1] * 0.5;
    responsValue[1] += curveObstacle[dist_y-1] * 0.5; 
    }

  }

  for (int i = 0; i < map["foodPositions"].size(); ++i) // FOOD 
  {
    int i_x, i_y;
    std::tie(i_x,i_y) = pos2xy(map["foodPositions"][i], map["width"]);
    int dist_x = std::max(std::min(i_x - snake_x, 5), -5);
    int dist_y = std::max(std::min(i_y - snake_y, 5), -5);

    if (dist_x < 0) {responsValue[1] += curveFood[abs(dist_x)-1]; } // DOWN
    else {responsValue[0] += curveFood[dist_x-1]; } // UP

    if (dist_y < 0){responsValue[3] += curveFood[abs(dist_y)-1]; } // LEFT
    else {responsValue[2] += curveFood[dist_y-1]; } // RIGHT
  }

  for (int j = 0; j < map["snakeInfos"].size(); ++j) // SNAKES (including yourself)
  {
    for (int i = 0; i < map["snakeInfos"][j]["positions"].size(); ++i)
    {
      int i_x, i_y;
      std::tie(i_x,i_y) = pos2xy(map["snakeInfos"][j]["positions"][i], map["width"]);
      int dist_x = std::max(std::min(i_x - snake_x, 5), -5);
      int dist_y = std::max(std::min(i_y - snake_y, 5), -5);

      if (dist_x < 0) {
        responsValue[1] += curvePlayers[abs(dist_x)-1]; } // DOWN
      else if (dist_x > 0) {
        responsValue[0] += curvePlayers[dist_x-1]; } // UP

      if (dist_y < 0){
        responsValue[3] += curvePlayers[abs(dist_y)-1]; } // LEFT
      else if (dist_y > 0){
        responsValue[2] += curvePlayers[dist_y-1]; } // RIGHT
    }
  }


  // WALLS 

  if(snake_x > (mapWidth - 4) ){  // RIGHT
    LOG(INFO) << "1 - " << curveWall[mapWidth - snake_x]; 
    responsValue[2] += curveWall[mapWidth - snake_x]; 
    LOG(INFO) << "2 - " << responsValue[2]; 
    responsValue[0] += curveWall[mapWidth - snake_x] * 0.5;
    responsValue[1] += curveWall[mapWidth - snake_x] * 0.5;
    }
  else if(snake_x > 4) {  // LEFT
    responsValue[3] += curveWall[snake_x]; 
    responsValue[0] += curveWall[snake_x] * 0.5; 
    responsValue[1] += curveWall[snake_x] * 0.5; 

    }

  if(snake_y > (mapHight - 4) ){  // DOWN
    responsValue[1] += curveWall[mapHight - snake_y]; 
    responsValue[2] += curveWall[mapHight - snake_y] * 0.5; 
    responsValue[3] += curveWall[mapHight - snake_y] * 0.5; 

    }
  else if(snake_y > 4 ){ // UP
    responsValue[0] += curveWall[snake_y]; 
    responsValue[2] += curveWall[snake_y] * 0.5;
    responsValue[3] += curveWall[snake_y] * 0.5;
    }

  //LOG(INFO) << "Last u" << responsValue[0] << " d" << responsValue[1] << " r" << responsValue[2]<<" l" << responsValue[3];

    // if last move was up you cant go down next move if lastMove = 0
    /*
    0 - up
    1 - down
    2- right
    3- left
    */

    // Avoid your last move 
    if(lastMove == 0){
      avoidedMove = 1;
    }
    else if(lastMove == 1){
      avoidedMove = 0;
    }
    else if(lastMove == 2){
      avoidedMove = 3;
    }
    else{
      avoidedMove = 2;
    }

    // Calculate what direction is the best 
    respMaxSlot = 0;
    int maxValue = -1000; 
    for(int i = 1; i < responsValue.size(); i++)
    {
      if(responsValue[i] >  maxValue ){//&& i != lastMove){
          if(i != avoidedMove){
            respMaxSlot = i;
            maxValue = responsValue[i];
          }

      }
    }
    
    lastMove = respMaxSlot;

  LOG(INFO) << "Snake is making move " << responsArray[respMaxSlot] << " at worldtick: " << map["worldTick"];
  return responsArray[respMaxSlot];

};
// ---------------------- OUR FUNCTIONS -------------------------------
void Snake::initializeCurves(){
  curveFood[0]= 10; 
  curveFood[1]= 5; 
  curveFood[2]= 2.5; 
  curveFood[3]= 1.25; 
  curveFood[4]= 0; 

  curveWall[0]= -30; 
  curveWall[1]= -20; 
  curveWall[2]= -10; 
  curveWall[3]= -5; 
  curveWall[4]= 0; 

  curvePlayers[0]= -10; 
  curvePlayers[1]= -5; 
  curvePlayers[2]= -2.5; 
  curvePlayers[3]= -1.25; 
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
  lastMove = 0; 
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

