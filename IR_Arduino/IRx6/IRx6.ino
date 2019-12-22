// About pinMode()
// https://www.baldengineer.com/when-to-use-arduinos-pinmode-and-why.html

#define BAUD_RATE 115200
#define MAX_SPEED 65
#define MID_SPEED 45
#define MIN_SPEED 35
#define SPEED_STEP 5
#define MAX_DIFF 30

// analog output IO(io3, 5, 6, 9, 10, 11)
#define Left_PWM_1 11
#define Left_PWM_2 10
#define Right_PWM_1 9
#define Right_PWM_2 6

#define LEFT_M  0
#define RIGHT_M 1

int speed_base, speed_diff, speed_left, speed_right;
String inString = "";    // string to hold input

// speed_base_rate: [-1, 1]
// speed_diff_rate: [-1, 1]
void motor_control(float speed_base_rate, float speed_diff_rate){
  speed_base = (int)speed_base_rate * 255;
  speed_diff = (int)speed_diff_rate * 255;

  if(speed_base < 0)
  {speed_base = -speed_base;}

  speed_left  = speed_base - speed_diff;
  speed_right = speed_base + speed_diff;
}

void motor_control(unsigned char index, int speed){
  if(index == LEFT_M)
  {
    if(speed > 0)
    {
      analogWrite(Left_PWM_1, speed);
      analogWrite(Left_PWM_2, 0);
    }
    else if(speed < 0)
    {
      analogWrite(Left_PWM_1, 0);
      analogWrite(Left_PWM_2, 0 - speed);
    }
    else
    {
      analogWrite(Left_PWM_1, 0);
      analogWrite(Left_PWM_2, 0);
    }
  }
  else if(index == RIGHT_M)
    if(speed > 0)
    {
      analogWrite(Right_PWM_1, speed);
      analogWrite(Right_PWM_2, 0);
    }
    else if(speed < 0)
    {
      analogWrite(Right_PWM_1, 0);
      analogWrite(Right_PWM_2, 0 - speed);
    }
  else
  {
      analogWrite(Right_PWM_1, 0);
      analogWrite(Right_PWM_2, 0);
  }
}

void setup() {
  Serial.begin(BAUD_RATE);
  
  // analog output IO
  pinMode(Left_PWM_1,  OUTPUT);
  pinMode(Left_PWM_2,  OUTPUT);
  pinMode(Right_PWM_1, OUTPUT);
  pinMode(Right_PWM_2, OUTPUT);

  analogWrite(Left_PWM_1, 0);
  analogWrite(Left_PWM_2, 0);
  analogWrite(Right_PWM_1, 0);
  analogWrite(Right_PWM_2, 0);
}

void loop() {
  delay(5);
//  Serial.print("\n");
  // Read serial input:
  while (Serial.available() > 0) {
    int inChar = Serial.read();

    if (inChar != ' ') {

      // As long as the incoming byte
      // is not a newline,
      // convert the incoming byte to a char
      // and add it to the string
      inString += (char)inChar;
    }
    // if you get a newline, print the string,
    // then the string's value as a float:
    else {
      Serial.print("Input string: ");
      Serial.print(inString);
      Serial.print("\tAfter conversion to float:");
      Serial.println(inString.toFloat());
      // clear the string for new input:
      inString = "";
    }
  }
}
