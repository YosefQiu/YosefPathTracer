#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <random>
#include <algorithm>
#include <functional>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS 1
#endif // WIN32


struct Ui_PT_Msg {
	bool bActive;
	int mIndex;
	Ui_PT_Msg() {
		bActive = false;
		mIndex = -1;
	}
};

extern Ui_PT_Msg SceneMsg;
extern Ui_PT_Msg EnvMsg;
extern Ui_PT_Msg DenoiseMsg;