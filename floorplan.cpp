#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>
#include "avlbst.h"
#include <map>

using namespace std;

struct Rectangle
{
    int ID;
    int length;
    int height;
};

// Typedefs for you input and output map. Start with
// std::map but once the backtracking search algorithm is working
// try to use your AVL-tree map implementation by changing these
// typedef's
typedef AVLTree<int, Rectangle> InputMapType;
typedef AVLTree<int, std::pair<int, int> > OutputMapType;

// Allowed global variables: the dimensions of the grid
int n; // X-dim size
int m; // Y-dim size

// No other global variables are allowed

// A dummy operator << for Rectangles so the BST and AVL BST will
// compile with their printRoot() implementations
std::ostream& operator<<(std::ostream& os, const Rectangle& r)
{
    os << r.ID;
    return os;
}

// A dummy operator << for pairs so the BST and AVL BST will
// compile with their printRoot() implementations
template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T,U>& p)
{
    os << p.first << "," << p.second;
    return os;
}



void printSolution(std::ostream& os, InputMapType& input, OutputMapType& output)
{
    for (OutputMapType::iterator it = output.begin(); it != output.end(); ++it)
    {
        InputMapType::iterator rbit = input.find(it->first);
        os << it->first << " ";
        os << it->second.first << " ";
        os << it->second.second << " ";
        os << rbit->second.length << " ";
        os << rbit->second.height << endl;
    }
}

// Changes the grid entries to their opposite values for the
// rectangle r starting at x1,y1
void flip(int x1, int y1, const Rectangle& r, vector<vector<bool> >& grid)
{
    for (int x = x1; x < x1+r.length; x++)
    {
        for (int y = y1; y < y1+r.height; y++) grid[x][y] = !grid[x][y];
    }
}

//check if rectangle is within bounds of grid and if spaces on grid are already occupied or not
bool gridcheck(int row, int col, InputMapType::iterator it, vector<vector<bool> > grid)
{
    //check if rectangle is within bounds of board
    if(it->second.length + row > n || it->second.height + col > m)
    {
        return false;
    }
    //check if needed space for rectangle are available or already filled
    for (int x = row; x < row+it->second.length; x++)
    {
        for (int y = col; y < col+it->second.height; y++)
        {
            if(grid[x][y] == true)
            {
                return false;
            }
        }
    }
    return true;
}

// TODO: Write your backtracking search function here
bool search(InputMapType::iterator it, vector<vector<bool> > grid, InputMapType::iterator end, OutputMapType &answer)
{
    if(it == end)
    {
        return true;
    }
    else
    {
        for(int i = 0; i < n; i++)
        {
            for(int j = 0; j < m; j++)
            {
                if(gridcheck(i,j,it, grid))
                {
                    flip(i, j, it->second, grid);
                    //create a temp iterator so that we don't change the original iterator passed in
                    //to the function in case we return
                    InputMapType::iterator temp = it;
                    if(search(++temp, grid, end,answer))
                    {
                        //succesfully placed the rectangle onto board, so insert into OutputMap
                        pair<int, std::pair<int, int>> rect = make_pair(it->first, std::make_pair(i,j));
                        answer.insert(rect);
                        return true;
                    }
                    //unsuccessful in trying to fit rectangle onto current board state
                    else
                    {
                        flip(i,j, it->second,grid);
                    }
                }
            }
        }
        //rotate the rectangle by switching its lenght and height and try everything again
        int temp = it->second.height;
        it->second.height = it->second.length;
        it->second.length = temp;
        for(int i = 0; i < n;i++)
        {
            for(int j = 0; j < m; j++)
            {
                if(gridcheck(i,j, it, grid))
                {
                    flip(i, j, it->second, grid);
                    InputMapType::iterator temp = it;
                    if(search(++temp,grid, end,answer))
                    {
                        pair<int, std::pair<int, int>> rect = make_pair(it->first, std::make_pair(i,j));
                        answer.insert(rect);
                        return true;
                    }
                    else
                    {
                        flip(i,j, it->second,grid);
                    }
                }
            }
        }
    }
    return false;
}




int main(int argc, char *argv[])
{
    if (argc < 3) {
        cout << "please specify an input and output file";
        return 0;
    }
    ifstream ifile(argv[1]);
    //check if input file exists
    if(ifile.fail())
    {
        cout << "invalid input file"<<endl;
        return 0;
    }
    stringstream ss;
    string line;
    ofstream ofile(argv[2]);
    int x;
    getline(ifile, line);
    ss << line;
    ss >> n;
    ss >> m;
    ss >> x;

    InputMapType input;
    OutputMapType output;
    for (int i = 0; i < x; i++) {
        getline(ifile, line);
        stringstream ss2(line);
        Rectangle r;
        ss2 >> r.ID;
        ss2 >> r.length;
        ss2 >> r.height;
        input.insert(std::make_pair(r.ID, r));
    }
    ifile.close();
    vector<vector<bool> > grid;

    for (int i = 0; i < n; i++)
    {
        grid.push_back(vector<bool>(m, false));
    }
    InputMapType::iterator it = input.begin();
    bool solution_exists = false;

    solution_exists = search(it, grid, input.end(), output);

    if (!solution_exists) {
        ofile << "No solution found.";
    }
    else {
        printSolution(ofile, input, output);
    }
    ofile.close();
    return 0;
}
