/*
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./client_service.h"

// STL
#include <random>
#include <sstream>


std::string _generate_random_alphanum_string(size_t length)
{
	std::string alphanum = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::mt19937 mt(std::random_device{}());
	std::uniform_int_distribution<size_t> dist(0, alphanum.size() - 1);
	std::string random_secret;
	random_secret.reserve(length);
	for (int i = 0; i < length; ++i)
	{
		random_secret += alphanum[dist(mt)];
	}

	return random_secret;
}

std::string _generate_uuid4()
{
	std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution<> dist(0, 15);
	std::uniform_int_distribution<> dist2(8, 11);

	std::stringstream ss;
	int i;
	ss << std::hex;
	for (i = 0; i < 8; i++)
	{
		ss << dist(gen);
	}

	ss << "-";
	for (i = 0; i < 4; i++)
	{
		ss << dist(gen);
	}

	ss << "-4";
	for (i = 0; i < 3; i++)
	{
		ss << dist(gen);
	}

	ss << "-";
	ss << dist2(gen);
	for (i = 0; i < 3; i++)
	{
		ss << dist(gen);
	}

	ss << "-";
	for (i = 0; i < 12; i++)
	{
		ss << dist(gen);
	}

	return ss.str();
}
