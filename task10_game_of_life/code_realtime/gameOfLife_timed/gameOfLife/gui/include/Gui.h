#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <vector>
#include <SFML/Graphics/RectangleShape.hpp>

class Gui{
    public:
        Gui(unsigned int windowWidth, unsigned int windowHeight, std::string windowName);

        void setBackgroundColor(sf::Color color);
        void setBackgroundColor(unsigned char red, unsigned char blue, unsigned char green);

        void createMiddleText();
        void setMiddleText(std::string text);
        void setMiddleTextCharacterSize(unsigned int size);
        void setMiddleTextColor(sf::Color color);
        void setMiddleTextColor(unsigned char red, unsigned char green, unsigned char blue);
        void showMiddleText();
        void hideMiddleText();

        void createCells(int numColumns, int numRows);
        void setCellColor(int x, int y, sf::Color color);
        void setCellColor(int x, int y, unsigned char red, unsigned char green, unsigned char blue);
        void setCellsOutlineColor(sf::Color color);
        void setCellsOutlineColor(unsigned char red, unsigned char green, unsigned char blue);
        void setCellsOutlineThickness(float thickness);

        void createCellTextBoxes();
        void setCellText(int x, int y, std::string text);
        void setCellsTextCharacterSize(unsigned int size);
        void setCellsTextColor(sf::Color);
        void setCellsTextColor(unsigned char red, unsigned char green, unsigned char blue);
        void setCellTextColor(int x, int y, sf::Color);
        void setCellTextColor(int x, int y, unsigned char red, unsigned char green, unsigned char blue);
        void showCellText();
        void hideCellText();

        int getCellColumn(int px);
        int getCellRow(int py);
        
        bool pollEvent(sf::Event& event);
        void draw();
        void close();
        
    private:
        
        sf::RenderWindow _window;
        sf::Color _backgroundColor;

        unsigned int _windowWidth;
        unsigned int _windowHeight;
        float _scaleX;
        float _scaleY;

        bool _fontIsLoaded;
        sf::Font _font;
        
        bool _middleTextIsCreated;
        bool _displayMiddleText;
        sf::Text _middleText;

        std::vector<sf::RectangleShape> _cells;
        int _numColumns;
        int _numRows;

        bool _displayCellTexts;
        std::vector<sf::Text> _cellTexts;
};