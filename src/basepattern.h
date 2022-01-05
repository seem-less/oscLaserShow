// Class used for loading an ILDA file ***INCOMPLETE***
#ifndef BASEPATTERN_H
#define BASEPATTERN_H

#include "ofFileUtils.h"
#include "ofLog.h"
#include "ofxIldaFile.h"

class BasePattern
{
public:
    BasePattern();
    ~BasePattern();
    vector<string> getPatternList();
    void loadIlda(unsigned int index);

    ofxIldaFile::Sequence sequence;
    vector<string> patternList;
};

#endif // BASEPATTERN_H
