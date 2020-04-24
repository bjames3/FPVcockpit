Version 0.2
- BUG: Issue #19 Incorrect timings in PPMOut
- CHG: Issue #18 Use of Timer1 refactored for shared use
- CHG: Issue #20 Signal properties Center and Travel have been moved to utility functions
- ADD: Issue #2  PPMIn, PPM reader/decoder (trainer port)
- ADD: Issue #16 ServoIn, Servo signal reader/decoder
- ADD: Issue #17 ServoOut, Servo signal writer/encoder
- CHG: Issue #22 Remove all use of normalized values from Signal generating/reading code
- CHG: Issue #23 Timer1 code and interrupts moved to separate class

Version 0.1
- ADD: AIPin, analog input; trim, reverse, calibration
- ADD: Channel, transformations; end points, reverse, subtrim
- ADD: Curve, for throttle and pitch curves
- ADD: DIPin, digital input; reverse
- ADD: DualRates
- ADD: Expo, for exponential input
- ADD: Gyro, support AVCS and Normal gyros
- ADD: PPMOut, configurable PPM generator; channels, pulse length, servo center, servo travel, invert, pin
- ADD: Swashplate, support all common types and mixing