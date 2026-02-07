//
// Created by bobi on 10/09/2025.
//

#include <iostream>
#include "Engine/Engine.h"
#include <bits/this_thread_sleep.h>
#include "Helper/Utils.h"
#include "Helper/Colors.h"
#include "Helper/Curve.h"
#include "Helper/NetworkUtils.h"

int main()
{
     using namespace DMX;

     Engine engine("testfile.txt");
     engine.setIP(NetworkUtils::primaryIP());
     // engine.setIP("192.168.0.52");

     engine.addToGroup("all", engine.patch("pixel", 6, 100));
     engine.addToGroup("all", engine.patch("pixel", 7, 100));
     engine.addToGroup("all", engine.patch("pixel", 8, 93));
     engine.addToGroup("all", engine.patch("pixel", 9, 120));

     engine.addColor("all", {0, 0, 0});
     engine.update();

     // std::cout << engine.describe();
     return 0;
}
