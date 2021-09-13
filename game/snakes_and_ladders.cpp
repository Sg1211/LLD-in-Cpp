#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Snake{
    private:
      int start;
      int end;
    
    public:
      Snake(int start, int end){
          this->start = start;
          this->end = end;
      }
         int getStart() {
          return this->start;
      }

      int getEnd(){
          return this->end;
      }
};

class Ladder{
    private:
      int start;
      int end;
    
    public:
      Ladder(int start, int end){
          this->start = start;
          this->end = end;
      }

      int getStart() {
          return this->start;
      }

      int getEnd(){
          return this->end;
      }
};

class Player{
    private:
        int id;
        string name;
        int currentPosition;

   

      int getUniqueId(){
          static int id =1;
          return id++;
      }

      public:
        Player(string name){
             this->id = getUniqueId();
             this->name = name;
             this->currentPosition = 0;
         }



        void setName(string name)
        {
            this->name = name;
        }

        string getName(){
            return this->name;
        }

        int getId(){
            return this->id;
        }


        int getCurrentPosition()
        {
            return this->currentPosition;
        }


        void setCurrentPosition(int currentPosition)
        {
            this->currentPosition = currentPosition;
        }

};

class Game{
  private:
      vector<Player> players;
      int currentTurn;
      Player *winner;
      unordered_map<int,int>snakesAndLadders;
      void nextPlayer(){
          currentTurn = (currentTurn + 1) % players.size();
      }

    public:
      Game(vector<Snake>& snakes, vector<Ladder>& ladders,  vector<Player>& players){
          this->players = players;
          for(Snake snake: snakes){
              snakesAndLadders[snake.getStart()] = snake.getEnd();
          }

          for(Ladder ladder: ladders){
               snakesAndLadders[ladder.getStart()] = ladder.getEnd();
          }

          this->currentTurn = 0;
          this->winner= nullptr;
      }
      vector<Player> getPlayers()
      {
          return this->players;
      }

      Player *getWinner()
      {
          return this->winner;
      }

      bool roll(Player& player, int diceValue){
        if(winner!=nullptr || diceValue >6 || diceValue <1 || players[currentTurn].getId()!=player.getId()){
        return false;
        }

        int destination = player.getCurrentPosition()+diceValue;
        if(destination < 100){
           if(snakesAndLadders.find(destination)!=snakesAndLadders.end()){
               player.setCurrentPosition(snakesAndLadders[destination]);
           }
           else{
               player.setCurrentPosition(destination);
           }
        }
        if(destination == 100){
            winner= &player;
            player.setCurrentPosition(destination);
        }

        nextPlayer();
        return true;

      }

};


int main()
{
    Player p1("Shivam");
    Player p2("Tushar");
    Player p3("Rahul");

    Snake s1(17,7);
    Snake s2(54,34);
    Snake s3(62,19);
    Snake s4(64,60);
    Snake s5(87,36);
    Snake s6(92,73);
    Snake s7(95,75);
    Snake s8(98,79);

    Ladder l1(1,38);
    Ladder l2(4,14);
    Ladder l3(9,31);
    Ladder l4(21,42);
    Ladder l5(28,84);
    Ladder l6(51,67);
    Ladder l7(72,91);
    Ladder l8(80,99);

    vector<Snake>snakes = {s1,s2,s3,s4, s5, s6, s7, s8};
    vector<Ladder>ladders = {l1,l2,l3,l4, l5, l6, l7, l8};
    vector<Player>players = {p1, p2, p3};

    Game game(snakes, ladders, players);

    while(game.getWinner() ==  nullptr)
    {
       int diceVal = (rand()%6)+1;
       game.roll(p1, diceVal);
       diceVal = (rand()%6)+1;
       game.roll(p2, diceVal);
       diceVal = (rand()%6)+1;
       game.roll(p3, diceVal);
    }

    cout<<"the winner is"<<" "<<game.getWinner()->getName()<<endl;
    cout<<"winner position is"<<" "<< game.getWinner()->getCurrentPosition()<<endl;
   return 0;
}