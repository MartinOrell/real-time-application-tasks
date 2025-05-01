#include "Gui.h"

#include <SFML/Window/Event.hpp>
#include <iostream>

Gui::Gui(unsigned int windowWidth, unsigned int windowHeight, std::string windowName)
: _window{ { windowWidth, windowHeight }, windowName }
, _windowWidth{windowWidth}
, _windowHeight{windowHeight}
, _scaleX{1.f}
, _scaleY{1.f}
, _backgroundColor{sf::Color(30,30,30)}
, _fontIsLoaded{false}
, _middleTextIsCreated{false}
, _displayMiddleText{false}
, _displayCellTexts{false}{}


void Gui::setBackgroundColor(sf::Color color){
    _backgroundColor = color;
}

void Gui::setBackgroundColor(unsigned char red, unsigned char green, unsigned char blue){
    _backgroundColor = sf::Color{red, green, blue};
}



void Gui::createMiddleText(){
    if(_middleTextIsCreated){
        return;
    }

    if(!_fontIsLoaded){
        _fontIsLoaded = _font.loadFromFile("res/fonts/arial.ttf");
        if(!_fontIsLoaded){
            std::cerr << "Failed to load font" << std::endl;
            return;
        }
    }

    _middleText.setFont(_font);
    _middleText.setCharacterSize(40);
    _middleText.setFillColor(sf::Color(255,255,255));
    _middleText.setPosition(_window.getView().getCenter());
    _middleTextIsCreated = true;
}

void Gui::setMiddleText(std::string text){
    if(!_middleTextIsCreated){
        createMiddleText();
        if(!_middleTextIsCreated){
            return;
        }
    }
    _middleText.setString(text);
    sf::FloatRect textRect = _middleText.getLocalBounds();
    float x = textRect.left + textRect.width/2.f;
    float y = textRect.top + textRect.height/2.f;
    _middleText.setOrigin(x,y);
    _displayMiddleText = true;
}

void Gui::setMiddleTextCharacterSize(unsigned int size){
    if(!_middleTextIsCreated){
        createMiddleText();
        if(!_middleTextIsCreated){
            return;
        }
    }
    _middleText.setCharacterSize(size);
    sf::FloatRect textRect = _middleText.getLocalBounds();
    float x = textRect.left + textRect.width/2.f;
    float y = textRect.top + textRect.height/2.f;
    _middleText.setOrigin(x,y);
}

void Gui::setMiddleTextColor(sf::Color color){
    _middleText.setFillColor(color);
}

void Gui::setMiddleTextColor(unsigned char red, unsigned char green, unsigned char blue){
    _middleText.setFillColor(sf::Color(red,green,blue));
}

void Gui::showMiddleText(){
    _displayMiddleText = true;
}

void Gui::hideMiddleText(){
    _displayMiddleText = false;
}



void Gui::createCells(int numColumns, int numRows){
    if(numColumns <= 0){
        std::cerr << "wrong number of columns to create cells" << std::endl;
        return;
    }
    
    if(numRows <= 0){
        std::cerr << "wrong number of rows to create cells" << std::endl;
        return;
    }

    if(_cells.size() > 0){
        std::cerr << "cells are already created" << std::endl;
        return;
    }

    _numColumns = numColumns;
    _numRows = numRows;
    int rectanglePixelWidth = _window.getSize().x/numColumns;
    int rectanglePixelHeight = _window.getSize().y/numRows;
    for(int y = 0; y < numRows; y++){
        for(int x = 0; x < numColumns; x++){
            sf::RectangleShape cell;
            cell.setSize(sf::Vector2f(rectanglePixelWidth, rectanglePixelHeight));
            cell.setPosition(sf::Vector2f(x*rectanglePixelWidth,y*rectanglePixelHeight));
            cell.setFillColor(sf::Color(255,255,255));
            cell.setOutlineColor(sf::Color(0,0,0));
            cell.setOutlineThickness(5);
            _cells.push_back(cell);
        }
    }
}

void Gui::setCellColor(int x, int y, sf::Color color){
    int cellPos = y*_numColumns+x;
    if(cellPos < 0 || cellPos >= _cells.size()){
        std::cerr << "Invalid coordinate (" << x << "," << y << ") when trying to set cell color";
        return;
    }
    _cells.at(cellPos).setFillColor(color);
}

void Gui::setCellColor(int x, int y, unsigned char red, unsigned char green, unsigned char blue){
    setCellColor(x,y,sf::Color(red,green,blue));
}

void Gui::setCellsOutlineColor(sf::Color color){
    for(auto& cell: _cells){
        cell.setOutlineColor(color);
    }
}

void Gui::setCellsOutlineColor(unsigned char red, unsigned char green, unsigned char blue){
    for(auto& cell: _cells){
        cell.setOutlineColor(sf::Color(red,green,blue));
    }
}

void Gui::setCellsOutlineThickness(float thickness){
    for(auto& cell: _cells){
        cell.setOutlineThickness(thickness);
    }
}



void Gui::createCellTextBoxes(){

    if(_cellTexts.size()>0){
        std::cerr << "Text for cells already created";
        return;
    }

    if(!_fontIsLoaded){
        _fontIsLoaded = _font.loadFromFile("res/fonts/arial.ttf");
        if(!_fontIsLoaded){
            std::cerr << "Failed to load font" << std::endl;
            return;
        }
    }

    for(const auto& cell: _cells){
        sf::Text cellText;
        cellText.setFont(_font);
        cellText.setCharacterSize(20);
        cellText.setFillColor(sf::Color(0,0,255));
        float px = cell.getPosition().x+cell.getSize().x/2;
        float py = cell.getPosition().y+cell.getSize().y/2;
        cellText.setPosition(px,py);

        cellText.setString("0");
        sf::FloatRect textRect = cellText.getLocalBounds();
        float x = textRect.left + textRect.width/2.f;
        float y = textRect.top + textRect.height/2.f;
        cellText.setOrigin(x,y);

        _cellTexts.push_back(cellText);
    }
    _displayCellTexts = true;
}

void Gui::setCellText(int x, int y, std::string text){
    int cellPos = y*_numColumns+x;
    if(cellPos < 0 || cellPos >= _cells.size()){
        std::cerr << "Invalid coordinate (" << x << "," << y << ") when trying to set cell text";
        return;
    }
    _cellTexts.at(cellPos).setString(text);
    sf::FloatRect textRect = _cellTexts.at(cellPos).getLocalBounds();
    float px = textRect.left + textRect.width/2.f;
    float py = textRect.top + textRect.height/2.f;
    _cellTexts.at(cellPos).setOrigin(px,py);
}

void Gui::setCellsTextCharacterSize(unsigned int size){
    for(auto& cellText:_cellTexts){
        cellText.setCharacterSize(size);
        sf::FloatRect textRect = cellText.getLocalBounds();
        float x = textRect.left + textRect.width/2.f;
        float y = textRect.top + textRect.height/2.f;
        cellText.setOrigin(x,y);
    }
}

void Gui::setCellsTextColor(sf::Color color){
    for(auto& cellText:_cellTexts){
        cellText.setFillColor(color);
    }
}

void Gui::setCellsTextColor(unsigned char red, unsigned char green, unsigned char blue){
    for(auto& cellText:_cellTexts){
        cellText.setFillColor(sf::Color(red,green,blue));
    }
}

void Gui::setCellTextColor(int x, int y, sf::Color color){
    int cellPos = y*_numColumns+x;
    if(cellPos < 0 || cellPos >= _cells.size()){
        std::cerr << "Invalid coordinate (" << x << "," << y << ") when trying to set cell text color";
        return;
    }
    _cellTexts.at(cellPos).setFillColor(color);
}

void Gui::setCellTextColor(int x, int y, unsigned char red, unsigned char green, unsigned char blue){
    setCellTextColor(x,y,(sf::Color(red,green,blue)));
}

void Gui::showCellText(){
    _displayCellTexts = true;
}

void Gui::hideCellText(){
    _displayCellTexts = false;
}



int Gui::getCellColumn(int px){
    if(_cells.size() == 0){
        return 0;
    }
    return px*_scaleX/_cells.at(0).getSize().x;
}

int Gui::getCellRow(int py){
    if(_cells.size() == 0){
        return 0;
    }
    return py*_scaleY/_cells.at(0).getSize().y;
}



bool Gui::pollEvent(sf::Event& event){
    bool success = _window.pollEvent(event);
    if(event.type == sf::Event::Resized){
        if(event.size.width != 0 && event.size.height != 0){
            float scaleX = (float)_windowWidth / (float)event.size.width;
            float scaleY = (float)_windowHeight / (float)event.size.height;
            if(scaleX > 0.1){
                _scaleX = _scaleX;
            }
            if(scaleY > 0.1){
                _scaleY = scaleY;
            }
        }
    }
    return success;
}

void Gui::draw(){
    _window.clear(_backgroundColor);
    for(const auto& cell:_cells){
        _window.draw(cell);
    }
    if(_displayCellTexts){
        for(const auto& cellText:_cellTexts){
            _window.draw(cellText);
        }
    }
    if(_displayMiddleText){
        _window.draw(_middleText);
    }
    _window.display();
}

void Gui::close(){
    _window.close();
}



















