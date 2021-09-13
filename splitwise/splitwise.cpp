#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

class User;         // Forward Declaration
class Expense;
class Splitwise;


enum Split { 
    EQUAL,
    EXACT, 
    PERCENT
};


class User {
   private:
       int id;
       string name;
       vector<pair<User, double>> userExpenseSheet;
       double totalExpenseSoFar;

      int getUniqueId(){
          static int id = 1;
          return id++;
      }

    public:

       User(string name){
           this->name = name;
           this->id = getUniqueId();
           this->totalExpenseSoFar = 0;
           this->userExpenseSheet.clear();
       }

       int getId()     
       {
           return this->id;
       }

       string getName()   
       {
           return this->name;
       }

       vector<pair<User, double>> getUserExpenseSheet()
       {
           return this->userExpenseSheet;
       }

       double getTotalExpenseSoFar()
       {
           return this->totalExpenseSoFar;
       }

       void addToUserExpenseSheet(User& user, double expense){
           if( (*this).getId() == user.getId())
           return;

           this->totalExpenseSoFar += expense;

           for(pair<User,double> &newExpense: userExpenseSheet){

               if(newExpense.first.getId() == user.getId()){
                   newExpense.second += expense;
                   return;
               }
           }

           userExpenseSheet.push_back({user, expense});
           return;
       }

       void printTotalBalance(){
           if(totalExpenseSoFar > 0){
               cout<<getName()<<" "<<"owes"<<" "<<totalExpenseSoFar<<endl;
           }
           else{
               cout<<getName()<<" "<<"gets back total of"<<" "<<totalExpenseSoFar*(-1)<<endl;
           }

       }

    inline bool operator == (User user){
     return (this->id == user.getId());
    };

};

class Expense{

    private:
        int id;
        string decription;
        Split split;
        vector<double> percentageDistribution;
        vector<double> exactDistribution;
        User *creditor;
        vector<User> defaulters;
        double exactTotalAmount;

        int getUniqueId(){
            static int id = 1;
            return id++;
        }

    public: 

        Expense(User& user, Split split, vector<User>& defaulters , double expense){
            this->creditor = &user;
            this->split = split;
            this->defaulters= defaulters;
            this->exactTotalAmount=expense;
            this->id = getUniqueId();
        }

        int getId()
        {
            return this->id;
        }

        string getDecription()
        {
            return this->decription;
        }

        void setDecription(string decription)
        {
            this->decription = decription;
        }

        vector<double> getPercentageDistribution()
        {
            return this->percentageDistribution;
        }

        void setPercentageDistribution(vector<double> percentageDistribution)
        {
            this->percentageDistribution = percentageDistribution;
        }
        vector<double> getExactDistribution()
        {
            return this->exactDistribution;
        }
        void setExactDistribution(vector<double> exactDistribution)
        {
            this->exactDistribution = exactDistribution;
        }

        User* getCreditor()
        {
            return this->creditor;
        }

        vector<User> getDefaulters()
        {
            return this->defaulters;
        }

        double getExactTotalAmount()
        {
            return this->exactTotalAmount;
        }

        Split getSplit(){
            return this->split;
        }
};

class Splitwise{

  private:
    vector<User> users;
    unordered_set<int>userIds;

    vector<double>divideEqually(double totalAmt, int usersLen ){
        vector<double>parts;
        for(int i=0 ; i<usersLen; i++){
            double part =  totalAmt/usersLen;
            parts.push_back(part);
        }
        return parts;
    };

    bool calculateExpense(Expense& expense){
      User creditor = *expense.getCreditor();
      vector<User>defaulters = expense.getDefaulters();
      vector<double>amtPerHead;
      switch (expense.getSplit())
      {
        case Split::EQUAL:
            amtPerHead = divideEqually(expense.getExactTotalAmount(), defaulters.size());
            for(int i=0; i<defaulters.size();i++){

                auto itr = find(users.begin(), users.end(), creditor);
                if(itr != users.end()){
                    itr->addToUserExpenseSheet(defaulters[i], (-1) * amtPerHead[i]);
                }

                itr = find(users.begin(), users.end(), defaulters[i]);
                if(itr != users.end()){
                    itr->addToUserExpenseSheet(creditor, amtPerHead[i]);
                }
            }
        break;

      case Split::EXACT:
      amtPerHead = expense.getExactDistribution();
      for(int i=0; i<defaulters.size();i++){

                auto itr = find(users.begin(), users.end(), creditor);
                if(itr != users.end()){
                    itr->addToUserExpenseSheet(defaulters[i], (-1) * amtPerHead[i]);
                }

                itr = find(users.begin(), users.end(), defaulters[i]);
                if(itr != users.end()){
                    itr->addToUserExpenseSheet(creditor, amtPerHead[i]);
                }
            }
        break;

      case Split::PERCENT:
       amtPerHead = expense.getPercentageDistribution();
        for(int i=0; i<defaulters.size();i++){
                double amount = (amtPerHead[i] * expense.getExactTotalAmount())/ 100;
                auto itr = find(users.begin(), users.end(), creditor);
                if(itr != users.end()){
                    itr->addToUserExpenseSheet(defaulters[i], (-1) * amount);
                }

                itr = find(users.begin(), users.end(), defaulters[i]);
                if(itr != users.end()){
                    itr->addToUserExpenseSheet(creditor, amount);
                }
            }
        break;
 
      default:
          break;
      }
    };

    bool verifyUsers(User& user, vector<User> users){
        if(find(users.begin(), users.end(), user) == users.end()){
            users.push_back(user);
        }
        
        for(User usr : users){
            if(userIds.find(usr.getId()) == userIds.end()){
                return false;
            }
        }
        return true;
    };

  public:
      Splitwise(){};
      void registerUser(User& user){
        if( userIds.find(user.getId()) == userIds.end()){
          users.push_back(user);
          userIds.insert(user.getId());
        }
      }

      void addExpense(Expense& expense){
          if(verifyUsers(*expense.getCreditor(), expense.getDefaulters()) == false){
              cout<<"Cant process please register user and retry"<< endl;
              return;
          }

          calculateExpense(expense);
      };

      void printBalanceForAllUers(){
          for( User usr : users){
              usr.printTotalBalance();
          }
      };

      vector<User> getUsers(){
          return this->users;
      }

};


int main()
{
   string s1 = "Shivam";
   string s2 = "Tushar";
   string s3 = "Rahul";
   string s4 = "Akshay";

   User user1(s1);
   User user2(s2);
   User user3(s3);
   User user4(s4);

   vector<User>users;
   users.push_back(user1);
   users.push_back(user2);
   users.push_back(user3);
   users.push_back(user4);

   Splitwise sp;
   sp.registerUser(user1);
   sp.registerUser(user2);
   sp.registerUser(user3);
   sp.registerUser(user4);

   Expense expense1(user1, Split::EQUAL, users, 2000);
   sp.addExpense(expense1);

   sp.printBalanceForAllUers();

   vector<User>users2;
   users2.push_back(user2);
   users2.push_back(user3);

   Expense expense2(user1, Split::EXACT, users2, 1400);
   vector<double>db = {500, 900};
   expense2.setExactDistribution(db);
   sp.addExpense(expense2);

   sp.printBalanceForAllUers();

   vector<User>users3;
   users3.push_back(user3);
   users3.push_back(user4);

   Expense expense3(user1, Split::PERCENT, users3, 1000);
   vector<double>db2 = {60, 40};
   expense3.setPercentageDistribution(db2);
   sp.addExpense(expense3);

   sp.printBalanceForAllUers();

   for(User usr : sp.getUsers()){
       for(pair<User, double> p : usr.getUserExpenseSheet()){
          if(p.second > 0 ){
        cout<< usr.getName() << " " << "owes a total of"<<" "<<p.second<<" "<<"from"<<" "<<p.first.getName()<<endl;
          }
          else{
        cout<< usr.getName() << " " << "gets a total of"<<" "<<p.second * (-1)<<" "<<"from"<<" "<<p.first.getName()<<endl;
          }
       }
   }

 return 0;
}