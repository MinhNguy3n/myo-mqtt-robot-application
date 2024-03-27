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

## Related publication
[1] M. Nguyen, T. N. Gia, and T. Westerlund, “EMG-based IoT System using Hand Gestures for Remote Control Applications,” in 2021 IEEE 7th World Forum on Internet of Things (WF-IoT), Jun. 2021, pp. 911–912. doi: [10.1109/WF-IoT51360.2021.9595957](https://doi.org/10.1109/wf-iot51360.2021.9595957).
