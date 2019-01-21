#include "main.h"

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

bool isForward = true;

bool isBreaking = false;

//------These map the joisticks to be used later for turning motors.------
int LeftControls = master.get_analog(ANALOG_LEFT_Y);
int RightControls = master.get_analog(ANALOG_RIGHT_Y);
int ArmControls = partner.get_analog(ANALOG_LEFT_Y);
int IntakeControls = partner.get_analog(ANALOG_RIGHT_Y);

//------sets drive trains target, but does not wait for them to reach their target.------
void driveForDistance(double leftInches, double rightInches) {
  FLMotor.move_relative(leftInches, 140);
  FRMotor.move_relative(rightInches, 140);

  BLMotor.move_relative(leftInches, 140);
  BRMotor.move_relative(rightInches, 140);
}


//------Function for setting the drive trian breaks.------
void brakeDriveTrain() {
  isBreaking = true;
  FLMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  FRMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  BLMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  BRMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  FLMotor.move_relative(0,200);
  FRMotor.move_relative(0,200);
  BLMotor.move_relative(0,200);
  BRMotor.move_relative(0,200);
}


//------Function for releasing the drive train breaks.------
void unBrakeDriveTrain() {
  isBreaking = false;
  FLMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  FRMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  BLMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  BRMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}


//------Function for seting the cap flipper side to be the front side.------
void setBackwords() {
  FLMotor.set_reversed(true);
  FRMotor.set_reversed(false);
  BLMotor.set_reversed(true);
  BRMotor.set_reversed(false);
  isForward = false;
}


//------Function for seting the ball shooter side to be the front side.------
void setForwards() {
  FLMotor.set_reversed(false);
  FRMotor.set_reversed(true);
  BLMotor.set_reversed(false);
  BRMotor.set_reversed(true);
  isForward = true;
}


//------Function for turning on the shooter.------
void shooterOn(int velocity) {
  Shooter1.move_velocity(velocity);
  Shooter2.move_velocity(velocity);
}

void shooterOff() {
  shooterOn(0);
}


void LeftNear() {
  FLMotor.move_relative(10000, 200);
  BRMotor.move_relative(10000, 200);
  BLMotor.move_relative(10000,200);
  pros::delay(150000);
}


void LeftFar() {

}


void RightNear() {

}


void RightFar() {

}


void SkillsAutonLeftNear() {

}


int selection = 0;

const char *titles[] = {"Left Near", "Left Far", "Right Near", "Right Far","skillsAutonLeftNear"};

void (*scripts[])() = {&LeftNear, &LeftFar, &RightNear, &RightFar,  &SkillsAutonLeftNear};

void lcdScriptExecute() { scripts[selection](); }



void autonomous() {
	//------tells the user that the autonomous faze has started.------
  pros::lcd::set_text(0, "autonomous has started");


  while (abs(int(FLMotor.get_position() - FLMotor.get_target_position())) > 5) {
    pros::delay(2);
  }
  while (abs(int(FRMotor.get_position() - FRMotor.get_target_position())) > 5) {
    pros::delay(2);
  }
  while (abs(int(BLMotor.get_position() - BLMotor.get_target_position())) > 5) {
    pros::delay(2);
  }
  while (abs(int(BRMotor.get_position() - BRMotor.get_target_position())) > 5) {
    pros::delay(2);
  }
  while (abs(int(Arm.get_position() - Arm.get_target_position())) > 5) {
    pros::delay(2);
  }
  while (abs(int(Intake.get_position() - Intake.get_target_position())) > 5) {
    pros::delay(2);
  }
  while (abs(int(Shooter1.get_position() - Shooter1.get_target_position())) > 5) {
    pros::delay(2);
  }
  while (abs(int(Shooter2.get_position() - Shooter2.get_target_position())) > 5) {
    pros::delay(2);
  }
}
