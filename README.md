# Myo armband robot control application using MQTT

## Command protocol

| Pose | arm's position| MQTT message | Serial command | Robot move |
| --- | --- |:---:|:---:|---:|
| WAVE_OUT | at chest level | ROTATE_CW | 1 | Bottom servo rotate clockwise |
| WAVE_IN | at chest level | ROTATE_CCW | 2 | counter-clockwise |
| FIST | raised | RAISE | 3 | Left servo rotate clockwise |
| FIST | lowered | LOWER | 4 | Left servo rotate counter-clockwise |
| FINGERS_SPREAD | at chest level | STRETCH | 5 | Right servo rotate counter-clockwise |
| FIST | at chest level | SHORTEN | 6 | Left servo rotate clockwise |
| DOUBLE_TAP | any | DOUBLE_TAP | 0 | Hand servo close/open |
| REST | any | - | - | Stop on-going move |
