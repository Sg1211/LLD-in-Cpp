#include <iostream>
#include <vector>

using namespace std;

class Task;       // Forward Declaration //
class User;       
class Sprint;

enum TaskType{
    STORY, 
    FEATURE,
    BUG
};

enum TaskStatus{
    OPEN,
    IN_PROGRESS,
    IN_QA,
    DONE
};

class Sprint{

    private:

    int begin, end;
    string name;
    vector<Task> tasks;

    public:
    
    Sprint(int begin, int end, string name){
        this->begin = begin;
        this->end = end;
        this->name= name;
    }

    void addTask(Task& task){
       tasks.push_back(task);
    }

    void eraseTask(int taskNumber){
        tasks.erase(tasks.begin() + taskNumber);
    }

    void printDetails(){
        cout<< "Sprint_name is" << " " << name << " " << "and sprint_begins" << " " << begin << " " << "and sprint_ends" << " " << end << "\n";
    }

	vector<Task> getTask() {
		return this->tasks;
	}

    inline bool operator == (Sprint sprint){
     return 
     (
       (this->begin == sprint.begin) && 
       (this->end == sprint.end) && 
       (this->name == sprint.name)
     );
    }
};

class Task{

  private:
    int id;
    string subtract;
    User *user;
    TaskType taskType;
    TaskStatus taskStatus;

    int getUniqueId(){
        static int id = 1;
        return id++;
    }

  public: 
 
    Task(){
     id = getUniqueId();
     taskStatus = TaskStatus::OPEN;
    }

    int getId()
    {
        return this->id;
    }

    void setSubtract(string subtract)
    {
        this->subtract = subtract;
    }

    void setUser(User *user)
    {
        this->user = user;
    }
    void setTaskType(TaskType taskType)
    {
        this->taskType = taskType;
    }
    void setTaskStatus(TaskStatus taskStatus)
    {
        this->taskStatus = taskStatus;
    }

};

class User{

    private:
      vector<Task>taskList;
      vector<Sprint>sprintList;

    public:

    Task createTask(TaskType taskType){
        if(taskType == TaskType::STORY){
            cout << "Warning task type story is created without subtract" << endl;
        }

        Task task;
        task.setTaskType(taskType);
        task.setUser(this);
        taskList.push_back(task);
        return task;
    }

    Task createTask(string subtract){
        Task task;
        task.setTaskType(TaskType::STORY);
        task.setSubtract(subtract);
        task.setUser(this);
        taskList.push_back(task);
        return task;
    }

    Sprint createSprint(int begin, int end, string name){
     Sprint sprint(begin, end, name);
     sprintList.push_back(sprint);
     return sprint;
    };

    bool addTaskToSprint(Sprint& sprint, Task& task){
      for(Sprint spr : sprintList){
          if(spr == sprint){
              for( Task t: taskList){
                  if(t.getId() == task.getId()){
                     sprint.addTask(task);
                     return true;
                  }
              }
          }
      }
      return false;

    };


    bool removeTaskFromSprint(Sprint& sprint, Task& task){
        vector<Task> tasks;
        for(Sprint spr: sprintList){
          if(spr == sprint){
            tasks = sprint.getTask();
            break;
          }
        }

        int i = 0;
        for(Task t : tasks){
            if(t.getId() == task.getId()){
                sprint.eraseTask(i);
                return true;
            }
            i++;
        }
        return false;

    }
    void printAllTasks(){
        for( Task t : taskList){
            cout<< t.getId() << " ";
        }
        cout<<endl;
    };


    bool changeStatus(Task& task, TaskStatus taskStatus){

      for( Task &t : taskList){

         if(t.getId() == task.getId()){
             t.setTaskStatus(taskStatus);
             return true;
         }
      }
      return false;
    };
     
};



int main()
{
   User user1, user2;
   Task task1 = user1.createTask(TaskType::STORY);
   Task task2 = user2.createTask(TaskType::BUG);
   Task task3 = user1.createTask("this is a subtract");

   Sprint sprint1 = user1.createSprint(1, 30 , "first-sprint");
   Sprint sprint2 = user2.createSprint(2, 31, "second-sprint");

   cout<< user1.changeStatus(task1, TaskStatus::IN_QA) << endl;  // 1
   cout<< user1.addTaskToSprint(sprint1, task1) << endl;    // 1
   cout<< user2.addTaskToSprint(sprint2, task2) << endl;    // 1
   cout<< user1.addTaskToSprint(sprint2, task1) << endl;    // 0

   cout<< user1.removeTaskFromSprint(sprint1, task3)<< endl; // 0
   cout<< user2.addTaskToSprint(sprint2, task1)<< endl; // 0

   sprint1.printDetails();
   cout<<endl;
   user1.printAllTasks();
   user2.printAllTasks();

   return 0;
}