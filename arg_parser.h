#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include "config.h"
#include <string>

class ArgParser {
public:
    ArgParser(int argc, char* argv[]);
    
    // Getters
    int getBrightness() const { return brightness_; }
    bool hasHelp() const { return showHelp_; }
    
    // Display help
    void printHelp(const char* programName) const;
    
private:
    int brightness_;
    bool showHelp_;
    
    void parseArguments(int argc, char* argv[]);
    bool isValidBrightness(int brightness) const;
};

#endif // ARG_PARSER_H
