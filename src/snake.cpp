#include "easylogging++.h"
#include "json.hpp"
#include "snake.h"

using nlohmann::json;

// Good runs:   http://game.snake.cygni.se/#/viewgame/88a759cf-e406-4941-9eba-28d51ed9c600
//              http://game.snake.cygni.se/#/viewgame/af42c3a4-fbf9-4a14-b892-3fdf6da063a4
//              http://game.snake.cygni.se/#/viewgame/ed59e861-8147-4845-8f6c-3b586b2b91a4
//              http://game.snake.cygni.se/#/viewgame/9645084f-3969-4c84-88ca-35be7cbd8216 (distant 4)
//              http://game.snake.cygni.se/#/viewgame/820f1c8b-77f3-4734-93ad-d5d9f5e424b2 (my good run)

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

  double danger[] = {0.0, 0.0, 0.0, 0.0};
  double food[] = {0.0, 0.0, 0.0, 0.0};
  int view_distance = 2;

  for (int i = 0; i < map["obstaclePositions"].size(); ++i) // OBSTACLE
  {
    int i_x, i_y;
    std::tie(i_x,i_y) = pos2xy(map["obstaclePositions"][i], map["width"]);
    int dist_x = i_x - snake_x;
    int dist_y = i_y - snake_y;

    if (dist_y > -view_distance && dist_y < 0 && abs(dist_x) < view_distance) // danger up
    {
      danger[0] += (view_distance + dist_y)*(view_distance + dist_y);
    }
    else if (dist_y > 0 && dist_y < view_distance  && abs(dist_x) < view_distance) //  danger down
    {
      danger[1] += (view_distance - dist_y)*(view_distance - dist_y);
    }
    
    if (dist_x > -view_distance && dist_x < 0  && abs(dist_y) < view_distance)
    {
      danger[3] += (view_distance + dist_x)*(view_distance + dist_x);
    }
    else if (dist_x > 0 && dist_x < view_distance  && abs(dist_y) < view_distance)
    {
      danger[2] += (view_distance - dist_y)*(view_distance - dist_y);
    }

  }

  for (int i = 0; i < map["foodPositions"].size(); ++i) // FOOD 
  {
    int i_x, i_y;
    std::tie(i_x,i_y) = pos2xy(map["foodPositions"][i], map["width"]);
    int dist_x = i_x - snake_x;
    int dist_y = i_y - snake_y;

    if (dist_y > -view_distance && dist_y < 0 && abs(dist_x) < view_distance) 
    {
      food[0] += (view_distance + dist_y)*(view_distance + dist_y);
    }
    else if (dist_y > 0 && dist_y < view_distance  && abs(dist_x) < view_distance) 
    {
      food[1] += (view_distance - dist_y)*(view_distance - dist_y);
    }
    
    if (dist_x > -view_distance && dist_x < 0  && abs(dist_y) < view_distance)
    {
      food[3] += (view_distance + dist_x)*(view_distance + dist_x);
    }
    else if (dist_x > 0 && dist_x < view_distance  && abs(dist_y) < view_distance)
    {
      food[2] += (view_distance - dist_y)*(view_distance - dist_y);
    }
  }

  for (int j = 0; j < map["snakeInfos"].size(); ++j) // SNAKES (including yourself)
  {
    for (int i = 0; i < map["snakeInfos"][j]["positions"].size(); ++i)
    {
      int i_x, i_y;
      std::tie(i_x,i_y) = pos2xy(map["snakeInfos"][j]["positions"][i], map["width"]);
      int dist_x = i_x - snake_x;
      int dist_y = i_y - snake_y;

      if (dist_y > -view_distance && dist_y < 0  && abs(dist_x) < view_distance) // danger up
      {
        danger[0] += (view_distance + dist_y)*(view_distance + dist_y)*(view_distance + dist_y);
      }
      else if (dist_y > 0 && dist_y < view_distance  && abs(dist_x) < view_distance) //  danger down
      {
        danger[1] += (view_distance - dist_y)*(view_distance - dist_y)*(view_distance - dist_y);
      }
      
      if (dist_x > -view_distance && dist_x < 0  && abs(dist_y) < view_distance)
      {
        danger[3] += (view_distance + dist_x)*(view_distance + dist_x)*(view_distance + dist_x);
      }
      else if (dist_x > 0 && dist_x < view_distance && abs(dist_y) < view_distance)
      {
        danger[2] += (view_distance - dist_y)*(view_distance - dist_y)*(view_distance - dist_y);
      }
    }
  }


  // WALLS 
  if (snake_y < view_distance)
  {
    danger[0] += 3*(view_distance - snake_y)*(view_distance - snake_y);
  }
  if (mapHight - snake_y < view_distance)
  {
    danger[1] += 3*(view_distance - mapHight + snake_y)*(view_distance - mapHight + snake_y);
  }
  if (snake_x < view_distance)
  {
    danger[3] += 3*(view_distance - snake_x)*(view_distance - snake_x);
  }
  if (mapWidth - snake_x < view_distance)
  {
    danger[2] += 3*(view_distance - mapWidth + snake_x)*(view_distance - mapWidth + snake_x);
  }


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
  danger[avoidedMove] += 1000;
  // Is not dangerous and food close, is not dagerous or food close 
  int best_choice = 0;
  double choice_value = 1000;
  for (int i = 0; i < 4; ++i)
  {
    LOG(INFO) << "dir: " << i << " d: " << danger[i] << " f: " << food[i];
    if (danger[i] - food[i] < choice_value) // is good choice and better then the chosen one
    {
      best_choice = i;
      choice_value = danger[i] - food[i];
    }
  }
  
  lastMove = best_choice;

  LOG(INFO) << "Snake is making move " << responsArray[best_choice] << " at worldtick: " << map["worldTick"];
  return responsArray[best_choice];

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

  curveObstacle[0]= -30; 
  curveObstacle[1]= -20; 
  curveObstacle[2]= -10; 
  curveObstacle[3]= -5; 
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

