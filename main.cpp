#include <ctime>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

#include "Point2D.h"
#include "VoronoiDiagram.hpp"
#include "Beachline.hpp"
#include "Mouse.hpp"

namespace bl = beachline;

static constexpr int w = 1200, h = 700, n = 70;

std::vector<Point2D> randomPoint(int number) {

    srand(time(NULL));
    std::vector<Point2D> points;

    for (int i = 0; i < number; ++i) {
        double x = rand() % w;
        double y = rand() % h;
        points.push_back(Point2D(x, y));
    }

    return points;
}

std::vector<Point2D> hexagons_points(int cr) {

    std::vector<Point2D> points;
    int xoffset = cr * 1.5f;
    int yoffset = cr * sqrt(3);
    int step = yoffset / 2;
    int k = 0;

    for (int i = 0; i < w + 100; i += xoffset) {

        for (int j = 0; j < h + 100; j += yoffset) {
            points.push_back(Point2D(i, j + step * (k % 2)));
        }

        k++;
    }

    return points;
}

void draw_he(SDL_Renderer* renderer, double x1, double y1, double x2, double y2) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderDrawLine(renderer, static_cast<int>(x1), static_cast<int>(y1), static_cast<int>(x2), static_cast<int>(y2));
}

void initEdgePointsVis(bl::HalfEdgePtr h, std::vector<double> &x, std::vector<double> &y,
                       const std::vector<Point2D> &points) {
    
    if (h->vertex != nullptr && h->twin->vertex != nullptr) {
        
        x[0] = h->vertex->point.x;
        x[1] = h->twin->vertex->point.x;
        
        y[0] = h->vertex->point.y;
        y[1] = h->twin->vertex->point.y;
        
    } else if (h->vertex != nullptr) {
        
        x[0] = h->vertex->point.x;
        y[0] = h->vertex->point.y;
        
        Point2D norm = (points[h->l_index] - points[h->r_index]).normalized().getRotated90CCW();
        x[1] = x[0] + norm.x * 1000;
        y[1] = y[0] + norm.y * 1000;
        
    } else if (h->twin->vertex != nullptr) {
        
        x[0] = h->twin->vertex->point.x;
        y[0] = h->twin->vertex->point.y;
        
        Point2D norm = (points[h->twin->l_index] - points[h->twin->r_index]).normalized().getRotated90CCW();
        x[1] = x[0] + norm.x * 1000;
        y[1] = y[0] + norm.y * 1000;
        
    } else {
        
        Point2D p1 = points[h->l_index], p2 = points[h->r_index];
        
        Point2D norm = (p1 - p2).normalized().getRotated90CCW();
        Point2D c = 0.5 * (p1 + p2);
        
        x[0] = c.x + norm.x * 1000;
        x[1] = c.x - norm.x * 1000;
        
        y[0] = c.y + norm.y * 1000;
        y[1] = c.y - norm.y * 1000;
    }
}

MouseInput mouse;

int main(int argc, char* argv[]) {
   

    /*std::cout << "Enter window width: ";
    std::cin >> w;

    std::cout << "Enter window height: ";
    std::cin >> h;

    std::cout << "Enter amount of points: ";
    std::cin >> n;
    std::cin.get();*/

    SDL_Window* win = SDL_CreateWindow("graphic", 0, 0, w, h, SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE);
    if (win == nullptr) {
        std::cout << "Failed to create window : " << SDL_GetError();
        return 1;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == nullptr) {
        std::cout << "Failed to create renderer : " << SDL_GetError();
        return 1;
    }

    std::vector<Point2D> points = randomPoint(n); // or hexagon points
    std::cout << points.size() << std::endl;

    std::vector<bl::HalfEdgePtr> halfedges, faces;
    std::vector<bl::VertexPtr> vertices;
    
    auto draw = [&]() {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

        for (size_t i = 0; i < points.size(); ++i) {
            SDL_RenderDrawPoint(ren, points[i].x, points[i].y);
        }

        build_voronoi(points, halfedges, vertices, faces);

        for (size_t i = 0; i < halfedges.size(); ++i) {
            bl::HalfEdgePtr h = halfedges[i];

            std::vector<double> x(2, 0.0), y(2, 0.0);
            initEdgePointsVis(h, x, y, points);
            draw_he(ren, x[0], y[0], x[1], y[1]);
        }

        halfedges.clear();
        vertices.clear();
        faces.clear();
    };

    draw();
    SDL_RenderPresent(ren);

    int mx = 0, my = 0;
    bool holding_left = false;
    bool holding_back = false;

    while (true) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                    
            }
        }

        mouse.update();
       
        if(mouse.button_pressed(SDL_BUTTON_LEFT)) {

            if (holding_left) {
                points.pop_back();
            }

            if (points.size() < n) {
                
                mouse.get_cursor_pos(mx, my);
                points.push_back(Point2D(static_cast<double>(mx), static_cast<double>(my)));
                draw();
            }

            holding_left = true;
        }
        else {
            holding_left = false;
        }

        if (mouse.button_pressed(SDL_BUTTON_X2)) {

            if (!holding_back && !points.empty()) {
                points.pop_back();
                draw();
            }
            
            holding_back = true;
        }
        else {
            holding_back = false;
        }

        SDL_RenderPresent(ren);
    }

    SDL_RenderPresent(ren);
    std::cin.get();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return EXIT_SUCCESS;
}

