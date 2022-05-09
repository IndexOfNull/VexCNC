#include "GCodeRunner.hpp"
#include "DriveSystem.hpp"

GCodeRunner::GCodeRunner(DriveSystem *system) {
    driveSystem = system;
}

GCodeRunner::~GCodeRunner() {

}