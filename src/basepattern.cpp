// Class used for loading an ILDA file ***INCOMPLETE***
// Using Decorator pattern
// https://sourcemaking.com/design_patterns/decorator
// https://refactoring.guru/design-patterns/decorator/cpp/example
#include "basepattern.h"

BasePattern::BasePattern(){
    patternList = getPatternList();
}

BasePattern::~BasePattern(){

}

vector<string> BasePattern::getPatternList(){
    string path = "./ilda/";
    ofDirectory dir(path);
    //only show ild files
    dir.allowExt("ild");
    //populate the directory object
    dir.listDir();
    dir.sort();

    vector<string> list;
    //go through and print out all the paths
    for(int i = 0; i < dir.size(); i++){
        list.push_back(dir.getPath(i));
    }

    return list;
}

void BasePattern::loadIlda(unsigned int index){
    this->sequence.load(patternList.at(index));
}
