#ifndef GRIDPARTITION_HPP
#define GRIDPARTITION_HPP

#include <vector>

#include "libraries/math.hpp"
#include "Circle.hpp"

struct Cell {
    Vector2f position;
    float dimension;
    std::vector<Circle*> list;

    // Initializer (constructor)
    Cell(const Vector2f& position, float dimension)
        : position(position), dimension(dimension) {}


    // You can also add methods to add or remove individual circles
    void addCircle(Circle* circle) {
        list.push_back(circle);
    }

    void simulateCollisionInCell() {
        
        if (list.size() == 0)
        {
            return;
        }
        

        for (int i = 0; i < list.size() - 1; i++)
        {
            for (int j = i + 1; j < list.size(); j++)
            {
                if (isCircleColliding(list[i], list[j]))
                {
                    resolveCollision(list[i], list[j]);
                }
                
            }
            
        }
    }
};

struct GridPartition
{
    float gridDimension;
    std::vector<std::vector<Cell>> cells;

    GridPartition(float gridDimension, float cellDimension)
        : gridDimension(gridDimension) {
        // Determine the number of cells along one side
        int numCells = static_cast<int>(gridDimension / cellDimension);

        // Construct the grid with the cells
        for (int i = 0; i < numCells; i++) {
            std::vector<Cell> row;
            for (int j = 0; j < numCells; j++) {
                Vector2f position(i * cellDimension, j * cellDimension);
                row.push_back(Cell(position, cellDimension));
            }
            cells.push_back(row);
        }
    }

    void addCircle(Circle* circle, int pos, std::vector<std::vector<int>> &posXGrid, std::vector<std::vector<int>> &posYGrid) {
        // Determine the number of cells along one side
        int numCells = cells.size();

        // Calculate the individual cell dimension
        float cellDimension = gridDimension / numCells;

        // Find the indices of the cells that might contain the circle
        int startRow = std::max(0, static_cast<int>((circle->position.x - circle->radius) / cellDimension));
        int endRow = std::min(numCells, static_cast<int>((circle->position.x + circle->radius) / cellDimension) + 1);
        int startCol = std::max(0, static_cast<int>((circle->position.y - circle->radius) / cellDimension));
        int endCol = std::min(numCells, static_cast<int>((circle->position.y + circle->radius) / cellDimension) + 1);

        // Check these cells and add the circle if it intersects the cell
        for (int i = startRow; i < endRow; ++i) {
            for (int j = startCol; j < endCol; ++j) {
                // Get the position of the current cell
                Vector2f cellPosition = cells[i][j].position;

                // Check if the circle intersects the cell
                if (circle->position.x + circle->radius >= cellPosition.x &&
                    circle->position.x - circle->radius < cellPosition.x + cellDimension &&
                    circle->position.y + circle->radius >= cellPosition.y &&
                    circle->position.y - circle->radius < cellPosition.y + cellDimension) {
                    //std::cout << pos << std::endl;
                    posXGrid[pos].push_back(i);
                    posYGrid[pos].push_back(j);
                    cells[i][j].addCircle(circle);
                }
            }
        }
    }

    void SimulateCollision() {
        for (size_t i = 0; i < cells.size(); i++)
        {                        
            for (size_t j = 0; j < cells[i].size(); j++)
            {
                cells[i][j].simulateCollisionInCell();   
            }
            
        }
        
    }

    void updateCirclePosition(Circle* circle, int pos, std::vector<std::vector<int>> &posXGrid, std::vector<std::vector<int>> &posYGrid) {
        // Remove the circle from all cells it's currently in
        for (size_t i = 0; i < posXGrid[pos].size(); i++)
        {
            auto& cellCircles = cells[posXGrid[pos][i]][posYGrid[pos][i]].list;

            auto it = std::find(cellCircles.begin(), cellCircles.end(), circle);

            if (it != cellCircles.end()) {
                cellCircles.erase(it); // Remove the pointer from the vector
            }
        }

        posXGrid[pos].clear();
        posYGrid[pos].clear();
        
        // Re-add the circle based on its new position
        addCircle(circle, pos, posXGrid, posYGrid);
    }

    

    
};


#endif
