#include "main.h"

//------Defining the motors.------
pros::Motor FLMotor (FLMotorport, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor BLMotor (BLMotorport, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor FRMotor (FRMotorport, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor BRMotor (BRMotorport, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor Intake (Intakeport, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor Arm (Armport, pros::E_MOTOR_GEARSET_36, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor Shooter1 (Shooter1port, pros::E_MOTOR_GEARSET_18, pros::E_MOTOR_ENCODER_DEGREES);
pros::Motor Shooter2 (Shooter2port, pros::E_MOTOR_GEARSET_18, true, pros::E_MOTOR_ENCODER_DEGREES);
pros::Controller master (CONTROLLER_MASTER);
pros::Controller partner (CONTROLLER_PARTNER);

//------Variables------
    bool IsBreaking = false;
    bool IsForward = true;
    bool IsShooterHot = false;
    bool IsFlipping = false;
    bool IsOverHeigh = false;
    int flipTarget = 350;
    int flipingSpeed = 100;
    int ArmCeiling = 1250;

//These map the joisticks to be used later for turning motors
int LeftControls = master.get_analog(ANALOG_LEFT_Y);
int RightControls = master.get_analog(ANALOG_RIGHT_Y);
int ArmControls = partner.get_analog(ANALOG_LEFT_Y);
int IntakeControls = partner.get_analog(ANALOG_RIGHT_Y);

//Returns true/false as to wheter the drive wheels have
//reached their position goal set by driveForDistance
bool AtDistanceDriveGoal(int threshold) {
  return (abs(FLMotor.get_position() - FLMotor.get_target_position()) < threshold) &&(abs(FLMotor.get_position() - FLMotor.get_target_position()) < threshold);
}

//Sets drive trains target, but does not wait for them to reach their target
void Drive(double leftInches, double rightInches, int speed) {
  FRMotor.move_relative(leftInches, speed);
  BRMotor.move_relative(rightInches, -speed);
  pros::delay(25);
  FLMotor.move_relative(rightInches, speed);
  BLMotor.move_relative(leftInches, -speed);
}

//Turns the robot to the target position
void Rotate(double turn, int speed) {
  FLMotor.move_relative(-turn , speed);
  FRMotor.move_relative(turn, speed);
  BLMotor.move_relative(-turn, speed);
  BRMotor.move_relative(turn, speed);
}

//Function for setting the drive trian breaks
void BrakeDriveTrain() {
  IsBreaking = true;
  FLMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  FRMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  BLMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  BRMotor.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  FLMotor.move_relative(0,200);
  FRMotor.move_relative(0,200);
  BLMotor.move_relative(0,200);
  BRMotor.move_relative(0,200);
}

//Function for releasing the drive train breaks
void UnBrakeDriveTrain() {
  IsBreaking = false;
  FLMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  FRMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  BLMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
  BRMotor.set_brake_mode(pros::E_MOTOR_BRAKE_COAST);
}

//Function for seting the cap flipper side to be the front side
void SetBackwords() {
  FLMotor.set_reversed(true);
  FRMotor.set_reversed(false);
  BLMotor.set_reversed(true);
  BRMotor.set_reversed(false);
  IsForward = false;
}

//Function for seting the ball shooter side to be the front side
void SetForwards() {
  FLMotor.set_reversed(false);
  FRMotor.set_reversed(true);
  BLMotor.set_reversed(false);
  BRMotor.set_reversed(true);
  IsForward = true;
}

//Function for turning on the shooter
void ShooterOn(int velocity) {
  Shooter1.move_velocity(velocity);
  Shooter2.move_velocity(velocity);
}

//Function for turning off the shooter
void ShooterOff() {
  ShooterOn(0);
}

void ShootTwice() {

}

//
void Arm_fn(void* param) {
  Arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  while (true) {
    //Resets the arm's starting position for the auto cap flip button if the button is pressed.
    if (partner.get_digital(DIGITAL_DOWN)) {
      Arm.set_zero_position(0);
    }

    if (partner.get_digital(DIGITAL_L1)) {
      Arm.move(100);
      pros::delay(300);
      Arm.move(-100);
      pros::delay(300);
      Arm.move(0);
    }
    else if (master.get_digital(DIGITAL_L2)) {
      Arm.move(100);
      pros::delay(300);
      Arm.move(-100);
      pros::delay(300);
      Arm.move(0);
    }
    else {
      Arm.move(ArmControls);
    }

    //doesn't let the arm to go over the 18 inches expantion limit unless a button is pressed on the partner controller.
    if(abs(int(Arm.get_position() >= ArmCeiling && partner.get_digital(DIGITAL_A) == false))) {
      IsOverHeigh = true;
      Arm.move(-50);
    }
    else {
      IsOverHeigh = false;
    }

    //Resets the arm's starting position for the auto cap flip button if the button is pressed.
    if (partner.get_digital(DIGITAL_DOWN)) {
      Arm.set_zero_position(0);
    }
  }
}

//Function for the red flag side of the field
void RedFlag() {
  Intake.move(600);
  Drive(3500, 3500, 100);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Drive(-3600,-3600, 100);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Intake.move(0);
  pros::delay(300);
  Rotate(750, 50);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Intake.move(-100);
  pros::delay(300);
  Intake.move(0);
  ShooterOn();
  Drive(3300,3300, 100);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Drive(-900,-1700, 100);
  pros::delay(1600);
  Intake.move(100);
  pros::delay(600);
  Intake.move(0);

  Rotate(100, 50);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));
  Drive(-1400, -1600, 100);
  pros::delay(2400);
  Intake.move(100);
  pros::delay(300);
  Intake.move(0);
  ShooterOff();
}

//Function for the red cap side of the field
void RedCap() {
  Intake.move(50);
  Drive(3500, 3500, 100);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Drive(-600, -600, 100);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Rotate(900, 50);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Drive(-800, -800, 100);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Arm.move_relative(600, 227);
  pros::delay(600);
  Intake.move(0);
}

//Function for the blue flag side of the field
void BlueFlag() {
  Intake.move(600);
  Drive(3500, 3500, 100);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Drive(-3500,-3500, 100);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Intake.move(0);
  pros::delay(300);
  Rotate(-770, 50);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Intake.move(-100);
  pros::delay(300);
  Intake.move(0);
  ShooterOn();
  Drive(3500,3500, 100);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Drive(-1700,-900, 100);
  pros::delay(1500);
  Intake.move(100);
  pros::delay(500);
  Intake.move(0);

  Drive(-1400, -1500, 100);
  pros::delay(2400);
  Intake.move(100);
  pros::delay(300);
  Intake.move(0);
  ShooterOff();
}

//Function for the blue cap side of the field
void BlueCap() {
  Intake.move(50);
  Drive(3500, 3500, 100);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));
  Drive(-600, -600, 100);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));
  Rotate(-900, 50);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));
  Drive(-840, -840, 100);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));
  Arm.move_relative(600, 227);
  pros::delay(600);
  Intake.move(0);
}

//Function for skills auton
void SkillsAuton() {
  Intake.move(600);
  Drive(3500, 3500, 50);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Drive(-3600,-3600, 50);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Intake.move(0);
  pros::delay(300);
  Rotate(750, 30);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Intake.move(-100);
  pros::delay(300);
  Intake.move(0);
  ShooterOn();
  Drive(3300,3300, 50);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  Drive(-900,-1700, 50);
  pros::delay(800);
  Intake.move(100);
  pros::delay(600);
  Intake.move(0);

  Rotate(100, 30);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));
  Drive(-1400, -1600, 100);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));
  pros::delay(2400);
  Intake.move(100);
  pros::delay(300);
  Intake.move(0);
  ShooterOff();
  pros::delay(400);

  //Turn strait and backup to allign with the platform.
  Rotate(20, 30);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));
  Drive(-1600, -1600, 50);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));

  //Rotate to face the platform.
  Rotate(790, 50); //if the rotate command doesn't work past this after it. pros::delay(1200);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));
  
  //Drive Up to the center platform.
  Drive(700, 700, 70);
  do {
    pros::delay(20);
  } while (!AtDistanceDriveGoal(5));
  
  //Lock drive train just incase.
  BrakeDriveTrain();
}

//Veriables and funstions for auton selector
int selection = 0;
const char *titles[] = {"Red Flag", "Red Cap", "Blue Flag", "Blue Cap","Skills Auton"};
void (*scripts[])() = {&RedFlag, &RedCap, &BlueFlag, &BlueCap,  &SkillsAuton};
void LCDScriptExecute() { scripts[selection](); }
