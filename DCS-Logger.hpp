#pragma once


#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <algorithm>
#include <vector>
#include <chrono>
class Logger
{
public:
	Logger::Logger() : totalTime(0.0), frequency(1.0){};
	Logger::Logger(std::string logLocation, std::vector<std::string>, bool msMode_ = false, int frequency = 1); // logLocation is full file path of log
	
    void Logger::AddToLogger(int addToLoggerInt); // Log a value of type int
    void Logger::AddToLogger(double addToLoggerDouble); // Log a value of type double
	void Logger::AddToLogger(std::string addToLoggerString); // Log a value of type std::string
	void Logger::AddToLogger(bool addToLoggerBool); // Log a value of type bool

    void Logger::SetCustomDelimiter(char newDelimiter); // Set a custom value delimiter

	void Logger::BeginFrame(double dt, bool loggingOnOff); // loggingOnOff : false off, true on
	void Logger::BeginFrame(bool loggingOnOff); // Initialize the frame this way for normal procedure, unless at very low framerate
	void Logger::EndFrame(); // End the frame
	void Logger::Close(); // Close the logger and free resources

private:
	double totalTime;
	std::chrono::steady_clock::time_point startTime;
	std::ofstream fout;
	std::vector<std::string> namesList;
	long frames;
	bool onoff;
	bool msMode;
	int frequency;
	bool offFrame;
    char delimiter;
};

Logger::Logger(std::string fileName, std::vector<std::string> namesList, bool msMode_, int frequency_)
{
	offFrame = false;
	
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	std::string timeString = buf;

	std::replace(timeString.begin(), timeString.end(), ':', '-');

	//debug_log("time stamp: %s", timeString.c_str());


	std::string fullFileName = fileName + "." + timeString + ".csv";
	fout.open(fullFileName, std::ios_base::out);

	this->frequency = frequency_;

	if (!fout.is_open()) printf("\nFile could not be written: %s", fullFileName.c_str());

	this->namesList = namesList;
	
	totalTime = 0.0;

	this->msMode = msMode_;
	if (this->msMode) startTime = std::chrono::steady_clock::now();

	fout << "Time, ";
	fout << "Frames, ";

	for (int i = 0; i < this->namesList.size(); i++)
	{
		fout << namesList[i] << ", ";
	}
	fout << "\n";

	frames = 0;
}

void Logger::BeginFrame(double dt, bool logginOnOff)
{
	offFrame = true;

	totalTime += dt;
	frames++;
	onoff = logginOnOff;

	if (!(frames % this->frequency))
		offFrame = false;

	if (!logginOnOff || offFrame == 0) return;

	if (this->msMode)
	{
		fout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - this->startTime).count() / 1000.0 << ", ";
	}
	else
	{
		fout << totalTime << ", ";
	}
	fout << frames << ", ";
}

void Logger::BeginFrame(bool loggingOnOff)
{
	offFrame = true;
	frames++;
	onoff = loggingOnOff;

	if (!(frames % this->frequency))
	{
		offFrame = false;
	}
	
	if (!loggingOnOff || offFrame) return;

	if (this->msMode)
	{
		fout << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - this->startTime).count() / 1000.0 << ", ";
	}
	fout << frames << ", ";
}

void Logger::AddToLogger(int addToLoggerInt)
{
    if (!onoff || offFrame) return;
    fout << addToLoggerInt << this->delimiter << " ";
}

void Logger::AddToLogger(double addToLoggerDouble)
{
	if (!onoff || offFrame) return;
	fout << addToLoggerDouble << this->delimiter << " ";
}

void Logger::AddToLogger(std::string addToLoggerString)
{
	if (!onoff || offFrame) return;
	fout << addToLoggerString << ", ";
}

void Logger::AddToLogger(bool addToLoggerBool)
{
	if (!onoff || offFrame) return;
	fout << addToLoggerBool << ", ";
}

void Logger::SetCustomDelimiter(char delimiter_)
{
    this->delimiter = delimiter_;
}

void Logger::EndFrame()
{
	if (!onoff || offFrame) return;
	fout << "\n";
}

void Logger::Close()
{
	if (fout.is_open()) fout.close();
}
