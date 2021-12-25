![ClassDiagram1](images/RunningOnLinux.PNG)

# Memory-Management-Chatbot
This is my solution for the 3rd project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The goal of this project is to optimize a chatbot-application with respect to memory management on the heap.
This project was accepted by Udacity's reviewer on 25th of December 2021 (see [here](https://review.udacity.com/#!/reviews/3341202)).

## Summary
The ChatBot code creates a dialogue where users can ask questions about some aspects of memory management in C++. After the knowledge base of the chatbot has been loaded from a text file, a knowledge graph representation is created in computer memory, where chatbot answers represent the graph nodes and user queries represent the graph edges. After a user query has been sent to the chatbot, the Levenshtein distance is used to identify the most probable answer.

## Dependencies
This section lists all dependencies that the application needs to be build and run.
* cmake >= 3.11
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* wxWidgets >= 3.0
  * Linux: `sudo apt-get install libwxgtk3.0-gtk3-dev libwxgtk3.0-gtk3-0v5`. If you are facing unmet dependency issues, refer to the [official page](https://wiki.codelite.org/pmwiki.php/Main/WxWidgets30Binaries#toc2) for installing the unmet dependencies.
  * Mac: There is a [homebrew installation available](https://formulae.brew.sh/formula/wxmac).
  * Installation instructions can be found [here](https://wiki.wxwidgets.org/Install). Some version numbers may need to be changed in instructions to install v3.0 or greater.

## Instructions to Build & Run
This section explains how to build & run the application.
1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./membot`.
A successful deployment on an Ubuntu-Linux System looks like displayed in the figure above.

## Architecture of the Application
The original cource code of the application can be found [here](https://github.com/udacity/CppND-Memory-Management-Chatbot). The architecture of the source code is described in the class diagrams below.
Diagram one shows _chatgui.h/.cpp_ - which is not a single class but a collection of classes that together realize the graphical user interface.

![ClassDiagram1](images/ClassDiagram1.PNG)

Diagram two describes principle references to the business logic of the application contained in _chatogic.h/.cpp_, _graphnode.h/.cpp_, _graphedge.h/.cpp_ and _chatbot.h/.cpp_.

![ClassDiagram2](images/ClassDiagram2.PNG)

## Optimizations made to the Original Source Code
The original cource code of the application can be found [here](https://github.com/udacity/CppND-Memory-Management-Chatbot). Several optimizations have been made to the source code including management of classes on the heap with smart pointers.
### Optimization 1
In file _chatgui.h/.cpp_ chatLogic is now an exclusive resource to class ChatbotPanelDialog. A _unique_ptr_ is used to realize the ownership here.
### Optimization 2
In file _chatbot.h/.cpp_ the class ChatBot now complies with the Rule of Five. It properly allocates / deallocates memory resources on the heap and also copies member data where it makes sense.
### Optimization 3
In file _chatlogic.h/ .cpp_, the instances of GraphNodes are now exclusively owned by the class ChatLogic.
### Optimization 4
In files _chatlogic.h/ .cpp_ and _graphnodes.h/ .cpp_ now each instance of GraphNode exclusively owns the outgoing GraphEdges and holds non-owning references to incoming GraphEdges.
### Optimization 5
In file _chatlogic.cpp_, a local ChatBot instance is now created on the stack at the bottom of function _LoadAnswerGraphFromFile_. Then, move semantics are used to pass the ChatBot instance into the root node; ChatLogic now has no ownership relation to the ChatBot instance and thus is no longer responsible for memory allocation and deallocation.

## Wrap up of the Rule of Five
In this section the Rule of Five is wrapped up in brief since it is one of the most important concepts that are applied when refactoring the original source code provided by Udacity.

The Rule of Five is especially important in resource management, where unnecessary copying needs to be avoided due to limited resources and performance reasons. The Rule of Five states that if you have to write one of the functions listed below (usually: When you need to write a destructor that frees resources again) then you should consider implementing all of them with a proper resource management policy in place.
* The destructor: Responsible for freeing the resource once the object it belongs to goes out of scope.
* The assignment operator: The default assignment operation performs a member-wise shallow copy, which does not copy the content behind the resource handle. If a deep copy is needed, it has be implemented by the programmer.
* The copy constructor: As with the assignment operator, the default copy constructor performs a shallow copy of the data members. If something else is needed, the programmer has to implement it accordingly.
* The move constructor: Because copying objects can be an expensive operation which involves creating, copying and destroying temporary objects, rvalue references are used to bind to an rvalue. Using this mechanism, the move constructor transfers the ownership of a resource from a (temporary) rvalue object to a permanent lvalue object.
* The move assignment operator: With this operator, ownership of a resource can be transferred from one object to another. The internal behavior is very similar to the move constructor.

An example of a class that complies with the rule of five is the following "moveable class":

```cpp
#include <stdlib.h>
#include <iostream>

class MyMovableClass
{
private:
    int _size;
    int *_data;

public:
    MyMovableClass(size_t size) // constructor
    {
        _size = size;
        _data = new int[_size];
        std::cout << "CREATING instance of MyMovableClass at " << this << " allocated with size = " << _size*sizeof(int)  << " bytes" << std::endl;
    }
    MyMovableClass(MyMovableClass &&source) // 4 : move constructor
    {
        std::cout << "MOVING (c’tor) instance " << &source << " to instance " << this << std::endl;
        _data = source._data;
        _size = source._size;
        source._data = nullptr;
        source._size = 0;
    }
    MyMovableClass &operator=(MyMovableClass &&source) // 5 : move assignment operator
    {
        std::cout << "MOVING (assign) instance " << &source << " to instance " << this << std::endl;
        if (this == &source)
            return *this;

        delete[] _data;

        _data = source._data;
        _size = source._size;

        source._data = nullptr;
        source._size = 0;

        return *this;
    }
    ~MyMovableClass() // 1 : destructor
    {
        std::cout << "DELETING instance of MyMovableClass at " << this << std::endl;
        delete[] _data;
    }
    MyMovableClass(const MyMovableClass &source) // 2 : copy constructor
    {
        _size = source._size;
        _data = new int[_size];
        *_data = *source._data;
        std::cout << "COPYING content of instance " << &source << " to instance " << this << std::endl;
    }
    MyMovableClass &operator=(const MyMovableClass &source) // 3 : copy assignment operator
    {
        std::cout << "ASSIGNING content of instance " << &source << " to instance " << this << std::endl;
        if (this == &source)
            return *this;
        delete[] _data;
        _data = new int[source._size];
        *_data = *source._data;
        _size = source._size;
        return *this;
    }
};

int main()
{
    MyMovableClass obj1(10); // regular constructor
    MyMovableClass obj2(obj1); // copy constructor
    obj2 = obj1; // copy assignment operator
    return 0;
}
```
This produces outputs similar to the ones below in the command line after compiling and linking:

CREATING instance of MyMovableClass at 0x7ffc1e837880 allocated with size = 40 bytes

COPYING content of instance 0x7ffc1e837880 to instance 0x7ffc1e837890

ASSIGNING content of instance 0x7ffc1e837880 to instance 0x7ffc1e837890

DELETING instance of MyMovableClass at 0x7ffc1e837890

DELETING instance of MyMovableClass at 0x7ffc1e837880

In the project, find similar signatures for the class ChatBot. In _chatbot.h_ find these methods that make ChatBot comply with the rule of five:
```cpp
// CONSTRUCTOR WITHOUT MEMORY ALLOCATION
ChatBot();  
// CONSTRUCTOR WITHOUT MEMORY ALLOCATION                  
ChatBot(std::string filename);
// DESTRUCTOR
~ChatBot();
// COPY ASSIGNMENT OPERARTOR
ChatBot &operator=(const ChatBot &source);
// COPY CONSTRUCTOR
ChatBot(const ChatBot &source);
// MOVE CONSTRUCTOR
ChatBot(ChatBot &&source);
// MOVE ASSIGNMENT OPERARTOR
ChatBot &operator=(ChatBot &&source);
```
