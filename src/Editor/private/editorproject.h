#ifndef EDITOR_PROJECT_H
#define EDITOR_PROJECT_H
#pragma once

#include <string>

struct EditorProject {
    std::string projectName;
    std::string organization;
    std::string defaultScenePath; //TODO: Change to serializable scene?
    
    std::string versionString;

    // TODO: assembly file for code
    // TODO: Plugins
};


#endif

