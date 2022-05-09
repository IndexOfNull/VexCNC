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
    pros::Motor carriageMotor(3);
    pros::Motor headMotor(4, true);

	DriveSystem driver (&leftMotor, &rightMotor, &carriageMotor, &headMotor);
	driver.setUnitMode(Millimeters);

	GCodeParser parser = GCodeParser();
	parser.inputData = TEST_GCODE;

	parser.tokenizeGCode();
	std::list<Command> *commands = parser.getCommands();
	GCodeRunner runner = GCodeRunner(&driver);
	runner.setCommands(commands);
	runner.runAll();

	while (true) {
		pros::delay(1000);
	}

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

	pros::delay(5000);
driver.setFeedrate(1000);driver.setTargetZ(5.54);driver.directMoveToTarget(false);
driver.setFeedrate(1000);driver.setTargetX(183.3679);driver.setTargetY(197.4317);driver.directMoveToTarget(false);
driver.setTargetZ(0.0000);driver.directMoveToTarget(false);
driver.setFeedrate(127);driver.setTargetZ(-0.0100);driver.directMoveToTarget(false);
driver.setFeedrate(1016);driver.setTargetX(88.2906);driver.setTargetY(197.4317);driver.directMoveToTarget(false);
driver.setTargetX(75.3267);driver.setTargetY(194.8117);driver.directMoveToTarget(false);
driver.setTargetX(64.7286);driver.setTargetY(187.6720);driver.directMoveToTarget(false);
driver.setTargetX(57.5772);driver.setTargetY(177.0924);driver.directMoveToTarget(false);
driver.setTargetX(54.9531);driver.setTargetY(164.1526);driver.directMoveToTarget(false);
driver.setTargetX(57.5767);driver.setTargetY(151.2118);driver.directMoveToTarget(false);
driver.setTargetX(64.7275);driver.setTargetY(140.6470);driver.directMoveToTarget(false);
driver.setTargetX(75.3257);driver.setTargetY(133.5253);driver.directMoveToTarget(false);
driver.setTargetX(88.2906);driver.setTargetY(130.9142);driver.directMoveToTarget(false);
driver.setTargetX(113.1293);driver.setTargetY(130.5865);driver.directMoveToTarget(false);
driver.setTargetX(128.8509);driver.setTargetY(127.4278);driver.directMoveToTarget(false);
driver.setTargetX(141.6824);driver.setTargetY(118.8134);driver.directMoveToTarget(false);
driver.setTargetX(150.3301);driver.setTargetY(106.0359);driver.directMoveToTarget(false);
driver.setTargetX(153.5000);driver.setTargetY(90.3885);driver.directMoveToTarget(false);
driver.setTargetX(150.3304);driver.setTargetY(74.7258);driver.directMoveToTarget(false);
driver.setTargetX(141.6835);driver.setTargetY(61.9301);driver.directMoveToTarget(false);
driver.setTargetX(128.8519);driver.setTargetY(53.2999);driver.directMoveToTarget(false);
driver.setTargetX(113.1293);driver.setTargetY(50.1346);driver.directMoveToTarget(false);
driver.setTargetX(17.8793);driver.setTargetY(50.1346);driver.directMoveToTarget(false);
driver.setTargetX(17.8412);driver.setTargetY(20.0000);driver.directMoveToTarget(false);
driver.setTargetX(113.0328);driver.setTargetY(20.0965);driver.directMoveToTarget(false);
driver.setTargetX(127.2441);driver.setTargetY(21.5243);driver.directMoveToTarget(false);
driver.setTargetX(140.4800);driver.setTargetY(25.6187);driver.directMoveToTarget(false);
driver.setTargetX(152.4569);driver.setTargetY(32.0970);driver.directMoveToTarget(false);
driver.setTargetX(162.8914);driver.setTargetY(40.6756);driver.directMoveToTarget(false);
driver.setTargetX(171.5005);driver.setTargetY(51.0716);driver.directMoveToTarget(false);
driver.setTargetX(178.0006);driver.setTargetY(63.0019);driver.directMoveToTarget(false);
driver.setTargetX(182.1083);driver.setTargetY(76.1833);driver.directMoveToTarget(false);
driver.setTargetX(183.5406);driver.setTargetY(90.3326);driver.directMoveToTarget(false);
driver.setTargetX(182.1083);driver.setTargetY(104.5073);driver.directMoveToTarget(false);
driver.setTargetX(178.0006);driver.setTargetY(117.7077);driver.directMoveToTarget(false);
driver.setTargetX(171.5005);driver.setTargetY(129.6521);driver.directMoveToTarget(false);
driver.setTargetX(162.8914);driver.setTargetY(140.0577);driver.directMoveToTarget(false);
driver.setTargetX(152.4569);driver.setTargetY(148.6421);driver.directMoveToTarget(false);
driver.setTargetX(140.4800);driver.setTargetY(155.1237);driver.directMoveToTarget(false);
driver.setTargetX(127.2441);driver.setTargetY(159.2194);driver.directMoveToTarget(false);
driver.setTargetX(113.0328);driver.setTargetY(160.6474);driver.directMoveToTarget(false);
driver.setTargetX(88.3109);driver.setTargetY(160.5128);driver.directMoveToTarget(false);
driver.setTargetX(84.7829);driver.setTargetY(164.0764);driver.directMoveToTarget(false);
driver.setTargetX(88.3109);driver.setTargetY(167.6426);driver.directMoveToTarget(false);
driver.setTargetX(183.3298);driver.setTargetY(167.6223);driver.directMoveToTarget(false);
driver.setTargetX(183.3679);driver.setTargetY(197.4317);driver.directMoveToTarget(false);
driver.setTargetX(183.3679);driver.setTargetY(197.4317);driver.directMoveToTarget(false);
driver.setFeedrate(1000);driver.setTargetZ(5.5400);driver.directMoveToTarget(false);
driver.setFeedrate(1000);driver.setTargetX(183.3679);driver.setTargetY(204.3913);driver.directMoveToTarget(false);
driver.setTargetZ(0.0000);driver.directMoveToTarget(false);
driver.setFeedrate(127);driver.setTargetZ(-0.0100);driver.directMoveToTarget(false);
driver.setFeedrate(1016);driver.setTargetX(183.3298);driver.setTargetY(234.3150);driver.directMoveToTarget(false);
driver.setTargetX(183.3298);driver.setTargetY(234.3150);driver.directMoveToTarget(false);
driver.setTargetX(88.3109);driver.setTargetY(234.3150);driver.directMoveToTarget(false);
driver.setTargetX(74.1113);driver.setTargetY(232.8888);driver.directMoveToTarget(false);
driver.setTargetX(60.8810);driver.setTargetY(228.7977);driver.directMoveToTarget(false);
driver.setTargetX(48.9049);driver.setTargetY(222.3240);driver.directMoveToTarget(false);
driver.setTargetX(38.4675);driver.setTargetY(213.7489);driver.directMoveToTarget(false);
driver.setTargetX(29.8541);driver.setTargetY(203.3550);driver.directMoveToTarget(false);
driver.setTargetX(23.3489);driver.setTargetY(191.4236);driver.directMoveToTarget(false);
driver.setTargetX(19.2369);driver.setTargetY(178.2369);driver.directMoveToTarget(false);
driver.setTargetX(17.8031);driver.setTargetY(164.0764);driver.directMoveToTarget(false);
driver.setTargetX(19.2369);driver.setTargetY(149.9152);driver.directMoveToTarget(false);
driver.setTargetX(23.3489);driver.setTargetY(136.7265);driver.directMoveToTarget(false);
driver.setTargetX(29.8541);driver.setTargetY(124.7923);driver.directMoveToTarget(false);
driver.setTargetX(38.4675);driver.setTargetY(114.3950);driver.directMoveToTarget(false);
driver.setTargetX(48.9049);driver.setTargetY(105.8169);driver.directMoveToTarget(false);
driver.setTargetX(60.8810);driver.setTargetY(99.3402);driver.directMoveToTarget(false);
driver.setTargetX(74.1113);driver.setTargetY(95.2470);driver.directMoveToTarget(false);
driver.setTargetX(88.3109);driver.setTargetY(93.8200);driver.directMoveToTarget(false);
driver.setTargetX(112.8981);driver.setTargetY(93.7819);driver.directMoveToTarget(false);
driver.setTargetX(116.3475);driver.setTargetY(90.3326);driver.directMoveToTarget(false);
driver.setTargetX(112.8981);driver.setTargetY(86.9214);driver.directMoveToTarget(false);
driver.setTargetX(17.8412);driver.setTargetY(86.8452);driver.directMoveToTarget(false);
driver.setTargetX(17.8793);driver.setTargetY(57.1323);driver.directMoveToTarget(false);
driver.setTargetX(113.1293);driver.setTargetY(57.1323);driver.directMoveToTarget(false);
driver.setTargetX(126.1206);driver.setTargetY(59.7436);driver.directMoveToTarget(false);
driver.setTargetX(136.7221);driver.setTargetY(66.8676);driver.directMoveToTarget(false);
driver.setTargetX(143.8661);driver.setTargetY(77.4383);driver.directMoveToTarget(false);
driver.setTargetX(146.4846);driver.setTargetY(90.3910);driver.directMoveToTarget(false);
driver.setTargetX(143.8661);driver.setTargetY(103.3293);driver.directMoveToTarget(false);
driver.setTargetX(136.7221);driver.setTargetY(113.8881);driver.directMoveToTarget(false);
driver.setTargetX(126.1206);driver.setTargetY(121.0034);driver.directMoveToTarget(false);
driver.setTargetX(113.1293);driver.setTargetY(123.6117);driver.directMoveToTarget(false);
driver.setTargetX(88.2906);driver.setTargetY(123.8809);driver.directMoveToTarget(false);
driver.setTargetX(72.5883);driver.setTargetY(127.0437);driver.directMoveToTarget(false);
driver.setTargetX(59.7621);driver.setTargetY(135.6713);driver.directMoveToTarget(false);
driver.setTargetX(51.1124);driver.setTargetY(148.4722);driver.directMoveToTarget(false);
driver.setTargetX(47.9402);driver.setTargetY(164.1552);driver.directMoveToTarget(false);
driver.setTargetX(51.1124);driver.setTargetY(179.8033);driver.directMoveToTarget(false);
driver.setTargetX(59.7621);driver.setTargetY(192.5869);driver.directMoveToTarget(false);
driver.setTargetX(72.5883);driver.setTargetY(201.2087);driver.directMoveToTarget(false);
driver.setTargetX(88.2906);driver.setTargetY(204.3710);driver.directMoveToTarget(false);
driver.setTargetX(183.3679);driver.setTargetY(204.3913);driver.directMoveToTarget(false);
driver.setFeedrate(1000);driver.setTargetZ(5.5400);driver.directMoveToTarget(false);





	std::cout << "Done moving" << std::endl;



	while (true) {
		//std::cout << leftMotor.get_position() << ", " << rightMotor.get_position() << std::endl;
		pros::delay(50);
	}


}
