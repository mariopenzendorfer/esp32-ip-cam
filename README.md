# esp32-ip-cam

The goal of this project is, to provide a cloudless, lightweight and cheap MJPEG IP camera, that is also compatible with [homeassistant's MJPEG IP Camera integration](https://www.home-assistant.io/integrations/mjpeg/).

This project is using the ESP32-CAM module. Different camera modules can be used as per instructions [below](#changing-the-camera-module).

## The ESP32 Firmware

### Installation

#### Prerequisites

* VSCode
* PlatformIO extension for VSCode
* ESP32-CAM module (e. g. https://amzn.eu/d/cETEouy)
* USB cable, compatible with ESP32-CAM (e. g. USB-C to USB-A)

#### Instructions

1. Create the file _src/wifi_config.h_ based on this template:

```cpp
#define WIFI_SSID "ssid"
#define WIFI_PASSWORD "password"
```

2. Build the project using PlatformIO
3. Connect the ESP32-CAM module to your computer
4. Upload the binary using PlatformIO

### Custom adjustments

#### Changing the camera module

Different camera modules can be supported by adjusting the `camera_config_t` configuration in _src/main.cpp_.

Only the relevant variables are shown in the below code segments.

##### ESP32-CAM

``` cpp
camera_config_t camera_config = {
    .pin_pwdn = 32,
    .pin_reset = -1,
    .pin_xclk = 0,
    .pin_sccb_sda = 26,
    .pin_sccb_scl = 27,
    .pin_d7 = 35,
    .pin_d6 = 34,
    .pin_d5 = 39,
    .pin_d4 = 36,
    .pin_d3 = 21,
    .pin_d2 = 19,
    .pin_d1 = 18,
    .pin_d0 = 5,
    .pin_vsync = 25,
    .pin_href = 23,
    .pin_pclk = 22,

    ...
};
```

##### ESP32 WROVER CAM

``` cpp
camera_config_t camera_config = {
    .pin_pwdn = -1,
    .pin_reset = -1,
    .pin_xclk = 21,
    .pin_sccb_sda = 26,
    .pin_sccb_scl = 27,
    .pin_d7 = 35,
    .pin_d6 = 34,
    .pin_d5 = 39,
    .pin_d4 = 36,
    .pin_d3 = 19,
    .pin_d2 = 18,
    .pin_d1 = 5,
    .pin_d0 = 4,
    .pin_vsync = 25,
    .pin_href = 23,
    .pin_pclk = 22,

    ...
};
```

##### ESP32S3 WROOM CAM

``` cpp
camera_config_t camera_config = {
    .pin_pwdn = 38,
    .pin_reset = -1,
    .pin_xclk = 15,
    .pin_sccb_sda = 4,
    .pin_sccb_scl = 5,
    .pin_d7 = 16,
    .pin_d6 = 17,
    .pin_d5 = 18,
    .pin_d4 = 12,
    .pin_d3 = 10,
    .pin_d2 = 8,
    .pin_d1 = 9,
    .pin_d0 = 11,
    .pin_vsync = 6,
    .pin_href = 7,
    .pin_pclk = 13,

    ...
};
```

#### Changing the maximum framerate

Different frame rates can be configured by changing the `MAX_FPS` define in _src/main.cpp_.

``` cpp
#define MAX_FPS 20
```

#### Changing image quality

The most important settings referring the image quality are `frame_size` and `jpeg_quality`. Changing these settings can massively influence the achievable frame-rate.

##### Frame-Size

Set the `frame_size` setting to the desired value.

For a full list of frame sizes, check out the `framesize_t` enum in the [official esp32-camera library](https://github.com/espressif/esp32-camera/blob/master/driver/include/sensor.h) on github.

Some of the most popular settings are

| Setting        | Resolution |
| -------------- | ---------- |
| FRAMESIZE_QVGA | 320x240    |
| FRAMESIZE_CIF  | 400x296    |
| FRAMESIZE_HVGA | 480x320    |
| FRAMESIZE_VGA  | 640x480    |
| FRAMESIZE_SVGA | 800x600    |
| FRAMESIZE_XGA  | 1024x768   |
| FRAMESIZE_HD   | 1280x720   |
| FRAMESIZE_SXGA | 1280x1024  |
| FRAMESIZE_UXGA | 1600x1200  |

##### JPEG Quality

Set the `jpeg_quality` setting to a value from 0-63, where a lower value means higher quality.

``` cpp
camera_config_t camera_config = {
    ...

    .frame_size = FRAMESIZE_QVGA,
    .jpeg_quality = 12,

    ...
};

```
