#include <Wire.h>
//Define important addresses
//MPU-6050 Address
#define MPU6050 0x68
//Address of power managment register
#define PWR_MGMT_1 0x6B
//Accel register address
#define ACC_XOUT_H 0x3B
//Gyro register address
#define GYRO_XOUT_H 0x43
//Accel config address (for the full-scale range)
#define ACCEL_CONFIG 0x1C
//Gyro config address (for the full-scale range)
#define GYRO_CONFIG 0x1B
//Define important numbers
#define N_Samples 200
#define PI 3.14159
#define A .96


//Variables to store the time
double Timer, Time, Prev_time, dt;
//Variables to store the calibration values
float Acc_x_Calibrated, Acc_y_Calibrated, Gyro_x_Calibrated, Gyro_y_Calibrated, Gyro_z_Calibrated;
//Buffer variables to store the angles
float Acc_Angle_x, Acc_Angle_y, Gyro_Angle_x, Gyro_Angle_y, Gyro_Angle_z;
//Variables to store the output angles
float yaw, pitch, roll;

void setup() {
  Wire.begin();
  Serial.begin(1000);
  //Activate the MPU-6050
  Wire.beginTransmission(MPU6050);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00);
  Wire.endTransmission();

  //Set full-scale range of ACCEL_CONFIG to ±2g (default)
  Wire.beginTransmission(MPU6050);
  Wire.write(ACCEL_CONFIG);
  //Set the register bits as 0b00000000
  Wire.write(0x00); 
  Wire.endTransmission(true);
  
  //Set full-scale range of GYRO_CONFIG to ±250°/s (default)
  Wire.beginTransmission(MPU6050);
  Wire.write(GYRO_CONFIG);
  //Set the register bits as 0b00000000
  Wire.write(0x00);
  Wire.endTransmission(true);

  delay(20);
  //Call calibration function
  Mpu_Calibration();
}



void loop() {
  //Define necessary variables
  float Acc_x = 0;
  float Acc_y = 0;
  float Acc_z = 0;
  float Gyro_x = 0;
  float Gyro_y = 0;
  float Gyro_z = 0;
    
  //Get ACCEL readings
  Wire.beginTransmission(MPU6050); 
  Wire.write(ACC_XOUT_H);
  Wire.endTransmission();
  Wire.requestFrom(MPU6050, 6);
  while(Wire.available()<6);
  Acc_x = (Wire.read() << 8 | Wire.read()) / 16384.0;
  Acc_y = (Wire.read() << 8 | Wire.read()) / 16384.0;
  Acc_z = (Wire.read() << 8 | Wire.read()) / 16384.0;
  //Calulate the angles
  Acc_Angle_x = (atan(Acc_y / sqrt(pow(Acc_x, 2) + pow(Acc_z, 2))) * 180 / PI) - Acc_x_Calibrated;
  Acc_Angle_y = (atan(-1 * Acc_x / sqrt(pow(Acc_y, 2) + pow(Acc_z, 2))) * 180 / PI) - Acc_y_Calibrated;

  //Calculate dt for the Complimentry Filter 
  Prev_time = Time;
  Time = millis();
  //Divide by 1000 to get dt in seconds
  dt = (Time - Prev_time) / 1000;

  //Get GYRO readings
  Wire.beginTransmission(MPU6050); 
  Wire.write(GYRO_XOUT_H);
  Wire.endTransmission();
  Wire.requestFrom(MPU6050, 6);
  while(Wire.available()<6);
  Gyro_x = (Wire.read() << 8 | Wire.read()) / 131.0 - Gyro_x_Calibrated;
  Gyro_y = (Wire.read() << 8 | Wire.read()) / 131.0 - Gyro_y_Calibrated;
  Gyro_z = (Wire.read() << 8 | Wire.read()) / 131.0 - Gyro_z_Calibrated;

  //Use Complimentry Filter to claculate the angles
  Gyro_Angle_x += Gyro_x * dt;
  Gyro_Angle_y += Gyro_y * dt;
  Gyro_Angle_z += Gyro_z * dt;

  //Get the final values
  roll = A * Gyro_Angle_x + (1 - A) * Acc_Angle_x;
  pitch = A * Gyro_Angle_y + (1 - A) * Acc_Angle_y;
  yaw = Gyro_Angle_z;

//Display the angles
  Serial.println("Roll angle: " + String(roll));
  Serial.println("Pitch angle: " + String(pitch));
  Serial.println("Yaw angle: " + String(yaw));
  Serial.println(("--------------------"));
}

//Calibration function
void Mpu_Calibration(){
  //Define necessary variables
  float Acc_x_sum = 0;
  float Acc_y_sum = 0;
  float Acc_z_sum = 0;

  //Loop for the required number of samples
  for(int i = 0; i < N_Samples; i++)
  {
    //Get ACCEL readings
    Serial.println("Begin " + String(i));
    Wire.beginTransmission(MPU6050);
    Wire.write(ACC_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(MPU6050, 6);
    while(Wire.available()<6);
    Acc_x_sum += (Wire.read() << 8 | Wire.read()) / 16384.0;
    Acc_y_sum += (Wire.read() << 8 | Wire.read()) / 16384.0;
    Acc_z_sum += (Wire.read() << 8 | Wire.read()) / 16384.0;

    //Get GYRO readings  
    Wire.beginTransmission(MPU6050);
    Wire.write(GYRO_XOUT_H);
    Wire.endTransmission();
    Wire.requestFrom(MPU6050, 6);
    while(Wire.available()<6);
    Gyro_x_Calibrated += (Wire.read() << 8 | Wire.read()) / 131.0;
    Gyro_y_Calibrated += (Wire.read() << 8 | Wire.read()) / 131.0;
    Gyro_z_Calibrated += (Wire.read() << 8 | Wire.read()) / 131.0;

    Acc_x_Calibrated += (atan(Acc_y_sum / sqrt(pow(Acc_x_sum, 2) + pow(Acc_z_sum, 2))) * 180 / PI);
    Acc_y_Calibrated += (atan(-1 * Acc_x_sum / sqrt(pow(Acc_y_sum, 2) + pow(Acc_z_sum, 2))) * 180 / PI);

  }
  //Divide the mean by the number of samples
  Acc_x_Calibrated /= N_Samples;
  Acc_y_Calibrated /= N_Samples;
  Gyro_x_Calibrated /= N_Samples;
  Gyro_y_Calibrated /= N_Samples;
  Gyro_z_Calibrated /= N_Samples;
}