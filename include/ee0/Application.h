#pragma once

#include <string>

namespace ee0
{

class Application
{
public:
	virtual ~Application() {}
	virtual void LoadFromFile(const std::string& filepath) = 0;
	virtual void StoreToFile(const std::string& filepath) const = 0;
	
}; // Application

}