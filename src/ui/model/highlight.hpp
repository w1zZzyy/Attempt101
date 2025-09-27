#pragma once

namespace UI::Model 
{

enum HighLight {
    WhiteCell, 
    BlackCell, 
    Background, 
    Danger, 
    Valid, 
    Selected
};
static constexpr int HighLightCnt = 6;

}