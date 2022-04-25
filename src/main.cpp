#include "main.h"
#include "DriveSystem.hpp"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);
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
    pros::Motor rightMotor(2);
	rightMotor.set_reversed(true);
    pros::Motor carriageMotor(3);
    pros::Motor headMotor(4);



	DriveSystem driver (&leftMotor, &rightMotor, &carriageMotor, &headMotor);
	driver.setUnitMode(Millimeters);
	pros::delay(1000);
	pros::lcd::set_text(1, "Homing");
	//driver.findCarriageEnd(-20);

	pros::delay(1000);
	//driver.findCarriageEnd(-10);
	driver.autoCalibrate(297, 10);

	driver.home(10);

	driver.setTargetY(100);
	driver.setFeedrate(100);
	driver.directMoveToTarget(false);
	driver.setTargetY(200);
	driver.directMoveToTarget(false);

	std::cout << "Done moving" << std::endl;

	/*driver.moveX(30, 10, true);
	driver.moveY(300, 30, false);*/


	while (true) {
		//std::cout << leftMotor.get_position() << ", " << rightMotor.get_position() << std::endl;
		pros::delay(50);
		/*leftMotor.move_absolute(1000, 30);
		rightMotor.move_absolute(1000, 30);
		carriageMotor.move_velocity(50);

		pros::delay(2000);

		leftMotor.move_absolute(10, 30);
		rightMotor.move_absolute(10, 30);
		carriageMotor.move_velocity(-50);

		pros::delay(2000);*/
	}


}
