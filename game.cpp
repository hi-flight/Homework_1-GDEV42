#include <raylib.h>
#include <iostream>
#include <fstream>
#include <string>

float Lerp(float start, float end, float t) {
    return start + t * (end - start);
}

float Clamp(float value, float min, float max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

float screenwidth = 800;
float screenheight = 600;

Vector2 targetDestination;

float speed = 300;

// Adjusted the box initialization to be at the center of the screen
Rectangle box = Rectangle{screenwidth / 2 - 150, screenheight / 2 - 150, 300, 300};

bool zoom = false;
int main() {
    int cam_type;
    Camera2D camera_view = {0};
    float EDGE_X[2], EDGE_Y[2];

    std::string backgroundFilename;
    Texture2D background;
    std::ifstream inputFile("settings.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error: Failed to open settings.txt" << std::endl;
        return 1;
    }

    inputFile >> backgroundFilename;
    inputFile >> camera_view.target.x >> camera_view.target.y; // Load camera target
    inputFile >> camera_view.offset.x >> camera_view.offset.y; // Load camera offset
    inputFile >> camera_view.zoom; // Load camera zoom
    inputFile >> EDGE_X[0] >> EDGE_X[1];
    inputFile >> EDGE_Y[0] >> EDGE_Y[1];
    inputFile.close();

    InitWindow(screenwidth, screenheight, "Lim, Sta. Cruz, Tadiarca_Homework01");
    SetTargetFPS(60);

    background = LoadTexture(backgroundFilename.c_str());

    while (!WindowShouldClose()) {
        float delta_time = GetFrameTime();
        targetDestination = GetMousePosition();

        // Archived Cam Movement code
        // //if (targetDestination.y < box.y) {
        //     box.y -= speed * delta_time;
        //     targetDestination.y = box.y - 1;
        // }
        // if (targetDestination.x < box.x) {
        //     box.x -= speed * delta_time;
        //     targetDestination.x = box.x - 1;
        // } 
        // if (targetDestination.y > box.y) {
        //     box.y += speed * delta_time;
        //     targetDestination.y = box.y + 1;
        // } 
        // if (targetDestination.x > box.x) {
        //     box.x += speed * delta_time;
        //     targetDestination.x = box.x + 1;
        // } 

        // Check if the mouse is inside the box
        bool mouseInsideBox = CheckCollisionPointRec(targetDestination, box);

        // Update camera position only if the mouse is outside the box
        if (!mouseInsideBox) {
            camera_view.target.x += (targetDestination.x - screenwidth / 2) * delta_time * speed;
            camera_view.target.y += (targetDestination.y - screenheight / 2) * delta_time * speed;
        }

        // Smoothly interpolate camera position towards box position
        camera_view.target.x = Lerp(camera_view.target.x, box.x + box.width / 2, 0.1f);
        camera_view.target.y = Lerp(camera_view.target.y, box.y + box.height / 2, 0.1f);

        // Clamp camera's target position within the specified edges
        camera_view.target.x = Clamp(camera_view.target.x, EDGE_X[0], EDGE_X[1]);
        camera_view.target.y = Clamp(camera_view.target.y, EDGE_Y[0], EDGE_Y[1]);

        // Update box position based on camera's target position
        box.x = camera_view.target.x - box.width / 2;
        box.y = camera_view.target.y - box.height / 2;

        // Handle zoom
        if (IsMouseButtonPressed(0)) {
            zoom = !zoom;
            camera_view.zoom = zoom ? 3.0f : 1.0f;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        BeginMode2D(camera_view);
        DrawTextureEx(background, Vector2{0, 0}, 0, 1, WHITE);
        DrawRectangleLines(box.x, box.y, box.width, box.height, WHITE);
        EndMode2D();

        EndDrawing();
    }
    CloseWindow();

    return 0;
}
