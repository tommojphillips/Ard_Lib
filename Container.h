#ifndef CONTAINER_H
#define CONTAINER_H

#include <Arduino.h>       // defines String type
#include <MCUFRIEND_kbv.h> // Hardware-specific library

/// @brief The settings for the container.
struct ContainerSettings
{
    /// @brief The width of the bar graph.
    int width = 50;
    /// @brief The height of the bar graph.
    int height = 215;
    /// @brief The distance in pixels from the edge of the bar graph to the edge of the screen.
    int margin = 5;
    /// @brief Creates a new instance of the ContainerSettings class.
    ContainerSettings(int width, int height, int margin) : width(width), height(height), margin(margin) { }
};
/// @brief A container to display a value as a bar graph.
class Container
{
    public:
        /// @brief The distance in pixels from the left of the screen.
        int left;
        /// @brief The distance in pixels from the top of the screen.
        int top;        

        /// @brief The color of the bar graph.
        unsigned int fillColor       = 0xffff;
        /// @brief The color of the bar graph outline.
        unsigned int containerColor  = 0xffff;
        /// @brief The color of the bar graph background.
        unsigned int backgroundColor = 0x0000;
        /// @brief The label contents of the bar graph.
        String name;
        
        /// @brief Creates a new instance of the Container class.
        /// @param lcd The LCD screen to draw on.
        /// @param settings The settings for the container.
        Container(MCUFRIEND_kbv &lcd, ContainerSettings& settings) : settings(settings), lcd(lcd)
        {
            previousLevel = 0;
        }

        /// @brief Creates a new instance of the Container class.
        /// @param left The distance in pixels from the left of the screen.
        /// @param top The distance in pixels from the top of the screen.
        /// @param name The label contents of the bar graph.
        /// @param outlineColor The color of the bar graph outline.
        /// @param fillColor The color of the bar graph.
        /// @param backgroundColor The color of the bar graph background.
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

        /// @brief Draws the bar graph. Should only be called once or when the bar graph moves moves..
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
        
        /// @brief Updates the bar graph with a new value. Only redraws the bar graph if the value has changed. 
        /// Only Updates the pixels that need to be changed to reduce flickering.
        /// @param level The new value to display. Range: 0 - 1.
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
        /// @brief The previous value of the bar graph.
        float previousLevel;
        /// @brief The settings for the container.
        ContainerSettings& settings;
        /// @brief The LCD screen to draw on.
        MCUFRIEND_kbv& lcd;

        /// @brief Fills the bar graph with a color.
        /// @param yOffset 
        /// @param heightOffset 
        /// @param color 
        void fillContainer(int yOffset, int heightOffset, unsigned int color)
        {
            lcd.fillRect(
                left + settings.margin + 2,         // left (x)
                top + 2 + yOffset,                  // top (y)
                settings.width - 4,                 // width
                settings.height - 4 - heightOffset, // height
                color);                             // color
        }
        /// @brief Calculates the height of the bar graph.
        /// @param modifier The level modifier. Range 0 - 1.
        /// @return 
        int calculateContainerHeight(float modifier)
        {
            return settings.height - ((settings.height - 4) * modifier) - 4;
        }
};
#endif