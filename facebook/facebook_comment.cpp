#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

class Comment{
    private:
        string id;
        string parentId; // To handle nested comments //
        string userId;
        string postId;
        string description;
        vector<Comment> comments;
        string getUniqueId(){
           static int id = 1;
           return 'a' + to_string(id++);
    }
    
    public:

       Comment(string description){
           this->description = description;
           this->id = getUniqueId();
       }

        string getId()
        {
            return this->id;
        }

        string  getparentId()
        {
            return this->parentId;
        }

        void setParentId(string parentId)
        {
            this->parentId = parentId;
        }

        string getUserId()
        {
            return this->userId;
        }

        void setUserId(string userId)
        {
            this->userId = userId;
        }

        string getPostId()
        {
            return this->postId;
        }

        void setPostId(string postId)
        {
            this->postId = postId;
        }

        vector<Comment> getComments()
        {
            return this->comments;
        }

        void setDescription(string desciption){
            this->description = description;
        }

        string getDescription(){
            return this->description;
        }

        void setNestedDescription(string commentId, string description){
            for(Comment& comment: comments){
                if(comment.getId() == commentId){
                    comment.setDescription(description);
                    return;
                }
            }
        }

         void setNestedDelete(string commentId){
             int i = 0;

            for(Comment& comment: comments){
                if(comment.getId() == commentId){
                    comments.erase(comments.begin() + i);
                    return;
                }
                i++;
            }
        }

        void addComment(Comment& comment){
            comments.push_back(comment);
        }

};

class Post{
    private:
        string id;
        vector<Comment>comments;
        string getUniqueId(){
            static int id =1;
            return 'b' + to_string(id++);
        }
    public:
        Post(){
            this->id= getUniqueId();
        };

        string getPostId(){
            return this->id;
        }

        vector<Comment> getComments(){
            return this->comments;
        }

        void addComment(Comment& comment){
            comments.push_back(comment);
        };

        void addNestedComment(string commentId, Comment& nestedComment){
            for(Comment &comment: comments){
               if(comment.getId() == commentId){
                   comment.addComment(nestedComment);
                   return;
               }
            }
        };

        void editComment(string parentId, string commentId, string description){

            for(Comment& comment : comments){
                if(comment.getId() == parentId){

                    if(parentId == commentId){
                        comment.setDescription(description);
                    }
                    else{
                        comment.setNestedDescription(commentId, description);
                    }
                    break;
                }
            }

        };
       
       void deleteComment(string parentId, string commentId){

          int i=0,  index = -1;

            for(Comment& comment : comments){
                if(comment.getId() == parentId){

                    if(parentId == commentId){
                        index= i;
                        break;
                    }
                    else{
                        comment.setNestedDelete(commentId);
                    }
                    break;
                }
                i++;
            }

            if(index != -1){
                comments.erase(comments.begin()+index);
            }

        };

};

class User{                    // Exposes API for outside world interaction // 
    private:
    string id;
    string name;

    string getUniqueId(){
        static int id = 1;
        return 'c' + to_string(id++);
    }

    public:
    User(string name){
        this->id = getUniqueId();
        this->name = name;
       }

    string getId()
       {
        return this->id;
       }

    string getName()
       {
        return this->name;
       }

    void addCommentToPost(Post& post, Comment& comment){
        comment.setUserId(id);
        comment.setPostId(post.getPostId());
        comment.setParentId(comment.getId());
        post.addComment(comment);
    }

    void replyToComment(Post& post, string parentId, Comment& comment){
        comment.setUserId(id);
        comment.setPostId(post.getPostId());
        comment.setParentId(parentId);
        post.addNestedComment(parentId, comment);
    }

    bool editComment(Post& post, string parentId, string commentId, string description){

        for(Comment& comment: post.getComments()){
            if(comment.getId() == commentId ){
                if(comment.getUserId() != id){
                    cout<<"Other user try to edit the comment \n";
                    return false;
                }
                   break;
            }
        }
        post.editComment(parentId, commentId, description);
        return true;
       }

    bool deleteComment(Post& post, string parentId, string commentId){

        for(Comment& comment: post.getComments()){

            if(comment.getId() == commentId ){
                if(comment.getUserId() != id){
                    cout<<"Other user try to delete the comment \n";
                    return false;
                }
                   break;
               }
           }
        post.deleteComment(parentId, commentId);
        return true;
       }
};

int main()
{
    User u1("Shivam");
    Comment c1("First comment");
    Comment c2("Second comment");
    Comment c11("First nested comment");
    Comment c22("Second nested comment");
    Comment c12("First second nested comment");
    Comment c13("First third nested comment");

    Post p;
    u1.addCommentToPost(p, c1);
    u1.addCommentToPost(p, c2);

    u1.replyToComment(p, c1.getId(), c11);
    u1.replyToComment(p, c1.getId(), c12);
    u1.replyToComment(p, c1.getId(), c13);
    u1.replyToComment(p, c2.getId(), c22);

    for(Comment par_c: p.getComments()){
        cout<<par_c.getDescription().c_str()<<"\n";
        for( Comment nest_c : par_c.getComments()){
            cout<<"\t"<< nest_c.getDescription().c_str() << "\n";
        }
    }

   return 0;
}