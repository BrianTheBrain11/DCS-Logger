# DCS-Logger
 A simple data logger for DCS world, in a header only configuration.

# Install
Add the "DCS-Logger.hpp" header to your project include directory. This is where your other .h or .hpp files would go. In Visual Studio, toggle the option to "view all files". Find the file in the list, and right click -> Include in Project

# Usage
I recommend using "ed_fm_set_plugin_data_install_path" to get the path you want to Log files to, otherwise you must provide a path to where you want logs output yourself. 
To define a Logger, use the "Logger" class. (std::string logLocation, std::vector<std::string> columnList, bool msMode_ = false, int frequency = 1)
'logLocation' is the path to the output folder, not the output file
the string vector is a vector of strings, in order, of the names of the columns in your output. For instance, std::vector<std::string> {"Cl", "Cd", "Thrust"}
'msMode' sets the logger into millisecond mode, which logs via delta time and not by frame calcuation. Recommended to leave this off unless you are running at a very low frame/execution rate
'frequency' is how often to log. The default of 1 will log the values of every frame. 2 will log every other frame, 3 will log every 3rd frame, etc. Recommended to set this as desired for file output size

After setting up the logger, you must initalize it at the beginning of every frame using Logger::BeginFrame(bool loggingOnOff) // for normal operation. setting this to true will output the file, and should be used as a "master" debug setting across all loggers.Recommended to be a private variable in your own class handled as an input or something similar

After that, you must log variables in the order that you listed them in the "columnsList" string vector. 
At the end of the frame, you must call Logger::EndFrame();
And to clear resources you must use Logger::Close() on simulation close in order to gain access to your logger file.

# Common usage errors
A common error occurs when using the delta time version of the logger. a double is not accurate enough to obtain good graph results, hence it is suggested to use the default mode of the logger.

Another common error occurs when the path you are pointing to has folders that don't exist. You must create the folder you want the output path to be. Otherwise, you will get an exception and it will not log.

# Simple Example
in main EFM file:
```cpp
void fm_initialize()
{
...
    Aircraft::logger = Logger("C:\Users\User\SavedGames\DCS\Mods\Aircraft\Aircraft\Logs", std::vector<std::string> { "AoA", "TAS", "IAS" });
...
}

void ed_fm_simulate(double dt)
{
// start of method
    Aircraft::logger.BeginFrame(true);
// do your fm simulation
...
    Aircraft::logger.addToLogger(Aircraft::AoA);
    Aircraft::logger.addToLogger(Aircraft::TAS);
    Aircraft::logger.addToLogger(Aircraft::IAS);
// wrap anything up/end method with
...
    Aircraft::logger.EndFrame();
}
```
