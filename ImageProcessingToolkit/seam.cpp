#include <cassert>
#include <limits>
#include <tgmath.h>
#include <vector>

#include "seam.h"

using namespace std;

// ***********************************
// TASK 1: COLOR
// ***********************************

// Returns red component (in the scale 0.0-1.0) from given RGB color.
double get_red(int rgb)
{
    return ((rgb & 0xFF0000) >> 16) / 255.0;
}

// Returns green component (in the scale 0.0-1.0) from given RGB color.
double get_green(int rgb)
{
    return ((rgb & 0xFF00) >> 8) / 255.0;
}

// Returns blue component (in the scale 0.0-1.0) from given RGB color.
double get_blue(int rgb)
{
    return (rgb & 0xFF) / 255.0;
}

// Returns the average of red, green and blue components from given RGB color. (Scale: 0.0-1.0)
double get_gray(int rgb)
{
    return (get_red(rgb) + get_green(rgb) + get_blue(rgb)) / 3;
}

// Returns the RGB value of the given red, green and blue components.
int get_RGB(double red, double green, double blue)
{
    return (int(red * 255) << 16) + (int(green * 255) << 8) + int(blue * 255);
}

// Returns the RGB components from given grayscale value (between 0.0 and 1.0).
int get_RGB(double gray)
{
    return get_RGB(gray, gray, gray); 
}

// Converts  RGB image to grayscale double image.
GrayImage to_gray(const RGBImage &cimage)
{
    GrayImage grayimg;
    const int rows = cimage.size();
    const int cols = cimage[0].size();
    
     /* rows is for the lines and cols for the columns
     */

    for(int i = 0; i < rows; i++)
    {
        std::vector<double> temp;
        for(int j = 0; j < cols; j++)
        {
            temp.push_back(get_gray(cimage[i][j]));
        }

        grayimg.push_back(temp);
    }

    return grayimg;
}

// Converts grayscale double image to an RGB image.
RGBImage to_RGB(const GrayImage &gimage)
{
    RGBImage rgbimg;
    const int rows = gimage.size();
    const int cols = gimage[0].size();

    for(int i = 0; i < rows; i++)
    {
        std::vector<int> temp;
        for(int j = 0; j < cols; j++)
        {
            temp.push_back(get_RGB(gimage[i][j]));
        }
        
        rgbimg.push_back(temp);
    }

    return rgbimg;
}

// ***********************************
// TASK 2: FILTER
// ***********************************

// Get a pixel without accessing out of bounds
// return nearest valid pixel color
void clamp(long &val, long max)
{
   if(val<0)
       val = 0;
   else if(val>max)
       val = max;
}

// Convolve a single-channel image with the given kernel.
// M[3x3]
GrayImage filter(const GrayImage &gray, const Kernel &kernel)
{

    GrayImage filtered;
    int img_rows = gray.size(); int img_cols = gray[0].size(); //image dimensions
    int kernel_rows = kernel.size(); int kernel_cols = kernel[0].size(); //kernel dimensions
    int row_bounds = round((kernel_rows -1) / 2); int col_bounds = round((kernel_cols -1) / 2); // bounds for the loops

    filtered.reserve(img_rows); 
    for(int i = 0; i < img_rows; ++i) // go trough all y coordinates
    {
        vector<double> temp;
        filtered.reserve(img_cols); // micro-optimization

        for(int j= 0; j < img_cols; ++j) // go trough all x coordinates
        {
            double pixel_sum = 0;

            for(int kr = -row_bounds; kr <= col_bounds; ++kr) // go trough all rows of the kernel
            {
                for(int kc = -col_bounds; kc <= col_bounds; ++kc) //go through all coefficients of the rows
                {
                    long clamped_i = i +kr;
                    long clamped_j = j + kc;
                    clamp(clamped_i, img_rows -1);
                    clamp(clamped_j, img_cols -1);

                    //calculate cumulative pixel_sum
                    pixel_sum += gray[clamped_i][clamped_j] * kernel[kr + row_bounds][kc + col_bounds];
                }

            }

            temp.push_back(pixel_sum);
        }

        filtered.push_back(temp);
    }


    return filtered;
}

// Smooth a single-channel image
GrayImage smooth(const GrayImage &gray)
{
    Kernel smooth_kernel  ={{0.1, 0.1, 0.1},
                            {0.1, 0.2, 0.1},
                            {0.1, 0.1, 0.1}};

    return filter(gray, smooth_kernel);
}

// Compute horizontal Sobel filter

GrayImage sobelX(const GrayImage &gray)
{
    Kernel sobel_x_kernel ={{-1, 0, 1},
                            {-2, 0, 2},
                            {-1, 0, 1}};

    return filter(gray, sobel_x_kernel);
}

// Compute vertical Sobel filter

GrayImage sobelY(const GrayImage &gray)
{
    Kernel sobel_y_kernel ={{-1,-2,-1},
                            { 0, 0, 0},
                            { 1, 2, 1}};

    return filter(gray, sobel_y_kernel);
}

// Compute the magnitude of combined Sobel filters

GrayImage sobel(const GrayImage &gray)
{
    GrayImage sobel_x = sobelX(gray), sobel_y = sobelY(gray), filtered; //get sobel-filtered images

    auto img_rows = gray.size();
    auto img_cols = gray[0].size();
    filtered.reserve(img_rows); //micro-optimization

    for(size_t i = 0; i < img_rows; ++i)
    {
        vector<double> temp;
        temp.reserve(img_cols); // micro-optimization

        for(size_t j = 0; j < img_cols; ++j)
        {
            double x = sobel_x[i][j], y = sobel_y[i][j];
            temp.push_back(sqrt(x * x  +  y * y));
        }

        filtered.push_back(temp);
    }

    return filtered;
}

// ************************************
// TASK 3: SEAM
// ************************************

Graph create_graph(const GrayImage &gray)
{
    constexpr double INF = numeric_limits<double>::max();

    auto img_rows = gray.size();
    auto img_cols = gray[0].size();
    auto graph_size = img_rows * img_rows;
    
    std::vector<size_t> succ;
    Graph graph;
    Node node;
    
    graph.reserve(graph_size + 2); // micro-optimization

    for(size_t i = 0; i < img_rows; i++)
    {
        for(size_t j = 0; j < img_cols; j++)
        {
           succ.clear();
            if(i < img_rows -1)
            {
                if(j == 0)
                {
                    succ.push_back(img_cols * (i + 1) + j);
                    succ.push_back(img_cols * (i + 1) + j + 1);
                }
                else
                {
                    if(j == img_cols -1)
                    {
                        succ.push_back(img_cols * (i + 1) + j - 1);
                        succ.push_back(img_cols * (i + 1) + j);
                    }
                    else
                    {
                        succ.push_back(img_cols * (i + 1) + j -1);
                        succ.push_back(img_cols * (i + 1) + j);
                        succ.push_back(img_cols * (i + 1) + j + 1);
                    }
                }
            }
            else
            {
                //Last Node to whom last row elements are successors
                succ.push_back(img_rows * img_cols + 1);
            }
            node = {succ, gray[i][j], INF, 0};
            graph.push_back(node);
        }
    }

        //Start node to whom row 0 elements are successors
        succ.clear();
        for(int k = 0; k < img_cols; ++k)
        {
            succ.push_back(k);
        }
        node = {succ, 0, INF,0};
        graph.push_back(node);

        succ.clear();
        //Commented because unit_test.cpp expects that the first node is empty
        //However, the example in slide 55 of "Mini-projet (Transparents).pdf" says the start node
        //is: img_rows * img_cols
        //succ.push_back(img_rows * img_cols);

        node = {succ, 0, INF, 0};
        graph.push_back(node);

        return graph;
}

// Return shortest path from Node from to Node to
// The path does NOT include the from and to Node
Path shortest_path(Graph &graph, size_t from, size_t to)
{

    graph[from].distance_to_target = graph[from].costs;

    // starts the modified Dijkstra's algorithm
    int limit = graph.size();
    bool modified = true;
    while(modified)
    {
        modified = false;
        for(size_t vertex_number = 0; vertex_number < limit; ++vertex_number)
        {
            auto vertex = graph[vertex_number];
            for(int successor : vertex.successors)
            {
                if(graph[successor].distance_to_target > vertex.distance_to_target + graph[successor].costs)
                {
                    graph[successor].distance_to_target = vertex.distance_to_target + graph[successor].costs;
                    graph[successor].predecessor_to_target = vertex_number;
                    modified = true;
                }
            }
        }
    }

    /* testing shortest_path independently ends successfully. For both tests.
     * Going trough Dijkstra two times fixes it alright ! It works.
     */

    size_t node_number = to;

    if(graph[node_number].predecessor_to_target == 0)
    {
        return { };
    }

    Path temp, shortest;
    while(graph[node_number].predecessor_to_target != from)
    {
        temp.push_back(graph[node_number].predecessor_to_target);
        node_number = graph[node_number].predecessor_to_target;
    }

    for(size_t i = temp.size() - 1; i > 0; --i) // > 0 because i is actually an *unsigned* long
    {
        shortest.push_back(temp[i]);
    }
    shortest.push_back(temp[0]);

    return shortest;
}

Path find_seam(const GrayImage &gray)
{
    Graph graph = create_graph(gray);
    Path path = shortest_path(graph, graph.size() - 2, graph.size() - 1), coordx_path;

    coordx_path.reserve(path.size()); // micro-optimizazion
    size_t img_rows = gray[0].size();
    for(size_t node_number : path)
    {
        coordx_path.push_back(node_number % img_rows);
    }

    return coordx_path;
}

// ***********************************
// TASK 3 provided functions
// Highlight or remove seam from RGB or gray image
// ***********************************

// Draw a seam on a gray image
// return a new gray image with the seam in black
GrayImage highlight_seam(const GrayImage &gray, const Path &seam)
{
    GrayImage result(gray);
    // Paint seam in black
    for (size_t row(0); row < seam.size(); ++row) {
        result[row][seam[row]] = 0;
    }
    return result;
}

// Draw a seam on an RGB image
// return a new RGB image with the seam in blue
RGBImage highlight_seam(const RGBImage &image, const Path &seam)
{
    RGBImage result(image);
    // Paint seam in blue
    for (size_t row(0); row < seam.size(); ++row) {
        result[row][seam[row]] = 0x000ff;
    }
    return result;
}

// Remove specified seam from a gray-scale image
// return the new gray image (width is decreased by 1)

GrayImage remove_seam(const GrayImage &gray, const Path &seam)
{
    GrayImage result(gray);
    for (size_t row(0); row < seam.size(); ++row) {
        result[row].erase(result[row].begin() + seam[row]);
    }
    return result;
}

// Remove specified seam from an RGB image
// return the new RGB image (width is decreased by 1)
RGBImage remove_seam(const RGBImage &image, const Path &seam)
{
    RGBImage result(image);
    for (size_t row(0); row < seam.size(); ++row) {
        result[row].erase(result[row].begin() + seam[row]);
    }
    return result;
}