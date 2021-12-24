#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>
#include <string>
#include <memory>

class GraphNode; // forward declaration
class ChatLogic; // forward declaration

class ChatBot
{
private:
    // data handles (owned)
    wxBitmap *_image; // avatar image

    // data handles (not owned)
    GraphNode *_currentNode;
    GraphNode *_rootNode;
    ChatLogic *_chatLogic;

    // proprietary functions
    int ComputeLevenshteinDistance(std::string s1, std::string s2);

public:
    // constructors / destructors
    ChatBot();                     // constructor WITHOUT memory allocation
    ChatBot(std::string filename); // constructor WITH memory allocation
    ~ChatBot();

    //// STUDENT CODE
    ////
    
      /*The Rule of Five is especially important in resource management, where unnecessary copying needs to be avoided due to limited resources and performance reasons.
    The Rule of Five states that if you have to write one of the functions listed below then you should consider implementing all of them with a proper resource management policy in place.
    *The destructor: Responsible for freeing the resource once the object it belongs to goes out of scope.
    *The assignment operator: The default assignment operation performs a member-wise shallow copy, which does not copy the content behind the resource handle. If a deep copy is needed, it has be implemented by the programmer.
    *The copy constructor: As with the assignment operator, the default copy constructor performs a shallow copy of the data members. If something else is needed, the programmer has to implement it accordingly.
    *The move constructor: Because copying objects can be an expensive operation which involves creating, copying and destroying temporary objects, rvalue references are used to bind to an rvalue. Using this mechanism, the move constructor transfers the ownership of a resource from a (temporary) rvalue object to a permanent lvalue object.
    *The move assignment operator: With this operator, ownership of a resource can be transferred from one object to another. The internal behavior is very similar to the move constructor.
    */
    // Since a deconstructor is implemented, here are the other methods needed to comply with the rule of five:

    // COPY ASSIGNMENT OPERARTOR
    ChatBot &operator=(const ChatBot &source);

    // COPY CONSTRUCTOR 
    ChatBot(const ChatBot &source);

    // MOVE CONSTRUCTOR 
    ChatBot(ChatBot &&source);

    // MOVE ASSIGNMENT OPERARTOR
    ChatBot &operator=(ChatBot &&source);

    ////
    //// EOF STUDENT CODE

    // getters / setters
    void SetCurrentNode(GraphNode *node);
    void SetRootNode(GraphNode *rootNode) { _rootNode = rootNode; }
    void SetChatLogicHandle(ChatLogic *chatLogic) { _chatLogic = chatLogic; }
    ChatLogic* GetChatLogicHandle() { return _chatLogic; }
    wxBitmap *GetImageHandle() { return _image; }

    // communication
    void ReceiveMessageFromUser(std::string message);
};

#endif /* CHATBOT_H_ */
