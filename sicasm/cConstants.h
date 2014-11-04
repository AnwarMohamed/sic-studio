#pragma once
#include "sicasm.h"
#include <vector>
#include <algorithm>
#include <string>
#include <map>

using namespace std;


class cConstants
{
public:
	cConstants();
	~cConstants();

	static void construct_optable();

	static map<string, UCHAR> kOpTable;
};

