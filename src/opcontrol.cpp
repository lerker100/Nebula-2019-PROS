//------Importing stuff.------
#include "main.h"
#include "opcontrol.h"


void opcontrol() {
  //------tells the user that the opcontrol faze has started.------
	pros::lcd::set_text(0, "opcontrol has started");
  //------Tells if the auto cap flip button is running.------
  bool IsFlipping = false;
  //------Tells the arm to hold it's position.------
  CArm.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);

  while (true) {
    //------Gets the joistics position and sets it to a veriable.------
    CLeftControls = Cmaster.get_analog(ANALOG_LEFT_Y);
    CRightControls = Cmaster.get_analog(ANALOG_RIGHT_Y);
    CArmControls = Cpartner.get_analog(ANALOG_LEFT_Y);
    CIntakeControls = Cpartner.get_analog(ANALOG_RIGHT_Y);


    //------Shooter controls.------
    if (Cpartner.get_digital(DIGITAL_R1)) {
      CshooterOn(200);
    }
    else {
      CshooterOff();
    }


    //------Drive train break controls.------
    if (Cmaster.get_digital(DIGITAL_UP)) {
      CbrakeDriveTrain();
    }
    else {
      CunBrakeDriveTrain();
    }


    //------Drive train directional controls.------
    if (Cmaster.get_digital_new_press(DIGITAL_L1)) {
      CsetBackwords();
    }
    if (Cmaster.get_digital_new_press(DIGITAL_R1)) {
      CsetForwards();
    }


    //------Sets the Arm's starting position for the auto cap flip button.------
    if (Cpartner.get_digital(DIGITAL_DOWN)) {
      CArm.set_zero_position(0);
    }


    //------Sets the target position for the auto cap flip button------
    //650 sometimes double flipped
    int flipTarget = 620;
    //------Sets how fast the arm will flip the cap------
    int flipingSpeed = 200;
    //------If the L2 button on the master controller is pressed------
    //------move the arm to the fliping target and set the IsFlipping veriable to true------
    //------so the arm will lower after it reaches the target height.------
    if (Cmaster.get_digital(DIGITAL_L2)) {
      CArm.move_absolute(flipTarget, flipingSpeed);
      IsFlipping = true;
    }
    //------Does the same thing as the other one except this the L1 button on the partner controller.------
    if (Cpartner.get_digital(DIGITAL_L1)) {
      CArm.move_absolute(flipTarget, flipingSpeed);
      IsFlipping = true;
    }
    //------Checks for if we pushed either of the buttons and if the arm has reached it's target.------
    //------If it has it moves it back down and sets the IsFlipping veriable to false telling allowing the joistic to control the arm again.------
    if (IsFlipping == true && abs(int(CArm.get_position())) >= (flipTarget - 20)) {
      CArm.move_absolute(0, flipingSpeed);
      pros::delay(flipingSpeed);
      IsFlipping = false;
    }
    //------If the auto cap flipping buttons arn't active maps the arm motor to the joistics.------
    if (IsFlipping == false) {
      CArm.move(CArmControls);
    }


    //------Intake button for the master controller.------
    if (Cmaster.get_digital(DIGITAL_R2)) {
      CIntake.move(200);
    }
    else {
      //------Maps the intake motor to the intake joistic if the master controllers button isn't being pressed.------
      CIntake.move(CIntakeControls);
    }


    //------Switches the motor sides if the drive train is reversed.------
    if (CisForward == true) {
      CFLMotor.move(CLeftControls);
      CBLMotor.move(CLeftControls);
      CFRMotor.move(CRightControls);
      CBRMotor.move(CRightControls);
    }
    else {
      CFLMotor.move(CRightControls);
      CBLMotor.move(CRightControls);
      CFRMotor.move(CLeftControls);
      CBRMotor.move(CLeftControls);
    }
    pros::delay(2);
  }
}
