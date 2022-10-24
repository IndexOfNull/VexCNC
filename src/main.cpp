#include "main.h"
#include "DriveSystem.hpp"
#include "gcode/GCodeParser.hpp"
#include "GCodeRunner.hpp"
#include "testgcode.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_background_color(0, 105, 255);
	pros::lcd::set_text_color(255, 255, 255);

	//pros::lcd::register_btn1_cb(on_center_button);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {



	pros::Motor leftMotor(1);
    pros::Motor rightMotor(2, true);
    pros::Motor carriageMotor(3, true);
    pros::Motor headMotor(4, true);

	DriveSystem driver (&leftMotor, &rightMotor, &carriageMotor, &headMotor, 295-50, 295-50, 135-106);
	driver.setUnitMode(Millimeters);

	// pros::delay(1000);
	// driver.calibrateHead(135-106);
	// pros::delay(1000);
	// driver.setFeedrate(1000);
	// driver.setTargetZ(-10);
	// driver.directMoveToTarget(false);
	
	pros::lcd::set_text(1, "Homing");

	//driver.autoCalibrate(295-50, 295-50, 10);
	pros::lcd::set_text(1, "Calibrating");
	//driver.millimeterToEncoderConsts[leftMotor.get_port()] = 0.130112;
	//driver.millimeterToEncoderConsts[rightMotor.get_port()] = 0.130112;
	//driver.millimeterToEncoderConsts[rightMotor.get_port()] = 0.128812;
	//driver.millimeterToEncoderConsts[carriageMotor.get_port()] = 0.0653159;
	driver.millimeterToEncoderConsts[leftMotor.get_port()] = 0.0623568;
	driver.millimeterToEncoderConsts[rightMotor.get_port()] = 0.0638853;
	driver.millimeterToEncoderConsts[carriageMotor.get_port()] = 0.0655255;
	driver.millimeterToEncoderConsts[headMotor.get_port()] = 0.0482234;
	
	driver.home(10);

	//std::cout << driver.millimeterToEncoderConsts[leftMotor.get_port()] << ", " << driver.millimeterToEncoderConsts[rightMotor.get_port()] << std::endl;
	//std::cout << "E->mm:" << driver.encoderToUnit(&leftMotor, 300) << ", mm->E" << driver.unitToEncoder(&leftMotor, 100) << std::endl;

	GCodeParser parser = GCodeParser();
	parser.inputData = COUGAR;

	parser.tokenizeGCode();
	std::list<Command> *commands = parser.getCommands();
	GCodeRunner runner = GCodeRunner(&driver);
	runner.setCommands(commands);
	runner.runAll();

	while (true) {
		pros::delay(1000);
	}

}
