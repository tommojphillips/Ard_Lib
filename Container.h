#include <Arduino.h>       // defines String type
#include <MCUFRIEND_kbv.h> // Hardware-specific library

struct ContainerSettings
{
    int width = 50;
    int height = 215;
    int margin = 5;

    ContainerSettings(int width, int height, int margin) : width(width), height(height), margin(margin) { }
};

class Container
{
    public:
        
        int left;
        int top;        
        unsigned int fillColor       = 0xffff;
        unsigned int containerColor  = 0xffff;
        unsigned int backgroundColor = 0x0000;
        String name;

        Container(MCUFRIEND_kbv &lcd, ContainerSettings& settings) : settings(settings), lcd(lcd)
        {
            previousLevel = 0;
        }

        void createContainer(int left, int top, String name, unsigned int outlineColor = 0xffff, unsigned int fillColor = 0xffff, unsigned int backgroundColor = 0x0000)
        {
            this->left = left;
            this->top = top;
            this->name = name;
            this->containerColor = outlineColor;
            this->fillColor = fillColor;
            this->backgroundColor = backgroundColor;

            drawContainer();         
        }

        void drawContainer()
        {
            lcd.drawRect(
                left + settings.margin, // left (x)
                top,                    // top (y)
                settings.width,         // width
                settings.height,        // height
                containerColor);        // color

            lcd.setCursor(left + (settings.width / 2) - (lcd.textWidth() / 2), top + settings.height + 2);
            lcd.print(name);
        }
        
        void updateLevel(float level)
        {
            if (level == previousLevel) 
            {
                return;
            }

            if (level > 1)
            {
                level = 1;
            } 
            else if (level < 0) 
            {
                level = 0;
            }

            int clevel = calculateContainerHeight(level);
            int plevel = calculateContainerHeight(previousLevel);
            int dlevel;

            if (level < previousLevel)
            {
                dlevel = calculateContainerHeight(previousLevel - level);
                fillContainer(plevel, dlevel, backgroundColor);
            }
            else
            { 
                dlevel = calculateContainerHeight(level - previousLevel);
                fillContainer(clevel, dlevel, fillColor);
            }

            previousLevel = level;
        }

    private:
        float previousLevel;

        ContainerSettings& settings;
        MCUFRIEND_kbv& lcd;

        void fillContainer(int yOffset, int heightOffset, unsigned int color)
        {
            lcd.fillRect(
                left + settings.margin + 2, // left (x)
                top + 2 + yOffset,          // top (y)
                settings.width - 4,         // width
                settings.height - 4 - heightOffset,   // height
                color);                               // color
        }

        int calculateContainerHeight(float modifier)
        {
            return settings.height - ((settings.height - 4) * modifier) - 4;
        }
};