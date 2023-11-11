#include <iostream>
#include "cmath"
#include <stdio.h>
#include <windows.h>
#include "vector"
#include "conio.h"
#include "3DObjects.cpp"
#include "ctime"
#include "string"

const int moveSpeed = 20, rotationSpeed = 15;

int main()
{
    const int width = 120, height = 30;

    wchar_t screen[width * height + 1] = {0};
    const float screenAspect = float(width / height), pixelAspect = 11.0 / 24.0;
    const char gradient[20] = ".:!/r(l1Z4H9W8$@";


    
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);

    DWORD dwBytesWritten = 0;
    
    std::vector<object*> objects;
    objects.push_back(new sphere(0, -4, 0, 2));
    objects.push_back(new sphere(0, -20, 0, 5));
    objects.push_back(new sphere(0, 0, 10, 3));
    objects.push_back(new sphere(7, 5, 0, 2.5f));
    objects.push_back(new plane({ 0, 0, -2 }, { 0, 0, 1 }));
    objects.push_back(new plane({10, 0, 0}, {-1, 0, 0}));
    
    Vector3f cameraPos(0, 0, 0), cameraDir(0, 0, 0);

    float dist = 0, min_dist = 100; int objNum = 0;;
    std::wstring str = L"fps:";
    int fps = 0, tempframe = 0;

    for (size_t frame = 0; frame < 100000; frame++, tempframe++){

        for (size_t i = 0; i < width; i++){
            for (size_t j = 0; j < height; j++){

                float x = float(i) / width * 2.0f - 1.0f;
                float y = float(j) / height * 2.0f - 1.0f;
                x *= screenAspect * pixelAspect;
                char pixel = ' ';

                Vector3f Ray(cos(cameraDir.y) * sin(cameraDir.x) + x * cos(cameraDir.x) - sin(cameraDir.x) * sin(cameraDir.y) * y,
                    -x * sin(cameraDir.x) + cos(cameraDir.x) * cos(cameraDir.y) - y * cos(cameraDir.x) * sin(cameraDir.y),
                    sin(cameraDir.y) + y * cos(cameraDir.y));
                Ray.normalize();
                min_dist = 100;
                for (int k = 0; k < objects.size(); k++)
                {
                    dist = objects[k]->isRayIntersects(cameraPos, Ray);
                    if (dist > 0 && dist < min_dist) {
                        min_dist = dist;
                        objNum = k;
                    }
                }

                if (min_dist > 0 && min_dist < 100)
                    pixel = gradient[max(16 - int(min_dist / 0.5), 0)];

                screen[i + j * width] = pixel;
            }
        }
        objects[0]->Center.x = 2*sin(frame / 300.0);
        objects[1]->Center.z = 2*sin(frame / 300.0);
        if (_kbhit()) 
            switch (_getch())
            {
            case 'w':
                cameraPos.x -= moveSpeed / 100.0 * sin(cameraDir.x);
                cameraPos.y -= moveSpeed / 100.0 * cos(cameraDir.x);
                break;
            case 's':
                cameraPos.x += moveSpeed / 100.0 * sin(cameraDir.x);
                cameraPos.y += moveSpeed / 100.0 * cos(cameraDir.x);
                break;
            case 'd':
                cameraPos.x -= moveSpeed / 100.0 * cos(cameraDir.x);
                cameraPos.y += moveSpeed / 100.0 * sin(cameraDir.x);
                break;
            case 'a':
                cameraPos.x += moveSpeed / 100.0 * cos(cameraDir.x);
                cameraPos.y -= moveSpeed / 100.0 * sin(cameraDir.x);
                break;
            case 32:
                cameraPos.z += moveSpeed / 100.0;
                break;
            case 87:
                cameraPos.z -= moveSpeed / 100.0;
                break;
            case 75:
                cameraDir.x -= rotationSpeed / 100.0;
                break;
            case 77:
                cameraDir.x += rotationSpeed / 100.0;
                break;
            case 72:
                cameraDir.y -= rotationSpeed / 100.0;
                break;
            case 80:
                cameraDir.y += rotationSpeed / 100.0;
                break;
            default:
                break;
            }
        if (clock() % CLOCKS_PER_SEC < 10 && tempframe>200) {
            str = L"fps: " + std::to_wstring(tempframe)+L", frame: " + std::to_wstring(frame)
                + L", time: " + std::to_wstring(clock()/CLOCKS_PER_SEC)
                + L", camera pos: {x=" + std::to_wstring(cameraPos.x) + L";y=" + std::to_wstring(cameraPos.y)
                + L";z=" + std::to_wstring(cameraPos.z) +L"}";
            tempframe = 0;
        }
        
        for (int i = 0; i < str.size(); i++)
            screen[i] = str[i];
        
        //camera.y = camera.y - 0.00005f;
        //camera.z = camera.z + 0.0005f;

        screen[width * height] = '\0';
        WriteConsoleOutputCharacterW(hConsole, screen, width * height, { 0, 0 }, &dwBytesWritten);


    }




}
