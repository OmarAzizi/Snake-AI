#include "snake.h"

snake* head;
snake* tail;

void init_snake() {

    snake* new = malloc(sizeof(snake));
    new->x = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
    new->y = rand() % (GRID_SIZE / 2) + (GRID_SIZE / 4);
    new->dir = SNAKE_UP;
    new->next = NULL;
    
    head = new;
    tail = new;

    return;
}

void increase_snake() {

    snake* new = malloc(sizeof(Snake));
    
    switch (tail->dir) {
        case SNAKE_UP:
            new->x = tail->x;
            new->y = tail->y + 1;
            break;
        case SNAKE_DOWN:
            new->x = tail->x;
            new->y = tail->y - 1;
            break;
        case SNAKE_RIGHT:
            new->x = tail->x - 1;
            new->y = tail->y;
            break;
        case SNAKE_LEFT:
            new->x = tail->x + 1;
            new->y = tail->y;
            break;
    }
 
    new->dir = tail->dir;
    new->next = NULL;
    tail->next = new;

    tail = new;

    return;
}

void move_snake() {
    
    int prev_x = head->x;
    int prev_y = head->y;
    
    int prev_dir = head->dir;
    switch (head->dir) {
        case SNAKE_UP:
            head->y--;
            break;
        case SNAKE_DOWN:
            head->y++;
            break;
        case SNAKE_LEFT:
            head->x--;
            break;
        case SNAKE_RIGHT:
            head->x++;
            break;
    }
    
    snake* track = head;
    if (track->next != NULL) track = track->next;

    while (track != NULL) { 

        int save_x = track->x;
        int save_y = track->y;
        int save_dir = track->dir;

        track->x = prev_x;
        track->y = prev_y;
        track->dir = prev_dir;
        track = track->next;

        prev_x = save_x;
        prev_y = save_y;
        prev_dir = save_dir;
    }

    return;
}

void render_snake(SDL_Renderer* renderer, int x, int y) {

    int seg_size = GRID_DIM / GRID_SIZE;
    SDL_Rect seg;
    seg.w = seg_size - 2;
    seg.h = seg_size - 2;
    
    snake* track = head;
    
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);

    while (track != NULL) {
        
        seg.x = x + track->x * seg_size - 1;
        seg.y = y + track->y * seg_size - 1;

        SDL_RenderFillRect(renderer, &seg);

        track = track->next;
    }

    return;
}

void render_grid(SDL_Renderer* renderer, int x, int y) {

    SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0xFF, 0xFF);

#if 0

    int cell_size = GRID_DIM / GRID_SIZE;

    SDL_Rect cell; 
    cell.w = cell_size;
    cell.h = cell_size;

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            cell.x = x + (i * cell_size);
            cell.y = y + (j * cell_size);
            SDL_RenderDrawRect(renderer, &cell);
        }
    }

#else
    
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);

        SDL_Rect outline;
        outline.x = x;
        outline.y = y;
        outline.w = GRID_DIM;
        outline.h = GRID_DIM;
        SDL_RenderDrawRect(renderer, &outline);
    
#endif

    return;
}

void gen_apple() {
    
    bool in_snake;

    do {
        in_snake = false;
        apple.x = rand() % GRID_SIZE;
        apple.y = rand() % GRID_SIZE;
        
        snake* track = head;
        // making sure the apple wont be generated inside the snake
        while (track != NULL) {
            if (track->x == apple.x && track->y == apple.y) 
                in_snake = true;
            track = track->next;
        }
    } while (in_snake);
    
    ++apple.score; // increasing the socre when the snake eats apple

    return;
}

void render_apple(SDL_Renderer* renderer, int x, int y) {
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);

    int apple_size = GRID_DIM / GRID_SIZE;

    SDL_Rect app;
    app.w = apple_size;
    app.h = apple_size;
    app.x = x + apple.x * apple_size;
    app.y = y + apple.y * apple_size;
    
    SDL_RenderFillRect(renderer, &app);
}

void detect_apple() {

    // resetting the snake if it crashed with the walls 
    if (head->x == apple.x && head->y == apple.y) {
        gen_apple();
        increase_snake();
    }

    snake* track = head;
    if (track->next != NULL) track = track->next;
    
    // resetting the snake if it crashed with itself
    while (track != NULL) {
        if (track->x == head->x && track->y == head->y) {
            reset_snake();
        }
        track = track->next;
    }

    return;
}

void reset_snake() {
    
    snake* track = head;
    snake* temp;

    while (track != NULL) {
        temp = track;
        track = track->next;
        free(temp);
    }
    
    init_snake();
    increase_snake();
    
    if (apple.score > apple.top_score) 
        apple.top_score = apple.score;
    
    apple.score = 0; // resetting the score when the snake dies

    return;
}

void detect_crash() {

    if (head->x < 0 || head->x >= GRID_SIZE || head->y < 0 || head->y >= GRID_SIZE)
        reset_snake();
    return;
}

void turn_left() {

    switch (head->dir) {
        case SNAKE_UP:
            head->dir = SNAKE_LEFT;
            break;
        case SNAKE_DOWN:
            head->dir = SNAKE_RIGHT;
            break;
        case SNAKE_LEFT:
            head->dir = SNAKE_DOWN;
            break;
        case SNAKE_RIGHT:
            head->dir = SNAKE_UP;
            break;
    }
    return;
}

void turn_right() {

    switch (head->dir) {
        case SNAKE_UP:
            head->dir = SNAKE_RIGHT;
            break;
        case SNAKE_DOWN:
            head->dir = SNAKE_LEFT;
            break;
        case SNAKE_LEFT:
            head->dir = SNAKE_UP;
            break;
        case SNAKE_RIGHT:
            head->dir = SNAKE_DOWN;
            break;
    }
    return;
}

int state(int try) {

    int reward = 0;

    int try_x = head->x;
    int try_y = head->y;
    
    switch (head->dir) {
        case SNAKE_UP:
            switch (try) {
                case TRY_FORWARD:
                    --try_y;
                    break;
                case TRY_LEFT:
                    --try_x;
                    break;
                case TRY_RIGHT:
                    ++try_x;
                    break;
            }
            break;

        case SNAKE_DOWN:
            switch (try) {
                case TRY_FORWARD:
                    ++try_y;
                    break;
                case TRY_LEFT:
                    ++try_x;
                    break;
                case TRY_RIGHT:
                    --try_x;
                    break;
            }
            break;

        case SNAKE_LEFT:
            switch (try) {
                case TRY_FORWARD:
                    --try_x;
                    break;
                case TRY_LEFT:
                    ++try_y;
                    break;
                case TRY_RIGHT:
                    --try_y;
                    break;
            }
            break;

        case SNAKE_RIGHT:
            switch (try) {
                case TRY_FORWARD:
                    ++try_x;
                    break;
                case TRY_LEFT:
                    --try_y;
                    break;
                case TRY_RIGHT:
                    ++try_y;
                    break;
            }
            break;
    }

    // negative reward (Penalty) for crashing the walls
    if (try_x < 0 || try_x > GRID_SIZE - 1) reward -= 100;
    if (try_y < 0 || try_y > GRID_SIZE - 1) reward -= 100;
    
    // positive reward if the snake ate the apple
    if (try_x == apple.x && try_y == apple.y) reward += 100;
    
    /*
    ---------------------------------------------------
        MOVING TOWARDS APPLE
    */    

    // calculating the difference between snake and apple
    int diff_x = abs(head->x - apple.x);
    int diff_y = abs(head->y - apple.y);     

    int try_diff_x = abs(try_x - apple.x);
    int try_diff_y = abs(try_y - apple.y);

    // giving a small reward if the snake moved closer to apple
    if (try_diff_x < diff_x) reward += 5; 
    if (try_diff_y < diff_y) reward += 5; 
    
    // DETECTING TAIL (OR SNAKE CRASHING INTO ITSELF)
    snake* track = head;

    if (track->next != NULL) track = track->next; // skipping the head
    
    // looping over everyting in the snake
    while (track != NULL) {
        if (try_x == track->x && try_y == track->y) reward -= 100;
        track = track->next;
    }
    return reward;
}

void ai() {

    int try_forward = state(TRY_FORWARD);
    int try_left = state(TRY_LEFT);
    int try_right = state(TRY_RIGHT);

    if (try_forward >= try_left && try_forward >= try_right) {
        // CONTINUE FORWARD
    } else {
        if (try_left > try_right)  
            turn_left();
        else turn_right();
    }

    return;
}

void render_score(SDL_Renderer* renderer, int x, int y) {
    
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    
    int cell_size = 9;

    SDL_Rect cell;
    cell.w = cell_size - 2;
    cell.h = cell_size - 2;
    
    char buff[10];
    sprintf(buff, "%4d", apple.score);
    
    for (int k = 0; k < 4; ++k) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (glyphs[buff[k]][j][i]) {
                    cell.x = x + cell_size * i + (cell_size * 9 * k);
                    cell.y = y + cell_size * j;
                    SDL_RenderFillRect(renderer, &cell);
                }
            }
        }
    }

    return;
}

void render_top_score(SDL_Renderer* renderer, int x, int y) {
    
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00, 0xFF);
    
    int cell_size = 9;

    SDL_Rect cell;
    cell.w = cell_size - 2;
    cell.h = cell_size - 2;
    
    char buff[10];
    sprintf(buff, "%4d", apple.top_score);
    
    for (int k = 0; k < 4; ++k) {
        for (int i = 0; i < 9; ++i) {
            for (int j = 0; j < 9; ++j) {
                if (glyphs[buff[k]][j][i]) {
                    cell.x = x + cell_size * i + (cell_size * 9 * k);
                    cell.y = y + cell_size * j;
                    SDL_RenderFillRect(renderer, &cell);
                }
            }
        }
    }

    return;
}

int main(void) {
    
    srand(time(0));

    SDL_Window* window;
    SDL_Renderer* renderer;

    if (SDL_INIT_VIDEO < 0) {
        fprintf(stderr, "ERROR: SDL_INIT_VIDEO");
        exit(EXIT_FAILURE);
    }
    window = SDL_CreateWindow(
                "Snake", 
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                SDL_WINDOW_BORDERLESS
             );

    if (!window) {
        fprintf(stderr, "ERROR: SDL_CreateWindow");
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    if (!renderer) {
        fprintf(stderr, "ERROR: SDL_CreateRenderer");
        exit(EXIT_FAILURE);
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    init_snake();
    increase_snake();
    gen_apple();
    
    apple.score = 0;

    bool quit = false;
    SDL_Event event;
    
    int grid_x = (WINDOW_WIDTH / 2) - (GRID_DIM / 2);
    int grid_y = (WINDOW_HEIGHT / 2) - (GRID_DIM / 2);

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYUP:
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                        case SDLK_UP:
                            if (head->dir != SNAKE_DOWN)
                                head->dir = SNAKE_UP;
                            break;
                        case SDLK_DOWN:
                            if (head->dir != SNAKE_UP)
                                head->dir = SNAKE_DOWN;
                            break;
                        case SDLK_LEFT:
                            if (head->dir != SNAKE_RIGHT)
                                head->dir = SNAKE_LEFT;
                            break;
                        case SDLK_RIGHT:
                            if (head->dir != SNAKE_LEFT)
                                head->dir = SNAKE_RIGHT;
                            break;
                    }
                    break;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0x11, 0x11, 0x11, 0xFF);
        SDL_RenderClear(renderer);
        // RENDER LOOP START
        
        move_snake();
        detect_apple();
        detect_crash();

        render_grid(renderer, grid_x, grid_y);
        render_snake(renderer, grid_x, grid_y);
        render_apple(renderer, grid_x, grid_y);
        
        render_score(renderer, WINDOW_WIDTH - 400, 50);
        render_top_score(renderer, WINDOW_WIDTH - 400, 200);

        ai();

        // RENDER LOOP END
        SDL_RenderPresent(renderer);

        SDL_Delay(DELAY_TIME);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
