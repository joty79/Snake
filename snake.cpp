#include <SDL2/SDL.h>
#include <cmath>
#include "snake.h"
#include "glyphs.hpp"

#define WINDOW_WIDTH 1920    // Increased for full HD
#define WINDOW_HEIGHT 1080   // Increased for full HD
#define GRID_SIZE 25        // Slightly larger grid cells
#define GRID_DIM 750        // Larger overall grid

class Game {
private:
    std::shared_ptr<Snake> head;
    std::shared_ptr<Snake> tail;
    Apple apple;
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool quit;
    bool paused;
    int actual_window_width;   // Actual window dimensions
    int actual_window_height;  // Will be set after window creation
    
public:
    Game() : head(nullptr), tail(nullptr), window(nullptr), renderer(nullptr), 
             quit(false), paused(false), actual_window_width(0), actual_window_height(0) {
        srand(static_cast<unsigned>(time(nullptr)));
    }
    
    ~Game() {
        if(renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if(window) {
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
    }

    bool init() {
        if(SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "ERROR: SDL_INIT_VIDEO" << std::endl;
            return false;
        }

        window = SDL_CreateWindow("Snake", 
                                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                WINDOW_WIDTH, WINDOW_HEIGHT,
                                SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN_DESKTOP);
        if(!window) {
            std::cerr << "ERROR: Could not create window" << std::endl;
            return false;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(!renderer) {
            std::cerr << "ERROR: Could not create renderer" << std::endl;
            return false;
        }

        // Get actual window size
        SDL_GetWindowSize(window, &actual_window_width, &actual_window_height);
        return true;
    }

    void init_snake() {
        head = std::make_shared<Snake>(GRID_DIM/(GRID_SIZE*2), GRID_DIM/(GRID_SIZE*2), SNAKE_RIGHT);
        tail = head;
    }

    void increase_snake() {
        std::shared_ptr<Snake> new_tail = std::make_shared<Snake>(tail->x, tail->y, tail->dir);
        tail->next = new_tail;
        tail = new_tail;
    }

    void gen_apple() {
        do {
            apple.x = rand() % (GRID_DIM/GRID_SIZE);
            apple.y = rand() % (GRID_DIM/GRID_SIZE);
        } while(check_collision(apple.x, apple.y));
    }

    bool check_collision(int x, int y) {
        std::shared_ptr<Snake> current = head;
        while(current) {
            if(current->x == x && current->y == y) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void detect_apple() {
        if(head->x == apple.x && head->y == apple.y) {
            increase_snake();
            apple.count++;
            gen_apple();
        }
    }

    void detect_crash() {
        if(head->x < 0 || head->x >= GRID_DIM/GRID_SIZE || 
           head->y < 0 || head->y >= GRID_DIM/GRID_SIZE) {
            reset_snake();
            return;
        }
        
        std::shared_ptr<Snake> current = head->next;
        while(current) {
            if(head->x == current->x && head->y == current->y) {
                reset_snake();
                return;
            }
            current = current->next;
        }
    }

    void reset_snake() {
        // Reset snake to initial state
        while(head->next) {
            std::shared_ptr<Snake> temp = head->next;
            head->next = temp->next;
        }
        tail = head;
        head->x = GRID_DIM/(GRID_SIZE*2);
        head->y = GRID_DIM/(GRID_SIZE*2);
        head->dir = SNAKE_RIGHT;
        
        // Reset score
        apple.count = 0;
        
        // Generate new apple
        gen_apple();
    }

    void move_snake() {
        std::shared_ptr<Snake> current = tail;
        while(current != head) {
            std::shared_ptr<Snake> prev = head;
            while(prev->next != current) {
                prev = prev->next;
            }
            current->x = prev->x;
            current->y = prev->y;
            current->dir = prev->dir;
            current = prev;
        }
        
        switch(head->dir) {
            case SNAKE_UP:    head->y--; break;
            case SNAKE_DOWN:  head->y++; break;
            case SNAKE_LEFT:  head->x--; break;
            case SNAKE_RIGHT: head->x++; break;
        }
    }

    void render_grid(int x, int y) {
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 255);
        SDL_Rect border{x-1, y-1, GRID_DIM+2, GRID_DIM+2};
        SDL_RenderDrawRect(renderer, &border);
    }

    void render_snake(int x, int y) {
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 255);
        std::shared_ptr<Snake> current = head;
        while(current) {
            SDL_Rect rect{
                x + current->x * GRID_SIZE,
                y + current->y * GRID_SIZE,
                GRID_SIZE,
                GRID_SIZE
            };
            SDL_RenderFillRect(renderer, &rect);
            current = current->next;
        }
    }

    void render_apple(int x, int y) {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 255);
        SDL_Rect rect{
            x + apple.x * GRID_SIZE,
            y + apple.y * GRID_SIZE,
            GRID_SIZE,
            GRID_SIZE
        };
        SDL_RenderFillRect(renderer, &rect);
    }

    void render_score(int x, int y) {
        // Move score to top-left corner
        int score_text_x = 50;  // Fixed position from left edge
        int score_y = 50;       // Fixed position from top edge

        // Draw "SCORE" in green
        SDL_SetRenderDrawColor(renderer, 0x00, 0xff, 0x00, 255);
        int letterSize = 10;

        // Draw "SCORE"
        draw_letter_S(score_text_x, score_y, letterSize);
        draw_letter_C(score_text_x + letterSize*4, score_y, letterSize);
        draw_letter_O(score_text_x + letterSize*8, score_y, letterSize);
        draw_letter_R(score_text_x + letterSize*12, score_y, letterSize);
        draw_letter_E(score_text_x + letterSize*16, score_y, letterSize);
        
        // Draw colon ":"
        SDL_Rect colon1{score_text_x + letterSize*20, score_y + letterSize*2, letterSize, letterSize};
        SDL_Rect colon2{score_text_x + letterSize*20, score_y + letterSize*4, letterSize, letterSize};
        SDL_RenderFillRect(renderer, &colon1);
        SDL_RenderFillRect(renderer, &colon2);
        
        // Draw the score number in yellow
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0x00, 255);  // Yellow color
        int score_x = score_text_x + letterSize*24;
        
        char buffer[100];
        snprintf(buffer, sizeof(buffer), "%d", apple.count);  // No padding
        int digits = strlen(buffer);  // Get actual number of digits
        
        int numberSize = letterSize * 0.8;  // Slightly smaller size for numbers
        int digitWidth = numberSize * 9;    // Full width of a digit
        int spacing = digitWidth / 3;       // Reduced spacing (was digitWidth/2)
        
        for(int k = 0; k < digits; k++) {
            int digit_x = score_x + k * (digitWidth + spacing);  // Position with spacing
            for(int i = 0; i < 9; i++) {
                for(int j = 0; j < 10; j++) {
                    SDL_Rect cell{
                        digit_x + i * numberSize,
                        score_y + j * numberSize,
                        numberSize,
                        numberSize
                    };
                    if(glyphs[(int)buffer[k] - '0'][j][i] == 1) {
                        SDL_RenderFillRect(renderer, &cell);
                    }
                }
            }
        }
    }

    void handle_event(SDL_Event& event, bool& moved) {
        if(event.type == SDL_QUIT) {
            quit = true;
        }
        else if(event.type == SDL_KEYDOWN && !moved) {
            switch(event.key.keysym.sym) {
                case SDLK_UP:
                    if(head->dir != SNAKE_DOWN) {
                        head->dir = SNAKE_UP;
                        moved = true;
                    }
                    break;
                case SDLK_DOWN:
                    if(head->dir != SNAKE_UP) {
                        head->dir = SNAKE_DOWN;
                        moved = true;
                    }
                    break;
                case SDLK_LEFT:
                    if(head->dir != SNAKE_RIGHT) {
                        head->dir = SNAKE_LEFT;
                        moved = true;
                    }
                    break;
                case SDLK_RIGHT:
                    if(head->dir != SNAKE_LEFT) {
                        head->dir = SNAKE_RIGHT;
                        moved = true;
                    }
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                case SDLK_SPACE:
                    paused = !paused;
                    break;
            }
        }
    }

    void show_welcome_screen() {
        SDL_Event event;
        bool waiting = true;
        float animation = 0;  // Changed to float for smoother animation
        
        while(waiting && !quit) {
            while(SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    quit = true;
                    break;
                }
                if(event.type == SDL_KEYDOWN) {
                    waiting = false;
                    break;
                }
            }

            SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
            SDL_RenderClear(renderer);

            // Draw a snake-like pattern
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 255);
            const int segments = 10;
            const int segmentSize = 40;
            for(int i = 0; i < segments; i++) {
                SDL_Rect segment{
                    (actual_window_width / 2) - 200 + (i * segmentSize) + (int)(sin(animation * 0.1 + i * 0.5) * 20),
                    (actual_window_height / 2) - 200 + (int)(cos(animation * 0.1 + i * 0.5) * 20),
                    segmentSize - 5,
                    segmentSize - 5
                };
                SDL_RenderFillRect(renderer, &segment);
            }

            // Draw an apple
            SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 255);
            SDL_Rect apple{
                (actual_window_width / 2) - 20,
                (actual_window_height / 2) - 100,  // Moved apple up
                40,
                40
            };
            SDL_RenderFillRect(renderer, &apple);

            // Draw "SNAKE" text
            SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 255);
            int letterSize = 20;
            int totalWidth = letterSize * (5 * 4);
            int startX = (actual_window_width - totalWidth) / 2;
            int startY = (actual_window_height / 2) + 50;  // Moved SNAKE below apple
            
            draw_letter_S(startX, startY, letterSize);
            draw_letter_N(startX + letterSize*4, startY, letterSize);
            draw_letter_A(startX + letterSize*8, startY, letterSize);
            draw_letter_K(startX + letterSize*12, startY, letterSize);
            draw_letter_E(startX + letterSize*16, startY, letterSize);

            // Draw "PRESS ANY KEY" text
            SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 255);
            letterSize = 10;
            int wordSpacing = letterSize * 8;
            totalWidth = letterSize * ((5 + 3 + 3) * 4) + wordSpacing * 2;
            startX = (actual_window_width - totalWidth) / 2;
            startY = (actual_window_height / 2) + 200;  // Moved "PRESS ANY KEY" further down
            
            // Draw "PRESS"
            draw_letter_P(startX, startY, letterSize);
            draw_letter_R(startX + letterSize*4, startY, letterSize);
            draw_letter_E(startX + letterSize*8, startY, letterSize);
            draw_letter_S(startX + letterSize*12, startY, letterSize);
            draw_letter_S(startX + letterSize*16, startY, letterSize);

            startX += letterSize*20 + wordSpacing;
            // Draw "ANY"
            draw_letter_A(startX, startY, letterSize);
            draw_letter_N(startX + letterSize*4, startY, letterSize);
            draw_letter_Y(startX + letterSize*8, startY, letterSize);

            startX += letterSize*12 + wordSpacing;
            // Draw "KEY"
            draw_letter_K(startX, startY, letterSize);
            draw_letter_E(startX + letterSize*4, startY, letterSize);
            draw_letter_Y(startX + letterSize*8, startY, letterSize);

            SDL_RenderPresent(renderer);
            SDL_Delay(16);
            animation += 0.5f;  // Slower animation
        }
    }

    void run() {
        if (!init()) return;

        show_welcome_screen();  // Show welcome screen first
        if(quit) return;       // Exit if user closed window

        init_snake();
        for(int i = 0; i < 4; i++) {
            increase_snake();
        }
        gen_apple();
        apple.count = 0;

        SDL_Event event;
        // Center the grid in the actual window dimensions
        int grid_x = (actual_window_width - GRID_DIM) / 2;
        int grid_y = (actual_window_height - GRID_DIM) / 2;
        bool moved = false;

        while(!quit) {
            while(SDL_PollEvent(&event)) {
                handle_event(event, moved);
            }

            SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 255);
            SDL_RenderClear(renderer);

            detect_apple();
            render_grid(grid_x, grid_y);
            render_snake(grid_x, grid_y);
            render_apple(grid_x, grid_y);
            render_score(grid_x, grid_y);

            if(!paused) {
                move_snake();
                moved = false;
                detect_crash();
            }

            SDL_RenderPresent(renderer);
            SDL_Delay(80);
        }
    }

    void draw_letter_S(int x, int y, int size) {
        SDL_Rect parts[] = {
            {x, y, size*3, size},             // top
            {x, y, size, size*2},             // left top
            {x, y + size*2, size*3, size},    // middle
            {x + size*2, y + size*2, size, size*2}, // right bottom
            {x, y + size*4, size*3, size}     // bottom
        };
        for(auto& part : parts) {
            SDL_RenderFillRect(renderer, &part);
        }
    }

    void draw_letter_C(int x, int y, int size) {
        SDL_Rect parts[] = {
            {x, y, size*3, size},             // top
            {x, y, size, size*5},             // left
            {x, y + size*4, size*3, size}     // bottom
        };
        for(auto& part : parts) {
            SDL_RenderFillRect(renderer, &part);
        }
    }

    void draw_letter_O(int x, int y, int size) {
        SDL_Rect parts[] = {
            {x, y, size*3, size},             // top
            {x, y, size, size*5},             // left
            {x + size*2, y, size, size*5},    // right
            {x, y + size*4, size*3, size}     // bottom
        };
        for(auto& part : parts) {
            SDL_RenderFillRect(renderer, &part);
        }
    }

    void draw_letter_R(int x, int y, int size) {
        SDL_Rect parts[] = {
            {x, y, size, size*5},             // left
            {x, y, size*3, size},             // top
            {x + size*2, y, size, size*2},    // right top
            {x, y + size*2, size*3, size},    // middle
            {x + size*2, y + size*3, size, size*2} // right bottom
        };
        for(auto& part : parts) {
            SDL_RenderFillRect(renderer, &part);
        }
    }

    void draw_letter_E(int x, int y, int size) {
        SDL_Rect parts[] = {
            {x, y, size*3, size},             // top
            {x, y, size, size*5},             // left
            {x, y + size*2, size*2, size},    // middle
            {x, y + size*4, size*3, size}     // bottom
        };
        for(auto& part : parts) {
            SDL_RenderFillRect(renderer, &part);
        }
    }

    void draw_letter_N(int x, int y, int size) {
        SDL_Rect parts[] = {
            {x, y, size, size*5},             // left
            {x + size, y + size, size, size*3}, // diagonal
            {x + size*2, y, size, size*5}     // right
        };
        for(auto& part : parts) {
            SDL_RenderFillRect(renderer, &part);
        }
    }

    void draw_letter_A(int x, int y, int size) {
        SDL_Rect parts[] = {
            {x + size, y, size, size},        // top
            {x, y + size, size, size*4},      // left
            {x + size, y + size*2, size, size}, // middle
            {x + size*2, y + size, size, size*4} // right
        };
        for(auto& part : parts) {
            SDL_RenderFillRect(renderer, &part);
        }
    }

    void draw_letter_K(int x, int y, int size) {
        SDL_Rect parts[] = {
            {x, y, size, size*5},             // left
            {x + size, y + size*2, size, size}, // middle
            {x + size*2, y, size, size*2},    // top right
            {x + size*2, y + size*3, size, size*2} // bottom right
        };
        for(auto& part : parts) {
            SDL_RenderFillRect(renderer, &part);
        }
    }

    void draw_letter_P(int x, int y, int size) {
        SDL_Rect parts[] = {
            {x, y, size, size*5},             // left
            {x, y, size*3, size},             // top
            {x + size*2, y, size, size*2},    // right top
            {x, y + size*2, size*3, size}     // middle
        };
        for(auto& part : parts) {
            SDL_RenderFillRect(renderer, &part);
        }
    }

    void draw_letter_Y(int x, int y, int size) {
        SDL_Rect parts[] = {
            {x, y, size, size*2},             // left top
            {x + size*2, y, size, size*2},    // right top
            {x + size, y + size*2, size, size*3} // middle bottom
        };
        for(auto& part : parts) {
            SDL_RenderFillRect(renderer, &part);
        }
    }
};

int SDL_main(int argc, char* argv[]) {
    Game game;
    game.run();
    return 0;
}