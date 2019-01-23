#include "main.h"

void opcontrol() {
  //------Init------
    //prints "opcontrol has started" on the brain screen.
	  pros::lcd::set_text(0, "opcontrol has started");

    //Setting the right motors to be reversed
    FLMotor.set_reversed(false);
    FRMotor.set_reversed(true);
    BLMotor.set_reversed(false);
    BRMotor.set_reversed(true);

    //------Tells the arm to hold it's position.------
    Arm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);


  //------Variables------
      bool IsShooterHot = false;
      bool IsFlipping = false;
      bool IsOverHeigh = false;
      int flipTarget = 350;
      int flipingSpeed = 100;
      int ArmCeiling = 1000;


  while (true) {
    //------Monitoring code------
      //Prints the shooter temps to the brain screen.
      pros::lcd::print(5, "%d, %d", int(pros::c::motor_get_temperature(Shooter1port)), int(pros::c::motor_get_temperature(Shooter2port)));

      //if the motors are starting to over heat rumble the controllers.
      if (pros::c::motor_is_over_temp(Shooter1port) || pros::c::motor_is_over_temp(Shooter2port) && IsShooterHot == true) {
        master.rumble("- - -");
        master.rumble("");
        partner.rumble("- - -");
        partner.rumble("");
      }

    //------Joistics------
      //Gets the joistics position and maps them to a veriable.
      LeftControls = master.get_analog(ANALOG_LEFT_Y);
      RightControls = master.get_analog(ANALOG_RIGHT_Y);
      ArmControls = partner.get_analog(ANALOG_LEFT_Y);
      IntakeControls = partner.get_analog(ANALOG_RIGHT_Y);

      //Switches the motor sides if the drive train is reversed.
      if (IsBreaking != true) {
            if (IsForward == true) {
              FLMotor.move(LeftControls);
              BLMotor.move(LeftControls);
              FRMotor.move(RightControls);
              BRMotor.move(RightControls);
            }
            else {
              FLMotor.move(RightControls);
              BLMotor.move(RightControls);
              FRMotor.move(LeftControls);
              BRMotor.move(LeftControls);
            }
      }
    //------Master Controller------
      //Drive train break controls.
      if (master.get_digital_new_press(DIGITAL_UP)) {
        if (IsBreaking == true) {
          UnBrakeDriveTrain();
        }
        else {
          BrakeDriveTrain();
        }
      }


      //Drive train directional controls.
      if (master.get_digital_new_press(DIGITAL_L1)) {
        SetBackwords();
      }
      if (master.get_digital_new_press(DIGITAL_R1)) {
        SetForwards();
      }

      //Intake button for the master controller.
      if (master.get_digital(DIGITAL_R2)) {
        Intake.move(200);
      }
      else if (partner.get_digital_new_press(DIGITAL_R2)) {
        Intake.move(100);
        pros::delay(600);
        Intake.move(0);
      }
      else {
        //Makes the motor move if the joistic moves and the button isn't being pressed.
        Intake.move(IntakeControls);
      }

    //------Partner Controller------
      //Shooter controls.
      if (partner.get_digital(DIGITAL_R1)) {
        ShooterOn(200);
      }
      else {
        ShooterOff();
      }

      //Resets the arm's starting position for the auto cap flip button if the button is pressed.
      if (partner.get_digital(DIGITAL_DOWN)) {
        Arm.set_zero_position(0);
      }

      //If the L2 button on the master controller is pressed.
      //move the arm to the fliping target and set the IsFlipping veriable to true.
      //so the arm will lower after it reaches the target height.
      if (master.get_digital(DIGITAL_L2)) {
        Arm.move_absolute(flipTarget, flipingSpeed);
        IsFlipping = true;
      }
      //Does the same thing as the other one except this the L1 button on the partner controller.
      if (partner.get_digital(DIGITAL_L1)) {
        Arm.move_absolute(flipTarget, flipingSpeed);
        IsFlipping = true;
      }
      //Checks for if we pushed either of the buttons and if the arm has reached it's target.
      //If it has it moves it back down and sets the IsFlipping veriable to false telling allowing the joistic to control the arm again.
      if (IsFlipping == true && abs(int(Arm.get_position())) >= (flipTarget - 20)) {
        Arm.move_absolute(0, flipingSpeed);
        pros::delay(flipingSpeed + 100);
        IsFlipping = false;
      }
      //If the auto cap flipping buttons arn't active maps the arm motor to the joistics.
      if (IsFlipping == false && IsOverHeigh == false) {
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
    //delay for background code execution.
    pros::delay(2);
  }
}
