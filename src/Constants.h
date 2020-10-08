#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include <string>

struct Constants
{
public:
    static const std::string defaultFileName;
    static constexpr int panelWidth = 400;
    static constexpr int panelHeight = 400;

    static constexpr int sliderXMinimum = -1000;
    static constexpr int sliderXMaximum = 1000;
    static constexpr int sliderXDefault = 0;


    static constexpr int sliderYMinimum = -1000;
    static constexpr int sliderYMaximum = 1000;
    static constexpr int sliderYDefault= 0;

    static constexpr float scaleMinimum = 100.0;
    static constexpr float scaleMaximum = 10000.0;
    static constexpr float scaleDefault = 1000.0;
 
};

#endif // CONSTANTS_H_