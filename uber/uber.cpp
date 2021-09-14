#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

enum RideStatus{
    IDLE, 
    CREATED,
    WITHDRAWN, 
    COMPLETED
};

class Ride{                // Ride class is encapsulated to Rider class //
    private:
        int id;
        int origin, destination;
        int seats;
        RideStatus rideStatus;

    public:
        static const int AMT_PER_KM = 20;
        Ride(){
            id = 0;
            origin = 0;
            destination = 0;
            seats = 0;
            rideStatus = RideStatus::IDLE;
        };

        int calculateFare(bool isPriorityRider){
            
            int dis = destination - origin;
            if( seats < 2){
                return dis * AMT_PER_KM * ( isPriorityRider ? 0.75 : 1);
            }

            return dis * seats * AMT_PER_KM * ( isPriorityRider ? 0.5 : 0.75);
        }

        int getId()
        {
            return this->id;
        }

        void setId(int id)
        {
            this->id = id;
        }
        
        int getOrigin(){
            return this->origin;
        }

        void setOrigin(int origin){
            this->origin = origin;
        }

        int getDestination()
        {
            return this->destination;
        }

         void setDes(int des){
            destination = des;
        }

        int getSeats()
        {
            return this->seats;
        }

        void setSeats(int seats){
            this->seats = seats;
        }

        RideStatus getRideStatus()
        {
            return this->rideStatus;
        }
        void setRideStatus(RideStatus rideStatus)
        {
            this->rideStatus = rideStatus;
        }
};

class Person{
    public:
     string name;

};

class Driver: private Person{          // All public member of Person are private to Driver //
    public:
      Driver(string name){
          this->name = name;
      }

};

class Rider: private Person{           // Used Inheritance //
     private:
       vector<Ride>completedRides;
       Ride currentRide;

     public:
      Rider(string name){
           this->name = name;
      }

      void createRide(int id, int origin, int des, int seats){
         currentRide.setId(id);
         currentRide.setOrigin(origin);
         currentRide.setDes(des);
         currentRide.setSeats(seats);
         currentRide.setRideStatus(RideStatus::CREATED);
      }

      void updateRide(int id, int origin, int des, int seats){

          if(currentRide.getRideStatus() == RideStatus::COMPLETED){
              cout<<"can't update, completed ride"<<endl;
              return;
          }

          if(currentRide.getRideStatus() == RideStatus::WITHDRAWN){
              cout<<"can't update, withdraw ride"<<endl;
              return;
          }
        createRide(id, origin, des, seats);
        return;
      }

      void withdrawRide(int id){
          if(currentRide.getId() != id){
              cout<<"can't withdraw, wrong ride \n";
              return;
          }

          if(currentRide.getRideStatus() != RideStatus::CREATED){
              cout<<"can't withdraw, ride in progress state \n";
              return;
          }

          currentRide.setRideStatus(RideStatus::WITHDRAWN);
          return;
      }

      int closeRide(){
           if(currentRide.getRideStatus() != RideStatus::CREATED){
              cout<<"can't close, ride was not in progress \n";
              return 0;
          }
          currentRide.setRideStatus(RideStatus::COMPLETED);
          completedRides.push_back(currentRide);
          return currentRide.calculateFare(completedRides.size()>=10);
      }
};


int main()
{
   Driver driver("Raju");
   Rider rider("Shivam");

   cout<<"1st test case"<<endl;
   rider.createRide(1, 10, 60, 1);
   cout<<rider.closeRide()<<endl;
   rider.updateRide(1, 10, 70, 1);
   cout<<rider.closeRide()<<endl;


   cout<<"2nd test case"<<endl;
   rider.createRide(1, 10, 60, 1);
   rider.withdrawRide(1);
   rider.updateRide(1, 10, 70, 1);
   cout<<rider.closeRide()<<endl;

   cout<<"3rd test case"<<endl;
   rider.createRide(1, 10, 60, 1);
   rider.updateRide(1, 10, 70, 1);
   cout<<rider.closeRide()<<endl;





   return 0;
}